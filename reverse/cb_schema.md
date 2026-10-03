# `.cb` file schema — derived from inspection of 76 preset files

All `.cb` files are plain text, CRLF-delimited, Windows-1252.

## Top-level structure

```
<line 0>  "CreakBox"                                 magic
<line 1>  "1.0"                                      format version
<line 2>  <tempo BPM>                                e.g. 140, 150, 300
<lines 3-8>  <6 global / header values>              see "Global block"
<lines 9 … 9 + 49*P - 1>  <P patterns × 49 lines>    see "Per-pattern block"
<line 9 + 49*P>  "9999"                              first sentinel
<6 values>                                            grid tab block
<line …>  "9999"                                     next sentinel
<6 values>
...
<2 trailing values>                                   file trailer
```

## Empirical pattern-count formula

Section A length (all lines after the 3-line header up to but not including the first `9999`) fits:

```
section_A_length = 6 + 49 × P
```

where `P` is the number of patterns in the file. Confirmed across 75/76 files:

| Section A lines | Pattern count | Example files |
|---:|---:|---|
| 55  | 1  | 303power.cb, acid machine.cb, bassing.cb (38 files) |
| 104 | 2  | automatic.cb (20 files) |
| 202 | 4  | badbass.cb, badbass2.cb (9 files) |
| 398 | 8  | acid sequence.cb, d&bassline.cb (3 files) |
| 790 | 16 | longbreak.cb (3 files) |
| 1574 | 32 | acidmaker.cb (1 file) |

Outliers: `blutonium.cb` (58) and `softsynth.cb` (418) don't fit the formula cleanly — likely hand-edited or from a different tooling variant. The loader should tolerate them by scanning for the first `9999` rather than trusting computed offsets.

## Global block (6 values after the 3-line header)

From `303power.cb` lines 3–8: `[0, 1, 1, 40, 1, 1]`.

Meaning still to be fully confirmed against the DFM, but the trailing `40` in many files matches the `lbOrder` starting position. Likely: `[current_pattern_index, numOrderEntries, orderEntry[0], ???, pattern_octave, waveform_index]` or similar. The loader can round-trip these six values as an opaque blob if exact semantics aren't critical for playback.

## Per-pattern block (49 values per pattern)

For `303power.cb`'s single pattern (lines 9–57):
- Lines 9–56: 48 values = 16 steps × 3
- Line 57: 1 trailing value (in `303power.cb` = `50`) — likely pattern-level metadata (random amount? pattern length?)

Step triple layout: `[pitch, slide, accent]` for step N where N = (lineIdx - 9) / 3 + 1.

Observed pitch values are in the range 0–60, matching the C++ source's `cNoteFreq[61]` table (0 = rest, 1–60 = C0 … B4). Slide and accent are 0/1 flags.

Example from `303power.cb`:
| Step | pitch | slide | accent |
|---:|---:|---:|---:|
| 1 | 37 | 0 | 0 |
| 4 | 25 | 1 | 0 |
| 8 | 49 | 1 | 1 |
| 12 | 37 | 1 | 0 |
| 15 | 37 | 0 | 1 |
| 16 | 2 | 1 | 0 |

All plausible.

## Tail: grid-tab blocks (decoded via Ghidra of `CREAKBOX.EXE`)

After Section A, the file contains a series of blocks delimited by `9999`. In most files there are **7 sentinels** enclosing 6 internal blocks of 6 values each, then a 2-value trailer after the last sentinel. Each 6-value block follows the pattern `[0, X, 2, 1, 0, Y]` where only `X` and `Y` vary. These correspond to the seven `TStringGrid` tabs (Volume / Tune / Cutoff / Rezo / EnvMod / Decay / Accent). Each grid is 3 columns × 2 data rows = 6 cells.

Files with modified grid row counts (`acidmaker.cb`, `auto.cb`) show larger inter-sentinel gaps (31 and 23 lines respectively), reflecting grids with extra rows. Exact per-cell semantics (what each column represents) remains to be confirmed.

**Observed invariant positions** in every 6-line block sampled from 70+ `.cb` files:
```
block[0] = 0           (always)
block[1] = OFFSET      (0-100 — user-configurable)
block[2] = 2           (always)
block[3] = 1           (always)
block[4] = 0           (always)
block[5] = VALUE       (0 to 300+ — user-configurable)
```
Only positions 1 and 5 vary; positions 0/2/3/4 are fixed scaffolding for the StringGrid serialization.

The VST-source comment in `src/SilverBox.cpp` around the `getRezo()` helper reveals the breakpoint math Frank used:
```
temp = order + offset/100        // current pattern + fractional position
if (temp > nextRezo.offset) { prevRezo = nextRezo; posRezo++; ... }
result = prevRezo.value + (nextRezo.value - prevRezo.value)
       * ((temp - prevRezo.offset) / (nextRezo.offset - prevRezo.offset))
result = result / 100
result = sin(result * π/2)       // curved output
result = result * 75 + 25        // 25..100% range (for Rezo specifically)
```
So each tab stores breakpoints `(val1, val2, val3)` where offset = val1 + val2/100 and value = val3, interpolated linearly between consecutive breakpoints and then warped by a per-parameter curve. The two-row canonical `[0, OFFSET, 2, 1, 0, VALUE]` layout therefore encodes:
- Breakpoint 0: offset = 0 + OFFSET/100,  value = 2
- Breakpoint 1: offset = 1 + 0/100 = 1.0, value = VALUE

**Why "not safe to apply by default" in the rebuild:** values of 2 for row-0 (constant across all sampled files) combined with VALUE between 1 and 100 produce sweeps that drop Volume and Cutoff to ~0 mid-pattern for most real presets (e.g., `bassline.cb` Volume: offset=50, target=1 → volume falls to 1% by step 8). The per-parameter curve (sin-warp, 25..100 remap, etc.) isn't yet decoded for each of the seven tabs, and the row-0 "value = 2" is probably an unused default rather than a real breakpoint. Until this is fully decoded, applying raw sweeps silences many patterns.

### Decoded structure (from Ghidra analysis of FUN_0045f5e8 and FUN_00462100)

Each tab's grid is **3 columns × 3 rows**. Row 0 is a fixed header the UI initializes to `["Order", "Offset%", "Value%"]` and the file does not store. The two data rows (row 1 and row 2) are two breakpoints, and the file stores them **row-major**:

```
<row1.col0 = Order>     pattern index (0-based), or 9999 = "forever"
<row1.col1 = Offset%>   0-100, percent into that pattern
<row1.col2 = Value%>    0-100 (sometimes higher), parameter value
<row2.col0 = Order>
<row2.col1 = Offset%>
<row2.col2 = Value%>
```

Effective position of a breakpoint = `Order + Offset/100` (e.g. `(0, 50)` = halfway through pattern 0). The engine linearly interpolates between consecutive breakpoints; the first breakpoint's value is held from offset 0 up to its position; after the last breakpoint (or one with `Order == 9999`) the value is held indefinitely.

### Per-tab output curve

The VST source's `getRezo()` helper in `src/SilverBox.cpp` reveals the curve applied to the raw `Value%` field before it drives the DSP:

```
out = sin(Value/100 * π/2) * 0.75 + 0.25   // in 0..1
```

So `Value=0` maps to 25% of the parameter's range, `Value=50` to ~78%, and `Value=100` to 100%. **This is why raw file values like `Value=2` don't silence the synth** — the curve floors output at 25%. The `sin`-warping is non-linear so small Values open up quickly but the top end saturates.

The current rebuild applies this curve uniformly to all 7 tabs as a first approximation. Per-tab refinements (e.g. Tune centers at 0.5, not 0.25) are a future tweak.

### Tail layout in the file

Each tab's 6 data values are preceded by a `9999` sentinel line. For 7 tabs there are 7 sentinels + 7×6 = 49 tail lines, which matches the observed 45 + some variations across the sample set. Block 0 = Volume, block 1 = Tune, block 2 = Cutoff, block 3 = Rezo, block 4 = EnvMod, block 5 = Decay, block 6 = Accent — in the order the DFM `knobpage` `TPageControl` declares tabs.

### Invariant column pattern explained

The observation that position-2 of row 1 is always "2" in every sampled `.cb` file was a coincidence of the default Row-1 Value being `2` in most presets; not a fixed scaffolding value. Users who edited Row-1's Value column would have different numbers there.

## Loader implementation notes

- Split on `\r\n`, drop a trailing empty string from the final `\r\n`.
- Verify `lines[0] == "CreakBox"` and `lines[1] == "1.0"`.
- `tempo = int(lines[2])`.
- Scan for the first `9999` index → that's `section_A_end`.
- `section_A_length = section_A_end - 3`, derive `numPatterns = (section_A_length - 6) / 49`. If this isn't an integer, fall back to loading globals as opaque bytes and patterns as 49-line chunks until the first `9999`.
- Parse 16 step-triples per pattern.
- After the first `9999`, parse blocks delimited by subsequent `9999`s, taking `X` from position 1 and `Y` from position 5 of each 6-value block.
- Write support: reverse of the above, canonicalizing back to `[0, X, 2, 1, 0, Y]` for all seven grid blocks.

## Remaining open questions (not blocking v1)

1. Exact semantics of the 6 global-block values.
2. Exact semantics of the trailing pattern metadata (the 49th value).
3. What the 3rd column of each grid tab represents (the `[0, X, 2, 1, 0, Y]` shape hides two more column values — the `2, 1, 0` triple).
4. Why `blutonium.cb` is 58-line section A (3 extra values) and `softsynth.cb` is 418.

These can be deferred — the rebuild can round-trip the unknown fields byte-for-byte if it keeps them as raw strings.
