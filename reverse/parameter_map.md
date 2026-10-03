# Parameter map — derived from `CREAKBOX.EXE` Form1.dfm

Every user-facing control exposed by the 2002 Delphi UI, as extracted from `reverse/forms/Form1.dfm`. This is the parameter surface the user's `.cb` presets were built against; the accessible standalone should mirror this surface first, and only surface additional C++-source parameters (MIDI learn, cycle start/end, etc.) in a secondary "Advanced" panel.

## Global synth parameters

| Delphi control | Type | Meaning | Range / Default |
|---|---|---|---|
| `rgType` | `TRadioGroup` | Oscillator waveform | `Saw`, `Square` (default: Square, `ItemIndex=1`) |
| `eBPM` | `TEdit` | Tempo | numeric BPM, default `150` (2002 exe clamps 40–300) |
| `eOctave` | `TEdit` + `btnOctMinus`/`btnOctUp` | Current octave for note-entry on the step grid | default `2`, adjusted ± via buttons |
| `eRandom` | `TEdit` | "Random Wildness" — seed/amount for `Chaos pattern` menu commands | default `50`, percent-like |
| `eFilename` | `TEdit` | Output WAV path for `Make Wave` | default `c:\cb303.wav` (rebuild should drop this — we render realtime) |

## Per-step controls (16 steps)

Each of the 16 steps has three accessible fields, all tagged `Tag=1..16` so the event handlers can identify the step:

| Delphi controls | Type | Meaning |
|---|---|---|
| `eNote1` … `eNote16` | `TEdit` with `OnKeyDown=eNote1KeyDown`, `OnKeyPress=eNote1KeyPress` | Note text (pitch). Typed as letter + optional accidental (C, C#, D, …). Three of them (`eNote1`, `eNote5`, `eNote9`, `eNote13`) are color-coded `14869218` — beat-1 of each group-of-4, purely visual |
| `cbSlide1` … `cbSlide16` | `TCheckBox` with `OnClick=cbSlide1Click` | Slide/portamento flag |
| `cbAccent1` … `cbAccent16` | `TCheckBox` with `OnClick=cbAccent1Click` | Accent flag |

## Per-step parameter grids (7 tabs)

`knobpage: TPageControl` with 7 tabs, each containing a `TStringGrid` (3 columns × variable rows). These are **per-step** parameter overrides — this is the crucial architectural difference from a traditional TB-303 where these are global knobs:

| Tab | Grid | Parameter |
|---|---|---|
| `TabSheet1` / `volume` | `gridVolume` | Per-step volume (or envelope attack level) |
| `tsTune` / `tune` | `gridTune` | Per-step pitch detune |
| `tsCutoff` / `cutoff` (default active tab) | `gridCutoff` | Per-step filter cutoff |
| `tsRezo` / `rezo` | `gridRezo` | Per-step resonance |
| `tsEnvMod` / `envmod` | `gridEnvMod` | Per-step filter envelope amount |
| `tsDecay` / `decay` | `gridDecay` | Per-step envelope decay time |
| `tsAccent` / `accent` | `gridAccent` | Per-step accent amount |

All grids share `OnKeyDown=gridTuneKeyDown` (the handler is reused) and column widths `(36, 44, 66)` — the 3 columns likely represent `[step number, value-from, value-to]` for interpolated sweeps across a pattern, but exact semantics need confirmation against the `.cb` schema.

## Pattern / order navigation

| Delphi control | Type | Meaning |
|---|---|---|
| `lbNR` | `TListBox` | "Order number" list — which pattern plays at each position in the song order. Default: single entry `"1"` |
| `lbOrder` | `TListBox` | The pattern-number column paired with `lbNR`. Default: single entry `"1"` |
| `btnAdd` | `TButton` "add" | Append an entry to the order |
| `btnDel` | `TButton` "del" | Delete the selected order entry |
| `btnPlus` | `TButton` "+" | Increase the pattern number at the selected order slot |
| `btnMinus` | `TButton` "-" | Decrease the pattern number at the selected order slot |
| `seFROM` | `TSpinEdit` | Render start pattern (`Min=1, Max=1`, grows as patterns are added) |
| `seTO` | `TSpinEdit` | Render end pattern (`Min=1, Max=1`, grows with patterns) |

## Transport / rendering

| Delphi control | Type | Meaning |
|---|---|---|
| `Button4` | `TButton` "Make Wave" | Render the `FROM`-`TO` range to the WAV path — **the only "transport"**. In the rebuild this becomes realtime `Run/Stop` |
| `ProgressBar` | `TProgressBar` | Render progress (0–100) |

## Menu + shortcuts

| Menu item | Shortcut | Handler |
|---|---|---|
| File → New | `Ctrl+N` | `New1Click` |
| File → Load module | `Ctrl+L` | `Loadmodule1Click` — reads a `.cb` file |
| File → Save module | `Ctrl+S` | `Savemodule1Click` — writes a `.cb` file |
| File → Exit | — | — |
| Edit → Copy pattern | `Ctrl+C` | `Copypattern1Click` |
| Edit → Cut pattern | `Ctrl+X` | `Cutpattern1Click` |
| Edit → Paste pattern | `Ctrl+V` | `Pastepattern1Click` |
| Edit → Pattern -1 | `Ctrl+T` | `Pattern11Click` |
| Edit → Pattern +1 | `Ctrl+U` | `Pattern12Click` |
| Edit → Chaos pattern | `Ctrl+R` | `Chaospattern1Click` — randomize (uses `eRandom`) |
| Edit → Chaos pattern nicely | `Ctrl+E` | `Chaospatternnicely1Click` |
| Popup → Octave Up | `Ctrl+W` | `btnOctUpClick` |
| Popup → Octave Dn | `Ctrl+V` | `btnOctMinusClick` |
| Popup → Add OrderNR | `Ctrl+M` | `btnAddClick` |
| Popup → Delete OrderNR | `Ctrl+N` | `btnDelClick` |
| Popup → Rotate pattern up | `Shift+Up` | `Rotatepatternup1Click` |
| Popup → Rotate pattern down | `Shift+Down` | `rotatepatterndown1Click` |

## File dialogs

- `sdSave` — save dialog, `DefaultExt='cb'`, filter `'CreakBox file|*.cb'`
- `odOpen` — open dialog, `DefaultExt='cb'`, filter `'CreakBox file|*.cb'`

## Summary — what the rebuild's v1 UI must expose

Mirror the 2002 surface, one accessible control at a time. Specifically:

1. **Global**: waveform combo, tempo edit, octave edit+/-, random-amount edit.
2. **Step list** (16 rows × 3 editable columns: Note, Slide, Accent).
3. **Per-step parameter grid** — 7 tabs (Volume / Tune / Cutoff / Rezo / EnvMod / Decay / Accent). Accessible rendering: for each parameter, a 16-row ListView where each row is one step's value for that parameter. A parameter-picker combo selects which parameter to edit.
4. **Pattern navigator**: order list, add/del, pattern +/-, from/to range.
5. **Transport**: Run/Stop replaces `Make Wave`; no WAV-file-path entry needed (realtime).
6. **Menu**: File (New / Load `.cb` / Load `.fxb` / Save `.cb` / Save `.fxb` / Exit), Edit (copy / cut / paste / pattern +- / chaos / chaos nicely / octave ± / rotate).
