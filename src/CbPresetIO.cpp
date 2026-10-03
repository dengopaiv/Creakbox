#include "CbPresetIO.h"

#include <fstream>
#include <sstream>

namespace {
std::vector<std::string> splitCrlf(const std::string& data) {
    std::vector<std::string> out;
    std::string cur;
    for (size_t i = 0; i < data.size(); ++i) {
        char c = data[i];
        if (c == '\r') {
            out.push_back(cur);
            cur.clear();
            if (i + 1 < data.size() && data[i + 1] == '\n') ++i;
        } else if (c == '\n') {
            out.push_back(cur);
            cur.clear();
        } else {
            cur += c;
        }
    }
    if (!cur.empty()) out.push_back(cur);
    return out;
}

int toInt(const std::string& s, int fallback = 0) {
    try { return std::stoi(s); } catch (...) { return fallback; }
}
float toFloat(const std::string& s, float fallback = 0.0f) {
    try { return std::stof(s); } catch (...) { return fallback; }
}
} // namespace

std::optional<CbPreset> loadCb(const std::string& path, std::string& error) {
    std::ifstream f(path, std::ios::binary);
    if (!f) { error = "cannot open " + path; return std::nullopt; }
    std::ostringstream buf;
    buf << f.rdbuf();
    auto lines = splitCrlf(buf.str());
    while (!lines.empty() && lines.back().empty()) lines.pop_back();
    if (lines.size() < 10) { error = "file too short"; return std::nullopt; }
    if (lines[0] != "CreakBox") { error = "not a CreakBox file (header mismatch)"; return std::nullopt; }
    if (lines[1] != "1.0")      { error = "unexpected format version " + lines[1]; return std::nullopt; }

    CbPreset p;
    p.tempo = toFloat(lines[2], 140.0f);

    // Corrected format per Ghidra of Loadmodule1Click (FUN_00462adc):
    //   line 0: "CreakBox"
    //   line 1: "1.0"
    //   line 2: tempo
    //   line 3: lbNR count - 1 = N, so (N+1) order entries follow
    //   lines 4..4+N: (N+1) order-list items
    //   line 5+N:    pattern count P
    //   lines 6+N..5+N+48*P-1: P patterns, 48 lines each (16 steps x 3 cols)
    //   remaining lines: 7 tab grids, each (1 + 3*rowCount) lines where
    //                    rowCount is usually 2 (so 7 lines per tab).
    // "9999" is NOT a sentinel between sections - it's a legitimate "Order:
    // forever" value in the Order column of row 2 of each grid.
    size_t pos = 3;
    const int lbNrCount = toInt(lines[pos++]) + 1;  // line 3: N = lbNR-1
    p.globalsRaw.clear();
    p.globalsRaw.reserve(1 + lbNrCount + 1);
    p.globalsRaw.push_back(std::to_string(lbNrCount - 1));
    for (int i = 0; i < lbNrCount && pos < lines.size(); ++i) {
        p.globalsRaw.push_back(lines[pos++]);
    }
    if (pos >= lines.size()) { error = "truncated in order list"; return std::nullopt; }
    const int patternCount = toInt(lines[pos]);
    p.globalsRaw.push_back(lines[pos]);
    pos++;
    if (patternCount < 1 || patternCount > 256) { error = "unreasonable pattern count"; return std::nullopt; }

    for (int pat_i = 0; pat_i < patternCount; ++pat_i) {
        if (pos + 48 > lines.size()) { error = "truncated in pattern data"; return std::nullopt; }
        CbPattern pat;
        pat.lastStep = 15;
        for (int s = 0; s < 16; ++s) {
            pat.steps[s].pitch  = toInt(lines[pos + s*3 + 0]);
            pat.steps[s].slide  = toInt(lines[pos + s*3 + 1]) != 0;
            pat.steps[s].accent = toInt(lines[pos + s*3 + 2]) != 0;
        }
        p.patterns.push_back(pat);
        pos += 48;
    }

    // Tail: 7 tab grids. Each grid is: 1 line (rowCount-1) + rowCount*3 lines.
    // Typical rowCount=2, giving 7 lines per tab. Read up to 7 tabs.
    for (int t = 0; t < 7 && pos < lines.size(); ++t) {
        if (pos >= lines.size()) break;
        int rows = toInt(lines[pos]);
        pos++;
        if (rows < 1 || rows > 32) break;
        // Row 1 = bp0 (Order, Offset, Value); row 2 = bp1 (Order, Offset, Value)
        auto get = [&](int off, int fallback = 0) -> int {
            return (pos + off < lines.size()) ? toInt(lines[pos + off]) : fallback;
        };
        p.bp0[t].order  = get(0);
        p.bp0[t].offset = get(1);
        p.bp0[t].value  = get(2);
        if (rows >= 2) {
            p.bp1[t].order  = get(3);
            p.bp1[t].offset = get(4);
            p.bp1[t].value  = get(5);
        } else {
            p.bp1[t] = { 9999, 0, p.bp0[t].value };  // single row = hold forever
        }
        pos += rows * 3;
    }

    return p;
}

bool saveCb(const std::string& path, const CbPreset& preset, std::string& error) {
    std::ofstream f(path, std::ios::binary);
    if (!f) { error = "cannot open " + path + " for writing"; return false; }

    auto write = [&](const std::string& s) { f << s << "\r\n"; };

    write("CreakBox");
    write("1.0");
    // Round tempo to int if integral (the 2002 app wrote ints), otherwise 1dp
    {
        int itp = static_cast<int>(preset.tempo);
        if (static_cast<float>(itp) == preset.tempo) write(std::to_string(itp));
        else {
            char buf[32]; _snprintf_s(buf, _TRUNCATE, "%.1f", preset.tempo);
            write(buf);
        }
    }
    // Globals (variable length):
    //   lbNR count - 1, then (count) order entries, then pattern count.
    const int nPat = static_cast<int>(preset.patterns.size());
    const int lbCount = nPat;
    write(std::to_string(lbCount - 1));
    for (int i = 0; i < lbCount; ++i) {
        // Preserve original order list if present, else default 1..N
        if (i + 1 < static_cast<int>(preset.globalsRaw.size())) {
            write(preset.globalsRaw[i + 1]);
        } else {
            write(std::to_string(i + 1));
        }
    }
    write(std::to_string(nPat));

    // Patterns: 48 lines each (16 steps x 3 cols interleaved).
    //
    // Fixed at 16 on purpose. The 2002 format has no field for a pattern
    // length and the Delphi reader counts lines, so a pattern of any other
    // length simply cannot be expressed here - a 23-step pattern loses steps
    // 17-23 and a 5-step one gains eleven rests. That is a real limitation of
    // .cb, not an oversight, and it does not bite in practice because nothing
    // in the UI reaches saveCb(): .cb is import-only. Use .roq (v10 or later)
    // to save anything that is not 16 steps.
    for (const auto& pat : preset.patterns) {
        for (int s = 0; s < 16; ++s) {
            write(std::to_string(pat.steps[s].pitch));
            write(pat.steps[s].slide  ? "1" : "0");
            write(pat.steps[s].accent ? "1" : "0");
        }
    }

    // 7 tab grids. Each: rowCount-1 line, then rowCount rows of 3 cells.
    for (int t = 0; t < 7; ++t) {
        write("2");  // rowCount - 1 = 2 -> 2 data rows (bp0 and bp1)
        write(std::to_string(preset.bp0[t].order));
        write(std::to_string(preset.bp0[t].offset));
        write(std::to_string(preset.bp0[t].value));
        write(std::to_string(preset.bp1[t].order));
        write(std::to_string(preset.bp1[t].offset));
        write(std::to_string(preset.bp1[t].value));
    }
    return true;
}
