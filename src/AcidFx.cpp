#include "AcidFx.h"

#include <cmath>

namespace {

constexpr double kTwoPi = 6.28318530717958647692;

// Release only. There is no attack time, and that is a design decision rather
// than a simplification: the follower rises INSTANTLY to |x| and only falls
// slowly. A smoothed attack lets an individual sample sit above the envelope
// that graded it, and the make-up gain below then pushes it past full scale —
// measured at 0.19 % of samples clamped at an ordinary setting, which is
// clipping, not glue. With a peak follower the output bound becomes provable
// (see the make-up comment), and the character that a slow attack would have
// bought is not worth an audible artifact at the setting people will actually
// use.
//
// 120 ms release is long enough not to pump on individual cycles of a bass note
// (a 55 Hz note is 18 ms per cycle) and short enough to recover between 16ths
// at any usable tempo.
constexpr double kReleaseSec = 0.120;

// Depth 1.0 maps to this much compression, not to a hard limit. 0.7 in the
// linear-domain formulation below is a 3.3:1 ratio, which is glue. A depth
// control that reached 1.0 would be a limiter and would flatten the accent
// dynamics that give the instrument its groove.
constexpr double kMaxK = 0.7;

// One-pole smoothing coefficient for a time constant in seconds.
double onePoleTime(double seconds, double sampleRate) {
    if (seconds <= 0.0) return 0.0;
    const double a = std::exp(-1.0 / (seconds * sampleRate));
    return (a < 0.0) ? 0.0 : (a > 0.99999 ? 0.99999 : a);
}

} // namespace

void cbCompPrepare(CbCompCoeffs& c, double threshold, double depth,
                   double sampleRate) {
    if (depth <= 0.0) { c = CbCompCoeffs{}; return; }   // off; caller skips
    if (sampleRate < 1000.0) sampleRate = 44100.0;
    if (threshold < 0.0) threshold = 0.0;
    if (threshold > 1.0) threshold = 1.0;
    if (depth > 1.0) depth = 1.0;

    c.on = true;
    // Never all the way to zero: at very low thresholds the make-up gain below
    // grows without bound and the quiet between notes gets pulled up into a
    // swell, which is a different effect from the one on the label.
    c.thresh = 0.05 + 0.95 * threshold;
    c.kk     = kMaxK * depth;

    // Gain law, applied to the follower's output `e` when e > thresh:
    //
    //     g = 1 + kk * (thresh/e - 1)      so      e*g = e*(1-kk) + kk*thresh
    //
    // i.e. a straight line in the LINEAR domain with slope (1 - kk), which is a
    // hard-knee compressor of ratio 1/(1-kk). Written this way rather than as
    // the usual pow(thresh/e, k) for two reasons: it costs one divide per
    // sample instead of a transcendental, and its make-up gain is bounded by
    // construction (below) instead of running away as the threshold drops.
    //
    // Auto make-up restores unity at full scale, so the transfer curve pivots
    // at 1.0. Without it the depth control would read as a volume knob.
    //
    // The pivot is also what BOUNDS the output, given a peak follower. Writing
    // e for the follower and knowing e >= |x|:
    //
    //   e >  thresh:  |x*g| <= e*g = makeup*(e*(1-kk) + kk*thresh), which rises
    //                 with e and equals exactly 1 at e = 1;
    //   e <= thresh:  |x*g| <= thresh*makeup <= 1 whenever thresh <= 1.
    //
    // So an input inside +-1 cannot leave this stage outside +-1, for any
    // threshold and any depth. That is a property of the code, not an
    // observation about one render.
    const double atFull = (1.0 - c.kk) + c.kk * c.thresh;
    c.makeup = (atFull > 1e-6) ? (1.0 / atFull) : 1.0;

    c.relA = onePoleTime(kReleaseSec, sampleRate);
}

double cbCompProcess(const CbCompCoeffs& c, CbCompState& s, double x) {
    const double d = std::fabs(x);
    // Peak follower: instant up, slow down. The max() is what guarantees
    // env >= |x|, and that is what makes the output bound above hold.
    const double released = d + c.relA * (s.env - d);
    s.env = (d > released) ? d : released;

    double g = c.makeup;
    if (s.env > c.thresh) g *= 1.0 + c.kk * (c.thresh / s.env - 1.0);

    const double y = x * g;
    // Cannot overshoot by construction (above), so this guard is only about the
    // state: a denormal or a bad sample rate must not be able to poison the
    // follower permanently, the same reasoning as the resonant filter's NaN
    // guard.
    if (!(y == y)) { s.reset(); return 0.0; }
    return y;
}

void cbRingPrepare(CbRingCoeffs& c, double hz, double amount, double sampleRate) {
    if (amount <= 0.0) { c = CbRingCoeffs{}; return; }  // off; caller skips
    if (sampleRate < 1000.0) sampleRate = 44100.0;
    if (hz < 0.0) hz = 0.0;
    if (hz > sampleRate * 0.45) hz = sampleRate * 0.45;
    if (amount > 1.0) amount = 1.0;

    c.on       = true;
    c.amount   = amount;
    c.phaseInc = hz / sampleRate;
}

double cbRingProcess(const CbRingCoeffs& c, double& phase, double x) {
    phase += c.phaseInc;
    if (phase >= 1.0) phase -= 1.0;

    // Cosine, not sine, so a note whose phase has just been re-zeroed starts at
    // the carrier's PEAK. Starting at a zero crossing would mute the attack
    // transient at high amounts, which is the one part of a 303 note nobody
    // wants softened.
    const double carrier = std::cos(kTwoPi * phase);

    // Dry/ring blend rather than a bare multiply: amount 0 is the untouched
    // signal, amount 1 is pure ring, and everything between keeps the
    // fundamental audible under the sidebands.
    return x * (1.0 - c.amount + c.amount * carrier);
}
