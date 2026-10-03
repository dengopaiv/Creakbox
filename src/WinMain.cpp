#include <windows.h>

#include "AudioEngine.h"
#include "CBEngine.h"
#include "MainWindow.h"
#include "MidiInput.h"

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int) {
    CBEngine engine;
    AudioEngine audio(engine);
    MidiInput midi(engine);

    if (!audio.start()) {
        MessageBoxA(nullptr, ("Audio startup failed: " + audio.lastError()).c_str(),
                    "Creakbox", MB_OK | MB_ICONERROR);
        return 1;
    }

    // MIDI is optional — absent device shouldn't prevent the app from starting.
    midi.start();

    MainWindow win(engine, audio, midi, hInst);
    if (!win.create()) {
        MessageBoxA(nullptr, "Failed to create main window.", "Creakbox", MB_OK | MB_ICONERROR);
        return 1;
    }

    return win.runMessageLoop();
}
