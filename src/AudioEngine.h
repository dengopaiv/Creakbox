#pragma once

#include <atomic>
#include <string>
#include <thread>

class CBEngine;

// WASAPI shared-mode output. Starts a dedicated render thread that pulls audio
// from CBEngine::process and pushes it to the default Windows audio endpoint.
class AudioEngine {
public:
    explicit AudioEngine(CBEngine& engine);
    ~AudioEngine();

    // Starts COM + WASAPI render loop. Returns true on success.
    // On failure, lastError() explains what went wrong.
    bool start();
    void stop();

    const std::string& lastError() const { return lastError_; }
    double sampleRate() const { return sampleRate_; }

private:
    void renderThread();

    CBEngine& engine_;
    std::thread thread_;
    std::atomic<bool> stop_{false};
    std::string lastError_;
    double sampleRate_ = 0.0;
};
