# Creakbox — outstanding work

All independent now. G took `.roq` v9, B took v10 and H took v11; nothing outstanding
wants a format change. Line references are current as of 2026-09-04.

Background on any of it is in `README.md` (what the program does) and `CLAUDE.md` (the
invariants and the traps). Rounds 1–3 — the PWM LFO, noise / resonance boost / reverse
step, and the documentation plus keyboard rework — are done and verified.

**Everything on this list is done:** ~~E~~, ~~A~~, ~~J~~, ~~G~~, ~~B~~, ~~H~~, ~~I~~, ~~C~~,
~~F~~, ~~K~~.

The honest gap is that **almost none of it has been heard** — only the resonance boost and
the PWM LFO have been confirmed by ear. Everything else is verified numerically and nothing
more. That list is at the bottom of this file, and it is the whole of the outstanding work.

---

## E. Sweep parameter ordering — RESOLVED 2026-09-03, no code change needed

**Verdict: the two tables are both correct and must not be reconciled.** Documented in
place rather than "fixed"; comments added at both tables and in `CBEngine.h`.

Interactive sweeps use UI slider order `{Volume, Cutoff, Resonance, EnvMod, Decay, Accent,
Tuning}`; legacy `.cb` sweeps use 2002 tab order `{Volume, Tuning, Cutoff, Resonance,
EnvMod, Decay, Accent}`. Tuning is last in one and second in the other. Three independent
sources agree that the legacy order is the file's real order:

1. `reverse/parameter_map.md` — the Delphi `Form1.dfm` `TPageControl` tab declaration
   order: `TabSheet1/volume`, `tsTune`, `tsCutoff`, `tsRezo`, `tsEnvMod`, `tsDecay`,
   `tsAccent`.
2. `reverse/cb_schema.md` — Ghidra of `CREAKBOX.EXE`: "the seven TStringGrid tabs
   (Volume / Tune / Cutoff / Rezo / EnvMod / Decay / Accent)".
3. The loader: `CbPresetIO::loadCb` reads the tail grids sequentially into `bp0[t]`/`bp1[t]`
   with `t` = file order and no remapping, and `MainWindow` copies `bp*[i]` → `sweeps[i]`
   one-for-one. Index equals file tab index from disk all the way to the apply site.

"Correcting" either table would silently mis-map every sweep in all 56 shipped `.cb`
modules.

**Second finding, and the reason the originally planned test was impossible.** The plan
said to verify by loading a `.cb` with tuning sweeps and listening. That cannot work: the
legacy apply path is **inert**. `setSweeps` has exactly one caller
(`MainWindow.cpp:1470`), which passes `enabled=false` deliberately, and `sweepsEnabled_`
defaults to false — so `if (sweepsOn)` never runs. Legacy sweeps are parsed and
round-tripped but never heard.

The rationale is at the call site and in `reverse/cb_schema.md`: the per-tab value curves
are not fully decoded, and applying the raw values silences many real presets (row-0 values
of 2 against a 0–100 range drop Volume and Cutoff to near zero mid-pattern). Both the
inertness and the reason are now commented at the apply site so nobody deletes it as dead
code or re-enables it without decoding the curves first.

**If legacy sweeps are ever turned on**, that is its own piece of work: decode the
per-parameter curves (`sin(x·π/2)`, the 25–100 % remap for Rezo, etc. — partially recorded
in `cb_schema.md`), and decide whether they *replace* slider values as in 2002 or
*modulate* them, which is the more useful behaviour and what the call-site comment suggests.

**Verified:** comments-only change; the full round-2 engine suite still passes, including
all three bit-identical proofs, so the engine is unchanged.

**Dengo's ruling on why they stay parsed at all:** legacy sweeps live in the file because
they are needed to convert an original `.cb` should anyone ever have one with sweeps in it.
That is a conversion obligation, not a playback feature — which is precisely why parsing
them and never applying them is the correct state, and why the parse path must not be
deleted as dead code.

---

## A. Audio-thread allocation — RESOLVED 2026-09-03

**`process()` now performs zero heap allocations and takes no lock.** It previously
deep-copied the whole song plus both sweep systems under `patternMu_` on every callback.

**What changed.** Everything the audio thread reads about the song moved into a
`SongData` blob, double-buffered in `CBEngine`:

- `process()` publishes the buffer index it intends to read into `readerIdx_`, re-reads
  `liveIdx_`, and retries if a writer flipped in between. Publish-then-recheck is the
  load-bearing part: without it a writer could pick the very buffer the reader had just
  chosen but not yet announced. It then takes a `const SongData&` — no copy, no lock.
- Writers go through `SongWriter` (RAII): take `writerMu_` — which the audio thread never
  touches — wait for `readerIdx_` to leave the idle buffer, copy live into it, mutate,
  then `commit()` flips `liveIdx_`. All waiting is on the UI thread and is bounded by one
  audio block. No commit means nothing is published, which is what every bounds-check
  early return wants.
- `patternIdx_` became a plain `std::atomic<int>`. It is the one field *both* threads
  write (the sequencer advances it, the UI selects a pattern), so it deliberately sits
  outside `SongData` — and that is what removed the audio thread's **second** lock, the
  one inside the pattern-advance path.
- `pat` is now a `const CbPattern*` into the live buffer rather than a per-block copy.

Copying live into idle reuses the idle buffer's existing vector capacity, so steady-state
editing does not allocate on either thread.

**Verified** — and the bit-exact standard deliberately did not carry over, as predicted:

1. **Zero allocations.** `test_a alloc` installs an `operator new` hook armed only around
   `process()` and renders 400 blocks of a 32-pattern song with all seven sweep lists
   populated. Result: **0**. The **negative control matters more than the result** —
   the same test against the pre-change engine reports **3200 allocations over 400
   blocks, exactly 8 per block**, matching the diagnosis (one patterns vector plus seven
   sweep vectors). A test that cannot fail proves nothing; this one fails on the old code.
2. **No torn state.** `test_a stress` runs a writer that swings the song between 1 and 64
   patterns so the vector reallocates on essentially every commit, against a render thread
   using 32-frame blocks. 60 000 blocks against 11 004 concurrent commits: no non-finite
   samples, nothing outside ±1, no crash. Reallocation is what makes this sensitive — a
   reader holding a pointer into a buffer being rewritten would be dereferencing freed
   storage.
3. **Bit-exact under quiescence.** `compare_golden.py` renders all 11 harness modes and
   compares byte-for-byte against `golden_preA/`, captured from the engine immediately
   before the change. All 11 identical. With no writer there is no flip, so the audio
   thread sees exactly what it saw before and the DSP must agree exactly.

The round-1 and round-2 suites still pass in full, so nothing else moved.

**Still open, and adjacent:** `setRunning()` calls `resetFilters()` from the UI thread
while the audio thread may be mid-block, which races on the DSP state (`boxOsc_`, the
filter memories). That predates this work and is untouched by it. It is unlikely to be
audible — the values are overwritten immediately — but it is a genuine data race that a
sanitiser would flag, and the fix is a "please reset" atomic flag the audio thread acts on
at the top of its next block.
**Promoted to its own item — see I below.**

---

## B. Variable pattern length — RESOLVED 2026-09-04

Patterns are now **1 to 64 steps**, set per pattern on a **Pattern length** combo (`Alt+G`)
placed just before the step list in tab order. `.roq` **v10** carries it.

**Almost nothing had to change in the engine.** `CbPattern` always carried 64 steps and the
sequencer always looped at `lastStep`, derived song position from `lastStep + 1` and picked
the next step from it. Two things were missing: a control that could say anything but 16,
and a writer that could save it.

**What was actually broken.** `.roq` round-tripped the `lastStep` *number* while both loops
read and wrote exactly 16 step rows. A 23-step pattern would have saved its declared length
and silently lost steps 17–23 — worse than refusing, because the file still looks valid.

### The format: why v10 is not just another bump

Every bump up to v9 appended a field, which a reader can default. v10 changes the *number of
step rows in a pattern block*, and a row count is positional: an old reader handed a long
pattern runs off the end of it and every field after it — including the sweep block — is
garbage.

So **the writer stamps the lowest version that can represent the song**, not the newest
(`neededVersion` in `RoqIO.cpp`). A song whose patterns are all 16 steps writes exactly the
bytes v9 wrote and still opens in an older build; only a song that actually uses another
length declares v10 and gives that up. The header number means "what this file needs", not
"what wrote it". Verified against all 25 shipped modules: every one still writes as v9.

`.hape` was pinned to its own constant at the same time. It shares the counter's history but
its schema has never changed, and letting it ride the bump would have made sweep exports
unreadable by older builds to describe a change that did not happen to them.

### The two things that were not obvious

1. **A sweep endpoint has to be normalised by the length of the pattern it names**, not by
   the length of the one playing. That was the same number for every pattern until now.
   Once lengths differ, "pattern 0, step 15" divided by a 4-step pattern's length is song
   position 3.75, and the sweep stays active three patterns too long. Fixed with `posOf` in
   `process()`, which also clamps an endpoint into its own pattern, so shortening a pattern
   under an existing sweep does not fling the endpoint downstream.
2. **`sweepsOverlap` mapped (pattern, step) to `pat * 16 + step`.** With a longer pattern in
   play, pattern 0 step 20 and pattern 1 step 4 both map to 20, so the overlap check
   compares the wrong intervals. Stride is now 64 — the maximum length, which is all the
   mapping needs to stay collision-free and increasing.

Also length-aware now, each of which would otherwise quietly write to steps the pattern does
not play: transpose, randomise-current-pattern (it keeps the length — it replaces notes, not
shape), the per-step toggles, the step list, and the sweep editor's step combos, which are
sized to the pattern selected beside them and refill when that selection changes.

**Shortening is not destructive within a session** — the steps past the new end keep their
notes and lengthening brings them back. Saving is where they go, which is what makes a short
pattern short on disk. `.cb` stays 16 and cannot be otherwise; commented at `saveCb`.

### Verified

- **`len4`, `len23`, `period4`, `period7`, `period16` and `sweepnone` render byte-identical
  to the pre-change engine.** This is the strongest statement available here: those modes
  use no parameter that did not already exist, so an older binary renders them too, and
  bit-equality says the sweep-positioning edit did not leak into the ordinary path.
- **The sequencer loops where it says it does.** One note on step 0, rests elsewhere; the
  per-step energy is then periodic, and the measured period is 4, 7 and 16 respectively. The
  period search takes the *shortest* period that fits, not the best-scoring one — a
  4-periodic signal scores just as well at 8 and 12, and without that the test happily
  passes a pattern of the wrong length. It did, first time round.
- **A sweep stays inside its pattern.** A 16-step pattern followed by a 4-step one, with a
  Cutoff sweep over pattern 0 only: steps 2–4 of the short pattern are unaffected
  (5.8e−07), where the pre-change engine swept straight through them (1.4e−01). The first
  step of the short pattern *does* differ, by 1.5e−01 — that is the filter carrying state
  across the boundary, and it is checked for being a ring-out rather than assumed to be one:
  it collapses 2236x within the first eighth of the step, where a still-running sweep would
  persist.
- **Format.** A mixed 23/5/16 song round-trips all three lengths, all 23 steps of the long
  pattern with flags, the patterns *after* it still aligned, and the sweep block after them
  still read correctly — that last one is what catches a reader losing its place. Steps past
  a pattern's end load as rests. An all-16 song still declares v9 and still writes 16 rows.
  All 25 shipped modules load, re-save and remain v9.
- Everything from rounds 1–5 still passes: golden bit-exactness, J, G, bounds, `test_a`
  alloc and stress. Release builds with no warnings; the app starts and closes cleanly.

**Worth a listen, not a bug:** 15 against 16 is the obvious first thing to try — it walks the
accents around the bar. A short pattern under a long sweep is the cheap way to a phrase that
never repeats the same way twice. Whether the length combo announces usefully as you arrow
through 64 entries is a screen-reader question this cannot answer.

---

## C. `RoqSong::impulsive` — RESOLVED 2026-09-04

Moved into a clearly-marked legacy section at the end of `RoqSong`, with the reason it
cannot simply be deleted written next to it and at the write site.

**The obvious fix would have been a bug.** The original plan said "stop writing it". `.roq`
is a *positional* line format — `readFloat` consumes one line at a time in a fixed order —
so dropping the line shifts every field after it by one, and a reader would parse
`accentSquelch` as `impulsive` and mis-read the whole rest of the header. **The constant `0`
is load-bearing padding and has to keep being written.** The change is documentation only.

**Surveyed first, as Dengo asked.** All 25 shipped modules: two are v1 and predate the field
(`Crazy wow.roq`, `bubblebath.roq`), the other 23 are v2 or v3 and every one stores `0`. It
has never carried a non-zero value, so nothing is lost by ignoring it — which is exactly why
it can be documented as inert rather than having to be interpreted.

**Verified:** `test_roq` still round-trips every version and all 25 modules still load.

---

## F. Mnemonics that could never fire — RESOLVED 2026-09-04

`Alt+A`, `Alt+S` and `Alt+R` now reach the `&Accent` slider, the `Re&sonance` slider and the
`&Run` button, which is what their underlines always claimed.

### It needed both halves, and the second was the hidden one

Moving the accelerators off those chords was the obvious half. But **two of the three chords
were claimed by a second label as well**, and a duplicated mnemonic does not activate
anything either — Win32 cycles focus between the matches. So the duplicates had to go too:

| Chord | Was | Now |
|---|---|---|
| `Alt+A` | `IDM_STEP_TOGGLE_ACCENT`, plus `P&attern:` and `&Accent (this step)` | the `&Accent` slider, alone |
| `Alt+S` | `IDM_STEP_TOGGLE_SLIDE`, plus the `&Step sequencer` heading | the `Re&sonance` slider, alone |
| `Alt+R` | `IDM_EDIT_RANDOM_STEP` | the `&Run (F5)` button |

The step accelerators moved to one family: **`Ctrl+Shift+A` / `S` / `I` toggle accent, slide
and reverse on the focused step, `Ctrl+Shift+G` randomises it.** `Alt+I` (reverse) collided
with nothing, but it moved anyway — three sibling actions on two different modifiers is
worse to learn than one rule, and freeing `Alt+I` turned out to be needed below.

The controls that lost an underline — the pattern combo, the three per-step checkboxes, the
step-sequencer heading — have none now rather than a false one. All are Tab-reachable, the
checkboxes have the `Ctrl+Shift` chords from anywhere, and `Ctrl+Left/Right` changes pattern
without focusing the combo at all.

### Two more the new check found, which were not in this item

Writing the check first (`tools/check_keyboard.py`) turned up two live faults nobody had
listed:

- **`Alt+E` never reached the EnvMod slider.** The menu bar is consulted *before* any
  control, so it only ever opened the **Edit** menu. Nothing in "EnvMod" was free — until
  Wildness gave up `D` by moving to `W&ildness` (`Alt+I`, freed above). EnvMod is now
  `EnvMo&d`. A chain of three moves to fix one silent failure, which is what "the window is
  out of letters" actually costs.
- **`Alt+P` was on two menus**, `&Pattern` and `&Play`, so it cycled between them instead of
  opening either. `Play` is now `P&lay` (`Alt+L`).

And one benign ambiguity worth removing while there: **every parameter slider had its
mnemonic on two controls**, because the trackbar carried the same `&`-bearing text as its
label. Both matches happened to land on the same trackbar, so it worked — but "harmless
duplicate mnemonic" is precisely the shape of bug this window has been bitten by twice. The
trackbars now carry the name without the ampersand, which keeps the accessible name and
removes the ambiguity.

### The check is the real deliverable

`tools/check_keyboard.py` fails on all four ways a key can silently do nothing here, each
of which has shipped at least once:

1. a chord bound twice (first entry wins — `Ctrl+Shift+N` was);
2. an unmodified key bound as an accelerator (dead on every control — `Space` was);
3. two controls sharing a mnemonic (cycles focus — `Alt+V` did);
4. a mnemonic shadowed by an accelerator or by the menu bar (never fires — this item).

**No plain `Alt+letter` is bound as an accelerator any more**, and the check enforces it.
That is what makes class 4 unrepeatable rather than merely fixed.

**Verified:** 32 accelerator chords, no duplicates, no unmodified bindings, no plain
`Alt+letter`. 18 main-window mnemonics, all unique, none shadowed, none on a menu-bar letter.
Free letters left: **J, K, Z**.

---

## G. Distortion block — RESOLVED 2026-09-04

`src/Distortion.{h,cpp}`, a sibling module beside the untouched stock `dist.cpp`. Five
models — **Classic / Rat / Screamer / Dist+ / Fuzz** — with per-block coefficients
(`cbDistPrepare`) and a per-sample path (`cbDistProcess`): optional highpass, optional
pre-lowpass standing in for the RAT's LM308 slew limit, drive, per-type clip, post-lowpass
tone, level, NaN guard, clamp. `Ctrl+D` opens the dialog; the button carries no mnemonic
because the main window has no free letter (finding F).

**Type 0 (Classic) calls the stock `dist_dist()` verbatim.**

**Completed since:**

1. **`.roq` v9** — `distType`, `distGain`, `distTone`, `distLevel` appended after
   `resoBoost`, guarded `if (version >= 9)`, defaulting to Classic / 0 / 0.5 / 1.0.
   **v9 is distortion only.** B's variable-length patterns are a *later* bump, not a second
   definition of v9 — keeping them apart means neither has to wait for the other, and one
   version number never means two things.
2. **`resetTo303()`** returns type to Classic and gain/tone/level to neutral. Per-step
   reverse is still deliberately untouched — that is pattern data.
3. **F1 reference** lists `Ctrl+D`, and the `Alt+I` line now describes reverse step as an
   envelope that runs backwards rather than "closes then opens" (see J).
4. **Docs** — README has a "Distortion dialog" section with the model table and the level
   figures below.

**Verified** (`analyze_g.py`, one render per model rather than one combined pass):

1. **Classic really is inert.** `dist0` — Classic with gain 0.6, tone 0.5, level 1.0 —
   renders **byte-identical** to a plain default render. The three knobs do nothing while
   Classic is selected, proven rather than asserted. All 10 unchanged golden modes also stay
   byte-identical with `Distortion.cpp` linked in.
2. **Every model changes the sound, and they are distinct.** At gain 0.6, difference from
   Classic as a fraction of signal RMS: Rat 368 %, Screamer 134 %, Dist+ 361 %, Fuzz 438 %;
   and all six pairs of models differ from each other.
3. **Bounded at maximum drive.** 20 s renders at gain 1.0 for all five types: zero
   non-finite samples, peak ≤ 0.85, all audible.
4. **Format.** `.roq` v9 round-trips all four fields; every older module (v1/v2/v5/v6/v7)
   loads with the block at Classic/neutral; the file declares version 9.

**Levels, measured, because this is the thing that would surprise someone:**

| | RMS at gain 0 (the default) | vs Classic |
|---|---|---|
| Classic | 0.073 | — |
| Rat | 0.068 | 0.9× |
| Screamer | 0.025 | **0.3×** |
| Dist+ | 0.072 | 1.0× |
| Fuzz | 0.123 | 1.7× |

So **selecting a model is not a volume jump** — the big numbers in test 2 come from
deliberately winding the gain up, where hard clipping stops output depending on input and
the level lands ~6× higher. That is what the level knob is for.

**Worth a listen, not a bug:** Screamer at 0.3× is the quiet one. Its high-pass keeps bass
out of the clipper at 720 Hz, which is authentic Tube Screamer behaviour and is *why* it
makes a line cut — but on a bassline that is most of the energy, and it may read as "I
picked Screamer and it went quiet". Lowering that corner to ~250 Hz would make it a
bass-friendly variant rather than a faithful one. Dengo's call after hearing it.

**Also corrected while here:** the README and `CLAUDE.md` both claimed `.roq` was
"forward-tolerant in both directions". It is not, and never was. A build rejects a file
whose version exceeds its own (`unsupported .roq version`) rather than mis-parsing it —
correct behaviour for a positional format, but the docs overstated it. Only the *step line*
is tolerant both ways.

---

## H. Acid FX block — RESOLVED 2026-09-04

The distortion dialog is now **Acid FX** (`Alt+X`, still `Ctrl+D`), holding three effects in
series after the filter and the volume envelope: **distortion → compressor → ring
modulator**. New sources `src/AcidFx.{h,cpp}`; `.roq` **v11** carries the four parameters.

The rename paid for itself: "Distortion" had no free mnemonic letter left, so that button was
Tab-only. "Acid FX" freed `X`.

### H1. Compressor

Threshold and depth, exactly as specified. Depth 1.0 is a 3.3:1 ratio and no further — glue,
not a limiter. Auto make-up keeps it from reading as a volume knob: the curve pivots at full
scale, so quiet material comes up and the loudest material stays put.

**The follower has no attack time, and that turned out to be load-bearing rather than a
simplification.** The first version used a 5 ms attack, on the reasoning that letting the
transient through is what stops a compressor eating the accents it is meant to even out. It
also lets an individual sample sit *above* the envelope that graded it, and the make-up gain
then pushes it past full scale: **0.19 % of samples clipped at an ordinary setting**
(threshold 0.3, depth 0.8). Measured, not theorised — the bounds test caught it.

With a peak follower (`env = max(|x|, released)`) the bound becomes provable instead of
measured. Given `env >= |x|` and a curve that pivots at 1.0, an input inside ±1 cannot leave
the stage outside ±1, for any threshold and any depth. The test now asserts **zero** clipped
samples rather than `|x| <= 1`, which is a much sharper instrument.

The gain law is written in the linear domain (`g = 1 + kk*(thresh/e - 1)`) rather than as the
usual `pow(thresh/e, k)`. One divide per sample instead of a transcendental, and the make-up
gain is bounded by construction instead of running away as the threshold drops.

### H2. Ring modulator

Post-filter, in the FX block, carrier independent of pitch — the version Dengo picked.
Frequency 5 Hz–2 kHz on an exponential taper, amount from dry to pure ring. Hz is stored, not
the slider position, so the taper can be retuned later without repitching saved songs.

**The carrier restarts on every new note, at its peak.** Three reasons that happen to agree:

1. A free-running audio-rate carrier drifts against the bar, so a one-pattern loop stops
   repeating identically — which is this block's own admission criterion.
2. Restarting gives every note the same timbre instead of one that lands differently each
   time.
3. **cos**, not sin, so a freshly zeroed phase starts at the carrier's peak. Starting at a
   zero crossing would mute the attack transient at high amounts.

Note this is the **opposite** of the `pwmLfoPhase_` rule, and the difference is the rate: a
slow LFO's drift across the bar is the feature; an audio-rate carrier's drift is not heard as
movement at all.

### The one that had two defensible answers

**Whether to reset the compressor's follower at pattern start.** `resetFilters()` runs there,
and it is the obvious place. Resetting makes the loop bit-identical, which satisfies the
block's criterion literally — at the price of a burst of make-up gain on the first few
milliseconds of beat 1 of *every* bar.

It is not needed. A one-pole is **contracting**: the difference between two runs with
different initial states shrinks by the smoothing coefficient every sample, so it is gone
long before a loop is over. Measured at exactly **0.0** between loops 2 and 3. A phase
accumulator never forgets, which is why the ring carrier needs an explicit reset and this
does not. Both are now invariants in `CLAUDE.md`, side by side, because they look like the
same situation and are not.

### `.roq` v11 and a generalised versioning rule

v11 appends four floats. It is also the first *appended* block to use the content-derived
version rule B introduced for a non-append change, because the rule turned out to be worth
having generally: **a song that never opened the FX dialog still writes v9 or v10** and still
opens in an older build.

The trigger is "some field differs from its default", not "the effect would be audible". A
carrier frequency dialled in with the amount left at zero is still something the user set,
and dropping it to keep a version number down would be the wrong trade. `neededVersion` is
now the single place that decides, and the writer's field blocks mirror the reader's guards
exactly — a file stamped 9 carrying v11's lines would have them read as the pattern count.

### Verified

- **Each effect has its own off-path proof.** `compoff` moves the threshold with depth at 0;
  `ringoff` moves the carrier frequency with amount at 0. Each is **bit-identical** to the
  same patch on the pre-change engine. A single all-off render would not have shown that the
  *other* control of each pair is inert.
- **Plain `off` is still bit-identical to the pre-round-2 engine**, so the whole stack with
  everything switched off remains the 2004 sound.
- **The compressor does what it says.** Accents on steps 0/4/8/12: the accented-to-plain peak
  ratio falls from 3.21 to 2.17 (68 % of dry) — evened out, but the accents still stand out,
  which is the difference between glue and a limiter. RMS goes *up* 1.24×, so it is not
  simply quieter.
- **The ring modulator produces real sidebands.** At amount 1.0 and 300 Hz the 65 Hz
  fundamental drops to 1.6 % of its energy and a sideband appears at 365 Hz at 6×. That is
  ring modulation, not a tremolo.
- **Neither breaks the loop.** Loop 2 vs loop 3: ring 0.0, compressor 0.0. Negative control:
  the free-running PWM LFO differs by 8.2e−01 on the same measure, so the test is sensitive
  to what it claims to test.
- **Bounded, with headroom.** `compmax`, `ringmax` and everything-at-once into Fuzz at full
  gain: finite, audible, and **zero samples reaching the output clamp** — peaks 0.83, 0.78
  and 0.68.
- **Format.** All four fields round-trip; a carrier-only song still declares 11 and keeps its
  frequency; an untouched song still declares 9 and loads at defaults; a 21-step pattern and
  the FX block round-trip together.
- Everything from rounds 1–6 still passes, including `test_a alloc` — the new per-sample code
  allocates nothing. Release builds with no warnings; the app starts and closes cleanly.

**Recorded, not chosen:** the two ring-modulator homes that did not win are still worth
having on paper. Pre-filter, in the oscillator section, the sidebands would run through the
filter and envelope like every other source, so a cutoff sweep would shape the metallic
content — better behaved, more like part of the instrument. Ringing the main oscillator
against the duplet would reuse the duplet's detune as a pitch-tracking carrier interval and
its level as the balance, needing no new oscillator at all — the most musical of the three,
at the cost of the duplet no longer being available as a plain detuned layer.

---

## I. Transport reset raced the audio thread — RESOLVED 2026-09-04

`setRunning()` now writes two atomics and nothing else. The DSP work moved into
`applyTransportReset()`, which `process()` calls at the top of a block after an
`exchange()` on `transportReset_` — so the request is applied exactly once, on the thread
that owns the state, and a press arriving mid-block is kept for the next one rather than
lost. No lock and no allocation, so invariant 3 still holds. The reset also now lands on a
block boundary instead of mid-buffer, which is better behaviour than what it replaced rather
than merely safer.

### Two more races, one of them found by the test

- **`currentStepIndex()` returned `currentStep_` directly**, a non-atomic int the audio
  thread writes while the UI polls it for the step display. Making `currentStep_` atomic
  would have put an atomic load in the per-sample hot path to serve a UI that repaints at
  30 Hz. The audio thread publishes `displayStep_` once per **step** instead — one relaxed
  store every few thousand samples.
- **`setParam` wrote `boxSndType_`** from whichever thread moved the Waveform control. Found
  by the ownership check below, not by reading. It was also *dead*: `process()` derives the
  same value from the Waveform atomic at the top of every block, before anything reads it.
  Deleted.

### The acceptance test had to change

This item asked for a ThreadSanitizer run. **MSVC ships AddressSanitizer but not
ThreadSanitizer, and ASan does not detect data races**, so that test cannot be run as
written on this toolchain. Two things stand in for it, and the pair is arguably stronger:

1. **`tools/check_dsp_ownership.py`** — a static check that every non-atomic member is
   assigned only from `process`, `triggerStep`, `resetFilters`, `applyTransportReset` or
   construction. Rather than observing that no race happened during one run, it checks the
   property that makes a race impossible. This is what found `boxSndType_`.
2. **`test_a stress`** — now runs a third thread hammering the transport while the writer
   thread rewrites the song and the render thread renders. Measured 60 000 render blocks
   against 13 954 song commits and 14.4 million transport changes: no non-finite samples, no
   samples outside ±1.

**Verified:** all 11 golden modes remain bit-identical, so none of this changed the sound.

---

## J. Reverse step — RESOLVED 2026-09-04

**Reverse step now reverses the filter envelope in time.** It was a sign flip on EnvMod,
which inverts the envelope but does not reverse it: an upside-down decay moves fastest at
the note's *start*, where a reversed note should be slow first and peak at the end. Dengo
heard the difference; the numbers below confirm it.

**What changed** (`src/CBEngine.cpp`):

- `boxDecay_` starts at `4.0 * pow(decayMul, N)` for a reversed step and multiplies by
  `1.0 / decayMul` per sample, climbing back to 4.0 as the note ends. `4·r^(N−t) ==
  (4·r^N)·(1/r)^t`, so it is the forward curve read backwards — one different starting
  value and one different multiplier. **Decay length becomes attack length.**
- `decayMul` is hoisted out of the sample loop (it was computed inline per sample from the
  same block constant, so the value is unchanged), and `decayMulInv` beside it.
- `audibleNoteLength()` walks the slide chain forward to size `N` as the note's *audible*
  length — its own step plus every tied step, less the tail after the gate closes, using
  the **last** step of the tie for the accent-dependent threshold.
- The direction is latched inside `triggerStep()`'s `!bNoteSlide_` branch, so a tie is one
  envelope and a slid reversed note reverses across the whole note.
- A `> 4.0` clamp bounds the growth, and the seed is floored at `1e-30` so an underflowed
  `r^N` cannot strand the envelope at zero.
- **`stepReverse_` no longer appears in the cutoff expression at all** — that line is back
  to its pre-reverse-step form verbatim, which is the cleanest possible off-path.

**Verified:**

1. **Off-path bit-identical.** All 11 harness modes rendered against `golden_preA/`:
   **10 of 11 byte-for-byte identical**, and the only one that differs is `invert`, the one
   mode that sets `reverseStep`. As a free extra, that also proves the distortion module's
   Classic path is bit-identical, since `Distortion.cpp` is now linked into the harness.
2. **It is a reversal, not an inversion — the decisive test.** A dedicated `revshape` probe
   (every step identical but the one under test, high note so a 6 ms window holds many
   cycles) measured brightness as `rms(first difference) / rms(signal)` across the audible
   window, averaged over three loop repetitions, against a pre-J engine reconstructed by
   reverse-applying the patch:

   | | slice 1 → 8 | peak | rise done by half-way | mirror error |
   |---|---|---|---|---|
   | control (normal) | 0.386 → 0.163 | slice 1 | — | — |
   | old (sign flip) | 0.079 → 0.236 | slice 7 | **62 %** | 67 % |
   | new (time-reversed) | 0.163 → **0.383** | slice 8 | **35 %** | **11 %** |

   *Mirror error* is the strongest of these: a time reversal means slice `k` of the reversed
   step should equal slice `N−1−k` of the normal step. The new curve matches its own control
   read backwards to within **11 %** of the control's range; the sign flip scores **67 %**.
   Note also the endpoints — the reversed step ends at 0.383 where the normal one *starts*
   at 0.386, and begins at 0.163 where the normal one *ends* at 0.163.

   The old engine's 62 % front-loading matches the analytic prediction for
   `E·(2 − 4·r^t)` exactly, which is what says the measurement is faithful rather than lucky.
3. **Bounded.** 20 s renders with reverse on a slid step (`revtie` — step 3 slides into an
   accented step 4, exercising the tie walk and the accented gate threshold) and with all
   sixteen steps reversed (`revall`): zero non-finite samples, peak 0.789, and still
   audible. A backwards envelope multiplies *up* every sample, so runaway was the one real
   risk.
4. **Nothing else moved.** `test_a alloc` still reports **0 allocations** in `process()`;
   `test_a stress` passes 60 000 blocks against 10 632 concurrent commits; all 15 `.roq`
   checks pass, including v1/v2/v5/v6/v7 files loading with reverse off.

**Docs updated:** `README.md` gained "The envelope — and why there are four of them" (the
explanation Dengo asked for) and a rewritten reverse-step section; `CLAUDE.md` invariant 7
now describes the time reversal, with the sign flip recorded as the wrong-looking-right
alternative and the mirror test as the thing that tells them apart.

**One behaviour change worth a listen:** the direction latch moved inside the `!bNoteSlide_`
branch, so a tie now takes its direction from the step that started it. Previously every
tied step re-latched. This follows from the redesign — a reversed tie is one backwards
envelope — but it is a change to how long notes behave, so it is worth hearing before it is
taken as settled.

---

## K. Triplet feel — RESOLVED 2026-09-04

A **Triplet feel** checkbox beside Pattern length, per pattern, plus `Ctrl+T` and a Pattern
menu item. `.roq` **v12** carries it.

**What it does:** off, a step is a sixteenth. On, a step is an **eighth-note triplet** — a
quarter divided by three, 4/3 longer — so twelve steps fill exactly one 4/4 bar at the
*same tempo*.

| | 12 steps | 16 steps | 24 steps |
|---|---|---|---|
| straight | ¾ bar (the polyrhythm) | 1 bar | 1½ bars |
| triplet | **1 bar** | 1⅓ bars | **2 bars** |

**The number it deletes is 105.** Getting twelve steps to fill a bar at 140 BPM used to mean
typing 105 into the tempo box — a figure with nothing to do with the music, out of step with
every other device, recomputed by hand every time the real tempo moved. That is exactly what
Dengo meant by "you'd not have to work out the tempo for it", and it is the whole feature.

**Which triplet: the ×4/3 eighth**, chosen because it makes *12* — the number named first —
fill one bar, and twelve eighth-triplets over a 4/4 bar is the loping 12/8 shuffle that
"groovy 3/4 beat" describes. The ×2/3 sixteenth triplet is the more literal reading and would
make *24* the one-bar number instead, with 12 giving half a bar of faster rolling triplets.
**If the ear says otherwise it is one constant** — `tempo * 3.0` becomes `tempo * 6.0` in
`stepSamples()`, and `analyze_k.py`'s expected periods halve. Recorded here so the change is
a two-line one and not an archaeology exercise.

Rejected: making the ratio depend on the pattern length so both 12 and 24 fill exactly one
bar. It would be magic — one checkbox meaning two things — and a 13-step pattern would have
no defensible answer.

### The trap, and it is a real one

`samplesPerStep_` is computed **once per block**, but the step duration is now a property of
the **pattern**, and the sequencer changes pattern *inside* the sample loop. Recomputing only
at the top of the block would run a triplet pattern following a straight one at the wrong
rate for up to a whole buffer: **a flam on beat 1, once per loop, that moves with the audio
buffer size** — the kind of bug that gets blamed on the sound card. `stepSamples()` is
therefore called in both places.

The test for it renders the same song at 32, 64, 512 and 1024 frames per block and requires
identical onsets. That mattered: the bug is invisible at any single block size, so a harness
with one hard-coded block would have passed a broken build. The harness gained a block-size
argument for this.

### `.roq` v12 costs nothing

The flag is a **second token on the pattern's `lastStep` line**, not a line of its own.
`readInt` parses with `std::stoi`, which reads the leading integer and stops — so a pre-v12
build reads `"11 1"` as `lastStep` 11 and never sees the flag. Same trick as the per-step
reverse column, and unlike v10 it loses no compatibility whatever. Still stamped v12 when
used, so the file is honest.

### Verified

- **Off is bit-identical.** `straight12` renders byte-for-byte the same on the pre-change
  engine. The straight branch is the original expression, not a multiply by 1.0 — house rule,
  and it makes the proof exact rather than a rounding coincidence.
- **The ratio is right, measured in samples against the bar.** 12 triplet steps = **75 600
  samples**, exactly what 16 straight steps take, exactly one bar at 140 BPM. 24 triplet
  steps = 151 200 = two bars, which is what pins it to the *eighth* triplet: a sixteenth
  triplet would have given 75 600 there. 12 straight steps = 56 700 = ¾ bar, the polyrhythm,
  measured for contrast.
- **The pattern boundary is clean.** A straight 16 followed by a triplet 12: every gap is
  75 600 samples, and the onsets are identical at all four block sizes.
- Finite, inside ±1, audible at every length. Everything from rounds 1–7 still passes,
  including `test_a alloc`. Release builds with no warnings.

### Two things the new keyboard check found on the way

Adding a menu item meant teaching `tools/check_keyboard.py` about **menu-internal
mnemonics**, which it had never covered — a popup is the same namespace problem one level
down. It immediately found three live collisions, all pre-existing, all silent:

- **Pattern menu:** `&Next` against `&New pattern` → `Ne&w pattern`.
- **File menu:** `Start &new track...` against `&New random pattern` → `Start new &track...`.
- **Edit menu:** `&Random pattern` against `T&ranspose current pattern` → `Tra&nspose`.

Each cycled focus between two items instead of activating either. The check now covers five
failure classes rather than four.

---

## Needs a human — cannot be checked headlessly

Every engine claim in rounds 1–3 is verified numerically. None of it has been verified by
ear or with an actual screen reader.

- **Copy / Cut / Paste pattern — implemented 2026-09-05, reported dead by Dengo.** They had
  never been implemented: menu items, accelerators and an F1 entry all existed, and no `case`
  in the WM_COMMAND switch handled any of the three, so `DefWindowProc` swallowed them in
  silence. Now `Ctrl+C` / `Ctrl+X` / `Ctrl+V`, carrying steps *plus* length *plus* triplet
  flag, with Cut clearing in place rather than removing the slot (sweeps address patterns by
  index, so removing one would relocate every sweep after it). `tools/check_keyboard.py`
  gained a sixth check for this class — the other five all ask whether a keystroke arrives,
  and none could see that nothing was listening. **Worth confirming by hand** that paste into
  a pattern of a different length announces sensibly.
- **The per-step toggles never lost their keys, but they had become undiscoverable.** They
  moved to `Ctrl+Shift+A` / `S` / `I` in F and the checkbox labels said nothing about it, so
  a screen reader announced no key at all. The labels now name the chord, the way
  `Triplet feel (Ctrl+T)` already did. **Open question for Dengo:** whether three-key chords
  are right for the two most-used controls in the program. Two keys would mean taking `Alt+A`
  and `Alt+S` back from the Accent and Resonance sliders and binding them as accelerators,
  which re-opens the shadowing class that F closed — see the trade-off in `README.md`.

- **Listening — done 2026-09-04.** The resonance boost **works**, and the PWM LFO
  **works**; both are confirmed by ear, not just by numbers. **Reverse step was not what was
  wanted** — rebuilt as a time reversal, see J. The 303 envelope now has a written
  explanation in `README.md` ("The envelope — and why there are four of them").
- **Reverse step, again.** J is verified numerically but not by ear. Worth checking both
  that a reversed step now sounds reversed, and that reversed *slid* notes behave — the
  direction latch moved, so a tie now reverses as one note.
- **Listening, still outstanding:** the noise source, and the five distortion models from
  G — particularly whether Screamer's 0.3× level reads as character or as a fault.
- **Pattern length (B).** Whether odd lengths are musically useful in this instrument — 15
  against 16 walks the accents around the bar, a short pattern under a long sweep never
  repeats the same way twice. Also whether the pattern boundary is clean at short lengths:
  the filter carries state across it, which is audible as a ring rather than a click in the
  renders, but a 2-step pattern crosses that boundary ten times a second.
- **Triplet feel (K).** Built with the ×4/3 eighth triplet, so 12 steps fill one bar. The
  question is whether that is the division you actually reach for, or whether the ×2/3
  sixteenth (24 steps = one bar, faster and busier) is the more useful default. Now that it
  exists, comparing 12-triplet against 12-straight against a 4/4 reference should settle it
  in about a minute. Changing it is one constant.
- **The keyboard, after F.** Every chord in the table moved or was checked, so this is the
  round most likely to have broken muscle memory rather than code. Worth confirming that
  `Alt+A`, `Alt+S` and `Alt+R` now land on the Accent slider, the Resonance slider and the
  Run button; that `Ctrl+Shift+A` / `S` / `I` / `G` reach the focused step from anywhere;
  and that `EnvMo&d` and `W&ildness` announce sensibly on their new letters.
- **The acid FX block (H).** Three questions the numbers cannot answer. Whether the
  compressor at a middling depth actually reads as *glue* rather than as "everything got
  louder" — the auto make-up is doing real work and the RMS goes up 1.24×. Whether the ring
  modulator's per-note carrier restart sounds locked and deliberate or mechanical, since the
  alternative (free-running) was rejected on loop-identity grounds rather than by ear.
  And whether the 5 Hz end of the carrier range is useful as a per-note tremolo or just
  lands as a fault.
- **Screen reader.** That every control announces its name and value, and that the four
  mnemonics moved off their first letter (`Te&mpo`, `T&uning`, `Deca&y`, `Wil&dness`)
  announce sensibly rather than just uniquely. Since B, also whether a 64-entry length combo
  is pleasant to arrow through, or whether it wants an edit box or a coarser list — it is
  the first control here whose *number of choices* is the usability question.
- **Activation — confirmed.** Space, Enter and the mnemonic each toggle every checkbox.
- **Transport — confirmed.** F5/F6/F7/F8 do what their labels say, including F5 restarting
  mid-playback and F8 being harmless when already stopped.
- **Layout — working so far** (Dengo). The main window and both dialogs, including the
  oscillator dialog after two rounds of growth (252 → 314 dialog units). Worth re-checking
  once G's distortion dialog has settled, and again for B's length combo, which is the first
  thing placed to the right of the step editor block.

---

## Standing rules

In `CLAUDE.md`, repeated here because they are what breaks silently:

- `resetFilters()` must not touch `pwmLfoPhase_`.
- `wwelti_IIR_IIRlp3pole` stays byte-for-byte; new filter behaviour goes in a sibling.
- No allocation and no locking on the audio thread — A fixed this 2026-09-03; anything
  added to `process()` (G, H) has to keep it true. `test_a alloc` is the enforcement.
- A new feature's off-state must be bit-identical to the engine before it.
- Never bind a plain unmodified key as a global accelerator, and never reuse a mnemonic
  letter. Both fail identically and silently: "the key does nothing."
