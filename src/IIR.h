//
// IIR
//

#ifndef _IIR_H_
#define _IIR_H_

//typedef long double tbuffer[8000];

struct tbiquad
{
	long double lp, lp_1, lp_2, lp_3, lp_4, lp_5;
};

struct tbiquadFIR
{
	long double lp;
};

long double	IIR_FIRlp(tbiquadFIR & b, long double  eCutoff,long double cur);
long double	IIR_IIRlp3pole(tbiquad & b, long double eCutoff, long double eRezo, long double cur, long double freq );
long double	IIR_doIIRCycle2(tbiquad & b, long double eCutoff, long double eRezo, long double cur);
long double	IIR_doIIRhigh(tbiquad & b, long double eCutoff, long double cur);
void	IIR_resetIIR(tbiquad & b);
void	IIR_resetFIR(tbiquadFIR & b);

#endif _IIR_H_



// Quick hack by wwelti following: Function table(s). First table is required for that weird sin(sin(sin(sin( .... stuff.

// Argh. Why's cyb using long doubles everywhere? They're extraordinarily slow (when accessed by memory. And he's doing this all the time).
// Doubles are definitely good enuff here. i believe he even wouldn't hear the difference if those were only floats.

void initFunTab1();
long double wwelti_IIR_IIRlp3pole(tbiquad & b, long double eCutoff, long double eRezo, long double cur, long double freq);

// Devilfish-style boosted-resonance variant. Identical filter topology to
// wwelti_IIR_IIRlp3pole, but the eRezo ceiling is lifted past the point of
// stability so the filter can self-oscillate, and the resonant integrator is
// soft-bounded so that oscillation settles into a limit cycle instead of
// growing without limit. The stock function above is deliberately left
// untouched so the normal signal path cannot regress.
long double wwelti_IIR_IIRlp3pole_boost(tbiquad & b, long double eCutoff, long double eRezo, long double cur, long double freq);
