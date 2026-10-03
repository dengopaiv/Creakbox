#pragma once

#include "CBEngine.h"
#include <optional>
#include <string>
#include <vector>

// Reads / writes 2002 Bass-Assessor `.cb` text preset files. Format decoded in
// reverse/cb_schema.md via Ghidra of CREAKBOX.EXE. Each of 7 tabs holds a 2x3
// grid of breakpoints with columns (Order, Offset%, Value%):
//   Order:  pattern index (0-based), 9999 = "forever"
//   Offset: 0-100 percent into that pattern
//   Value:  0-100 percent parameter value
// Effective position = Order + Offset/100; linearly interpolated.
struct CbBreakpoint {
    int order  = 0;    // 0..numPatterns, or 9999 = forever
    int offset = 0;    // 0..100 percent within the pattern
    int value  = 0;    // 0..100 (or higher for some tabs) percent parameter value
};

struct CbPreset {
    float tempo = 140.0f;
    std::vector<CbPattern> patterns;
    std::vector<std::string> globalsRaw;          // 6 values after header, opaque
    std::vector<float> trailingPatternMetadata;   // one per pattern (the 49th)
    // Per-tab breakpoint pairs: index 0..6 for Volume, Tune, Cutoff, Rezo,
    // EnvMod, Decay, Accent (in file order).
    CbBreakpoint bp0[7];
    CbBreakpoint bp1[7];
};

// Returns nullopt if parsing fails (with `error` set). Accepts files whose
// section-A length doesn't match the `55 + 49*P` formula — outliers like
// `blutonium.cb` and `softsynth.cb` fall into the unknown-trailing-bytes bucket.
std::optional<CbPreset> loadCb(const std::string& path, std::string& error);

// Writes a canonical .cb file. Patterns, tempo, and tab values round-trip.
// Fields we don't understand are written as zeros (safe default — v2 can
// preserve originals).
bool saveCb(const std::string& path, const CbPreset& preset, std::string& error);
