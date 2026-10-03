#pragma once

// The rest of the acid FX block: a compressor and a ring modulator, sitting in
// the same place as the distortion models — after the filter and the volume
// envelope, where a pedal sits relative to a real 303's output.
//
// The admission criterion for this block is that an effect must not break the
// loop: per-sample, no delay lines, nothing time-based that would stop a
// one-pattern loop from repeating identically. Distortion and compression are
// memoryless or near-memoryless waveshapers and qualify outright. The ring
// modulator qualifies only because its carrier phase is re-zeroed on every new
// note (see cbRingProcess) — a free-running audio-rate carrier would drift
// against the bar and the loop would stop repeating.
//
// Chain order in CBEngine::process is distortion -> compressor -> ring -> clamp.
// The compressor sits before the ring deliberately: its envelope follower would
// otherwise chase the carrier's amplitude modulation and partly cancel the very
// effect the ring amount control is asking for, so the two knobs would fight in
// a way neither of them explains.
//
// Real-time safe: no allocation, all per-block constants computed once in the
// prepare functions, and both effects are skipped entirely when off rather than
// multiplied by a neutral value — the off path has to leave no state behind.

// ---------------------------------------------------------------------------
// Compressor. Two controls, threshold and depth: "slight" is the specification,
// glue that evens out accents, not a limiter. The release time is fixed in code
// rather than exposed, because four controls would make it a different pedal
// from the one that was asked for; the attack is instant, which is what makes
// the output bound provable rather than measured.
// ---------------------------------------------------------------------------
struct CbCompCoeffs {
    bool   on     = false;
    double thresh = 1.0;    // linear amplitude where compression starts
    double kk     = 0.0;    // 0 = no compression, 1 = hard limit at thresh
    double makeup = 1.0;    // auto make-up, restores unity at full scale
    double relA   = 0.0;    // release coefficient; the follower's attack is
                            // instant, deliberately - see AcidFx.cpp
};

struct CbCompState {
    double env = 0.0;
    void reset() { env = 0.0; }
};

// `threshold` and `depth` are 0..1 as they come off the UI sliders.
// depth == 0 leaves `on` false, and the caller must then skip cbCompProcess.
void   cbCompPrepare(CbCompCoeffs& c, double threshold, double depth,
                     double sampleRate);
double cbCompProcess(const CbCompCoeffs& c, CbCompState& s, double x);

// ---------------------------------------------------------------------------
// Ring modulator. Carrier frequency is independent of the note, which is what
// makes it clangorous and inharmonic — the sound people mean by "ring mod on a
// 303" — rather than a well-behaved extra oscillator.
// ---------------------------------------------------------------------------
struct CbRingCoeffs {
    bool   on       = false;
    double amount   = 0.0;  // 0 = dry, 1 = pure ring
    double phaseInc = 0.0;  // cycles per sample
};

// `hz` is a real frequency, not a slider position; `amount` is 0..1.
// amount == 0 leaves `on` false, and the caller must then skip cbRingProcess.
void   cbRingPrepare(CbRingCoeffs& c, double hz, double amount, double sampleRate);

// `phase` is owned by the caller (it lives in CBEngine so a new note can
// re-zero it) and is in cycles, 0..1.
double cbRingProcess(const CbRingCoeffs& c, double& phase, double x);
