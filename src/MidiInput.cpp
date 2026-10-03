#include "MidiInput.h"
#include "CBEngine.h"

#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

MidiInput::MidiInput(CBEngine& engine) : engine_(engine) {}

MidiInput::~MidiInput() { stop(); }

bool MidiInput::start() {
    const UINT n = midiInGetNumDevs();
    devices_.clear();
    for (UINT i = 0; i < n; ++i) {
        MIDIINCAPSA caps{};
        if (midiInGetDevCapsA(i, &caps, sizeof(caps)) == MMSYSERR_NOERROR) {
            devices_.emplace_back(caps.szPname);
        }
    }
    if (n == 0) {
        lastError_ = "No MIDI input devices available.";
        return false;
    }

    HMIDIIN hMidi = nullptr;
    const MMRESULT r = midiInOpen(&hMidi, 0,
        reinterpret_cast<DWORD_PTR>(&MidiInput::midiProc),
        reinterpret_cast<DWORD_PTR>(this),
        CALLBACK_FUNCTION);
    if (r != MMSYSERR_NOERROR) {
        char buf[128]{};
        midiInGetErrorTextA(r, buf, sizeof(buf));
        lastError_ = "midiInOpen failed: " + std::string(buf);
        return false;
    }
    handle_ = hMidi;
    midiInStart(hMidi);
    return true;
}

void MidiInput::stop() {
    if (handle_) {
        midiInStop(static_cast<HMIDIIN>(handle_));
        midiInClose(static_cast<HMIDIIN>(handle_));
        handle_ = nullptr;
    }
}

void __stdcall MidiInput::midiProc(void* /*hMidi*/, unsigned wMsg, unsigned long long dwInst, unsigned long long dwP1, unsigned long long /*dwP2*/) {
    if (wMsg != MIM_DATA) return;
    auto* self = reinterpret_cast<MidiInput*>(dwInst);
    const BYTE status   = static_cast<BYTE>(dwP1 & 0xFF);
    const BYTE data1    = static_cast<BYTE>((dwP1 >> 8) & 0xFF);
    const BYTE data2    = static_cast<BYTE>((dwP1 >> 16) & 0xFF);
    const BYTE cmd      = status & 0xF0;
    if (cmd == 0x90) {  // note on
        if (data2 == 0) self->engine_.noteOff(data1);
        else            self->engine_.noteOn(data1, data2);
    } else if (cmd == 0x80) {  // note off
        self->engine_.noteOff(data1);
    }
    // MIDI clock (0xF8) and other messages ignored in v1.
}
