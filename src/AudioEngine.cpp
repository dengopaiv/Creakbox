#include "AudioEngine.h"
#include "CBEngine.h"

#include <windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <avrt.h>
#include <functiondiscoverykeys_devpkey.h>

#include <algorithm>
#include <vector>

#pragma comment(lib, "avrt.lib")

namespace {
struct ComInit {
    HRESULT hr;
    ComInit()  { hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED); }
    ~ComInit() { if (SUCCEEDED(hr)) CoUninitialize(); }
};

template <class T>
void safeRelease(T*& p) { if (p) { p->Release(); p = nullptr; } }

std::string hrToString(HRESULT hr) {
    char buf[64];
    _snprintf_s(buf, _TRUNCATE, "HRESULT=0x%08lX", static_cast<unsigned long>(hr));
    return buf;
}
} // namespace

AudioEngine::AudioEngine(CBEngine& engine) : engine_(engine) {}

AudioEngine::~AudioEngine() {
    stop();
}

void AudioEngine::stop() {
    stop_.store(true);
    if (thread_.joinable()) thread_.join();
}

bool AudioEngine::start() {
    stop_.store(false);
    thread_ = std::thread([this] { renderThread(); });
    // Wait briefly for the thread to publish sampleRate_ or an error.
    for (int i = 0; i < 200 && sampleRate_ == 0.0 && lastError_.empty(); ++i) {
        Sleep(5);
    }
    return lastError_.empty();
}

void AudioEngine::renderThread() {
    ComInit com;
    if (FAILED(com.hr)) { lastError_ = "CoInitializeEx failed: " + hrToString(com.hr); return; }

    IMMDeviceEnumerator* enumerator = nullptr;
    IMMDevice* device = nullptr;
    IAudioClient* client = nullptr;
    IAudioRenderClient* render = nullptr;
    WAVEFORMATEX* mixFormat = nullptr;
    HANDLE evt = nullptr;
    HANDLE taskHandle = nullptr;
    UINT32 bufferFrames = 0;
    int channels = 0;

    HRESULT hr = CoCreateInstance(
        __uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
        __uuidof(IMMDeviceEnumerator), reinterpret_cast<void**>(&enumerator));
    if (FAILED(hr)) { lastError_ = "MMDeviceEnumerator create failed: " + hrToString(hr); goto cleanup; }

    hr = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
    if (FAILED(hr)) { lastError_ = "GetDefaultAudioEndpoint failed: " + hrToString(hr); goto cleanup; }

    hr = device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, reinterpret_cast<void**>(&client));
    if (FAILED(hr)) { lastError_ = "Activate IAudioClient failed: " + hrToString(hr); goto cleanup; }

    hr = client->GetMixFormat(&mixFormat);
    if (FAILED(hr)) { lastError_ = "GetMixFormat failed: " + hrToString(hr); goto cleanup; }

    // Force float32 stereo at the mix-format sample rate for simplicity.
    // WASAPI shared-mode will accept this if the endpoint is configured as float.
    // For robustness on endpoints that report int16, we'd convert here; for now
    // we trust the default Windows mix-format to be float32.
    if (mixFormat->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
        // Already float32 extensible — great.
    } else if (mixFormat->wFormatTag != WAVE_FORMAT_IEEE_FLOAT) {
        lastError_ = "Unexpected default audio format (not float).";
        goto cleanup;
    }

    sampleRate_ = mixFormat->nSamplesPerSec;
    engine_.setSampleRate(sampleRate_);

    evt = CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
    if (!evt) { lastError_ = "CreateEventEx failed"; goto cleanup; }

    {
        const REFERENCE_TIME bufferDuration = 300000;  // 30 ms
        hr = client->Initialize(
            AUDCLNT_SHAREMODE_SHARED,
            AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM | AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY,
            bufferDuration, 0, mixFormat, nullptr);
        if (FAILED(hr)) { lastError_ = "IAudioClient::Initialize failed: " + hrToString(hr); goto cleanup; }
    }

    hr = client->SetEventHandle(evt);
    if (FAILED(hr)) { lastError_ = "SetEventHandle failed: " + hrToString(hr); goto cleanup; }

    hr = client->GetService(__uuidof(IAudioRenderClient), reinterpret_cast<void**>(&render));
    if (FAILED(hr)) { lastError_ = "GetService render failed: " + hrToString(hr); goto cleanup; }

    {
        DWORD task = 0;
        taskHandle = AvSetMmThreadCharacteristicsA("Pro Audio", &task);
    }

    hr = client->GetBufferSize(&bufferFrames);
    if (FAILED(hr)) { lastError_ = "GetBufferSize failed: " + hrToString(hr); goto cleanup; }

    hr = client->Start();
    if (FAILED(hr)) { lastError_ = "IAudioClient::Start failed: " + hrToString(hr); goto cleanup; }

    channels = mixFormat->nChannels;
    {
        std::vector<float> scratch;
        scratch.reserve(bufferFrames * 2);

        while (!stop_.load()) {
            DWORD wait = WaitForSingleObject(evt, 500);
            if (wait != WAIT_OBJECT_0) continue;

            UINT32 padding = 0;
            if (FAILED(client->GetCurrentPadding(&padding))) break;
            UINT32 available = bufferFrames - padding;
            if (available == 0) continue;

            BYTE* dst = nullptr;
            if (FAILED(render->GetBuffer(available, &dst))) break;

            scratch.assign(static_cast<size_t>(available) * 2, 0.0f);
            engine_.process(scratch.data(), static_cast<int>(available));

            float* out = reinterpret_cast<float*>(dst);
            if (channels == 2) {
                std::copy(scratch.begin(), scratch.end(), out);
            } else {
                // Fan stereo into N channels, zero the rest.
                for (UINT32 f = 0; f < available; ++f) {
                    const float L = scratch[f * 2];
                    const float R = scratch[f * 2 + 1];
                    for (int c = 0; c < channels; ++c) {
                        out[f * channels + c] = (c == 0) ? L : (c == 1 ? R : 0.0f);
                    }
                }
            }

            render->ReleaseBuffer(available, 0);
        }

        client->Stop();
    }

cleanup:
    if (taskHandle) AvRevertMmThreadCharacteristics(taskHandle);
    if (evt) CloseHandle(evt);
    if (mixFormat) CoTaskMemFree(mixFormat);
    safeRelease(render);
    safeRelease(client);
    safeRelease(device);
    safeRelease(enumerator);
}
