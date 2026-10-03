# Signal flow — 2002 Delphi exe vs. 2004 C++ source

## 2004 C++ signal flow (authoritative, verified from `src/SilverBox.cpp`)

Per-sample inside `CBProcess()`:

```
            ┌──────────────────────────────────────────────────────────────┐
            │                                                              │
            │   [Step Sequencer]  — timing from eSBN = sampleRate /        │
            │        (((tempo/tsNum) × (tsDen×tsNum)) / 60)                │
            │                                                              │
            │   on step boundary:                                          │
            │     • CBReset() — clears filter state                        │
            │     • loads pattern[step].pitch → note on                    │
            │     • sets accent and slide flags from pattern[step]         │
            │                                                              │
            └──────────────────────────────────────────────────────────────┘
                                        │
                                        ▼
   [Oscillator]  selectable saw or square (kWaveform), with kTuning offset
     • pitch drives via cNoteFreq[] table (60 entries, C0..B4)
     • if slide flag: lerp frequency toward next step's pitch across step
                                        │
                                        ▼
   [Envelope Generator]  single-stage decay
     • triggered on note-on from the step sequencer
     • decay time from kDecay
     • envelope modulates filter cutoff via kEnvMod depth
     • accent flag boosts amplitude + env depth via kAccent
                                        │
                                        ▼
   [Low-Pass Filter]  3-pole resonant (IIR_IIRlp3pole in src/IIR.cpp)
     • cutoff from kCutOffFreq + (envelope × kEnvMod) + accent boost
     • resonance from kResonance (can self-oscillate near 1.0)
     • followed by FIR anti-alias (IIR_FIRlp)
                                        │
                                        ▼
   [Distortion]  lookup-table soft clip (dist_dist in src/dist.cpp)
                                        │
                                        ▼
   [Volume]  kVolume master gain
                                        │
                                        ▼
   Stereo output (same signal to L and R)
```

## 2002 Delphi exe signal flow (inferred from DFM)

Architecturally the same pipeline (Frank wrote both), with two notable differences:

1. **Per-step parameter automation.** The Delphi UI has a `knobpage: TPageControl` with 7 tabs — Volume, Tune, Cutoff, Rezo, EnvMod, Decay, Accent — each containing a `TStringGrid`. Each grid captures values **per-step** (or at least per-breakpoint), meaning each parameter can sweep across a pattern. In the C++ VST this was collapsed to single global knobs with no per-step automation; the only per-step fields in `t_CB_Step` are `cPitch`, `bAccent`, `bSlide`.

2. **Non-realtime render.** The Delphi engine runs offline and writes `c:\cb303.wav`. There is no audio thread, no MIDI, no sync. The `Button4` ("Make Wave") handler walks the pattern-order list, renders each step into an in-memory buffer, and dumps the result.

## How per-step automation should work in the rebuild

The C++ DSP already supports per-sample updates to `kCutOffFreq`, `kResonance`, etc. via `setParam`. To match the 2002 behavior, the sequencer needs to call `setParam` at each step boundary with the grid's value for that step, before the oscillator processes the step's samples.

The per-step grid is stored in the `.cb` file's tail blocks (see `cb_schema.md`). Each block appears to encode a 2-breakpoint interpolation `[0, start, 2, 1, 0, end]` — the loader can expose this as "parameter ramps linearly from `start` to `end` across the pattern" or "parameter takes value `start` for pattern 1 and `end` for pattern 2" depending on what empirical playback shows. Start with the simpler interpretation: the `start` and `end` are parameter values applied to the first and last occurrence of that pattern in the order list. This can be refined once we can listen to the output.

## Parameter-to-stage map

| Parameter (Delphi / user-facing) | DSP stage | C++ source name |
|---|---|---|
| `rgType` waveform | Oscillator | `kWaveform` |
| `eBPM` tempo | Sequencer clock | computed into `eSBN` |
| `eOctave` | Note-entry helper (UI only — does not affect DSP directly; adds to typed pitch) | — |
| Tune-tab values | Oscillator pitch offset | `kTuning` (scaled per-step) |
| Cutoff-tab values | Filter | `kCutOffFreq` (per-step) |
| Rezo-tab values | Filter | `kResonance` (per-step) |
| EnvMod-tab values | Envelope → filter | `kEnvMod` (per-step) |
| Decay-tab values | Envelope | `kDecay` (per-step) |
| Accent-tab values | Envelope / amp | `kAccent` (per-step) |
| Volume-tab values | Amp | `kVolume` (per-step) |
| Per-step Note | Sequencer | `t_CB_Step.cPitch` |
| Per-step Slide | Sequencer / Oscillator | `t_CB_Step.bSlide` |
| Per-step Accent | Sequencer | `t_CB_Step.bAccent` |

## Verification checklist for Phase 1

- [ ] Load `303power.cb`, play pattern 1, confirm audible notes match pitches 37, 25, 25, 25, 25, 25, 25, 49, 37, 37, 37, 37, 37, 37, 37, 2 across 16 steps.
- [ ] Confirm slides applied on steps 4, 8, 12, 16.
- [ ] Confirm accents on steps 8, 15.
- [ ] Confirm tempo = 140 BPM.
- [ ] Confirm the per-step cutoff tab values sweep the filter audibly as the pattern runs.
