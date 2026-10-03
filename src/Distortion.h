#pragma once

// Distortion models for the stage that sits after the filter and the volume
// envelope — the same place a pedal sits relative to a real 303's output.
//
// The stock 2004 clipper (dist.cpp) is NOT touched and stays the default: type
// Classic routes to it directly, so the default sound is bit-identical to the
// engine before this file existed. Everything here is opt-in.
//
// Which models, and why these: the ProCo RAT is the canonical acid distortion —
// it is on the large majority of pre-2000 303 records, and Hardfloor famously
// ran several. Its character comes from three things in series: an LM308 op-amp
// whose slow slew rate is already distorting before anything else, symmetric
// diodes clipping hard to ground, and a passive low-pass "Filter" control
// (roughly 475 Hz to well above audio). The Tube Screamer is the other pole of
// the comparison — soft clipping inside the feedback loop, bass kept out of the
// clipper, a mid push — and it is the one people reach for when the RAT is too
// brutal. The MXR Distortion+ is hard clipping without the slew limiting, so it
// stays brighter and rawer. Fuzz covers the germanium end that several players
// prefer for sheer aggression over the RAT's precision.
//
// Real-time safe: no allocation, no branching on anything but the type, and all
// per-block constants are computed once in cbDistPrepare().

enum class CbDistType {
    Classic = 0,   // the stock 2004 clipper — the default, and bit-identical
    Rat,
    Screamer,
    DistPlus,
    Fuzz,
    Count
};

// Per-block constants. Recomputed only when the block starts, never per sample.
struct CbDistCoeffs {
    int    type    = 0;
    double drive   = 1.0;   // pre-clip gain
    double level   = 1.0;   // output make-up
    double preLpA  = 0.0;   // one-pole coefficient, slew limiting before clip
    double postLpA = 0.0;   // one-pole coefficient, the tone control after clip
    double hpA     = 0.0;   // one-pole coefficient, bass kept out of the clipper
    double bias    = 0.0;   // asymmetry, for the fuzz
};

// Per-voice filter memory.
struct CbDistState {
    double preLp  = 0.0;
    double postLp = 0.0;
    double hpZ    = 0.0;
    void reset() { preLp = postLp = hpZ = 0.0; }
};

// `gain`, `tone`, `level` are all 0..1 as they come off the UI sliders.
void   cbDistPrepare(CbDistCoeffs& c, int type, double gain, double tone,
                     double level, double sampleRate);

// One sample. Only called when type != Classic; Classic goes to dist_dist().
double cbDistProcess(const CbDistCoeffs& c, CbDistState& s, double x);
