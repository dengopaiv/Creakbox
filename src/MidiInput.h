#pragma once

#include <string>
#include <vector>

class CBEngine;

// Opens the default WinMM MIDI input device (first one enumerated) and routes
// note-on / note-off messages to CBEngine. MIDI clock messages drive tempo if
// external-sync mode is enabled (deferred to a later pass).
class MidiInput {
public:
    explicit MidiInput(CBEngine& engine);
    ~MidiInput();

    bool start();
    void stop();

    const std::string& lastError() const { return lastError_; }
    std::vector<std::string> enumeratedDevices() const { return devices_; }

private:
    CBEngine& engine_;
    void* handle_ = nullptr;   // HMIDIIN
    std::string lastError_;
    std::vector<std::string> devices_;

    static void __stdcall midiProc(void* hMidi, unsigned wMsg, unsigned long long dwInst, unsigned long long dwP1, unsigned long long dwP2);
};
