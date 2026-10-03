//
// IIR.cpp
//

#include "IIR.h"
#include "core.h"
#include  <math.h>

long double hp;
long double hp_1;
long double hp_2;

long double hp1;
long double hp1_1;
long double hp1_2;

long double lp;
long double lp_1;
long double lp_2;
long double lp_3;
long double lp_4;
long double lp_5;

//tbuffer buffer;
int bufln;



void IIR_resetIIR(tbiquad & b)
{
  b.lp = 0.f;
  b.lp_1 = 0.f;
  b.lp_2 = 0.f;
  b.lp_3 = 0.f;
  b.lp_4 = 0.f;
  b.lp_5 = 0.f;
}

void IIR_resetFIR(tbiquadFIR & b)
{
	b.lp = 0.f;
}

long double IIR_FIRlp(tbiquadFIR & b, long double eCutoff, long double cur)
{
  b.lp = b.lp + (cur-b.lp)*eCutoff;
  return b.lp;
}


long double IIR_IIRlp3pole(tbiquad & b, long double eCutoff, long double eRezo, long double cur, long double freq)
{
	long double result;
//	long double frqdiff;

	if (eCutoff<0.01f) eCutoff = 0.01f;
	if (eCutoff>0.7f) eCutoff = 0.7f;
	if (eRezo<0.f) eRezo = 0.f;
	if (eRezo>1.f) eRezo = 1.f;

	//Bestem min/max resonens og bestem slide!
	freq = freq/44100.f;

	eRezo = (eRezo*(0.965f-(freq*0.975f) )) + sin( sin( sin( sin(eCutoff*(pi*0.5f)) *(pi*0.5f)) *(pi*0.5f)) *(pi*0.5f)) *( (0.023f) + (freq*0.42f) );

	eRezo = eRezo*1.01f;

	if (eCutoff<0.05f)
	{
		eRezo = eRezo* ((eCutoff/(0.05f))*0.1f+0.9f);
	}

	eRezo = 1.f-eRezo;

	b.lp_2 = cur-b.lp_1*eRezo-b.lp;
	b.lp_1 = b.lp_1 + b.lp_2*(eCutoff);
	b.lp = b.lp + b.lp_1*(eCutoff);
	b.lp_2 = -b.lp_1*eRezo;

	b.lp_1 = b.lp_1 + b.lp_2*(eCutoff);
	b.lp = b.lp + b.lp_1*(eCutoff);
	b.lp_2 = -b.lp_1*eRezo;

	b.lp_1 = b.lp_1 + b.lp_2*(eCutoff);
	b.lp = b.lp + b.lp_1*(eCutoff);
	b.lp_2 = -b.lp_1*eRezo;

	b.lp_1 = b.lp_1 + b.lp_2*(eCutoff);
	b.lp = b.lp + b.lp_1*(eCutoff);
	b.lp_2 = -b.lp_1*eRezo;

	b.lp_1 = b.lp_1 + b.lp_2*(eCutoff);
	b.lp = b.lp + b.lp_1*(eCutoff);
	result = b.lp;

	return result;
}

long double IIR_doIIRCycle2(tbiquad & b, long double eCutoff, long double eRezo, long double cur)
{
	long double result;

	b.lp = cur - b.lp_1*(1.0f - eRezo) - b.lp_2;
	b.lp_1 = b.lp_1+b.lp*eCutoff;
	b.lp_2 = b.lp_2+b.lp_1*eCutoff;
	result = b.lp_1;

	if (result>1.0f) result = 1.0f;
	if (result<-1.0f) result = -1.0f;

	return result;
}

long double IIR_doIIRhigh(tbiquad & b, long double eCutoff, long double cur)
{
	long double result;

	// high pass filter! (brighten sound)
	result = b.lp;
	b.lp = b.lp+(cur-b.lp)*eCutoff;
	result = cur-b.lp;

	b.lp_1 = b.lp_1+(result-b.lp_1)*eCutoff;
	result = result-b.lp_1;

	b.lp_2 = b.lp_2+(result-b.lp_2)*eCutoff;
	result = result-b.lp_2;

	if (result < -1.0f) result = -1.0f;
	if (result > 1.0f) result = 1.0f;

	return result;
}






// more hacking by wwelti:

#define FUNTAB1_SIZE 64  // Don't whine about the 64. That's plenty. We're going to use linear interpolation anyway.

double funTab1[FUNTAB1_SIZE+1];

void initFunTab1()
{
  int i;
  for (i=0; i<FUNTAB1_SIZE+1; i++)
  {
    double v = double(i)/FUNTAB1_SIZE;
    funTab1[i] = sin( sin( sin( sin(v*(pi*0.5f)) *(pi*0.5f)) *(pi*0.5f)) *(pi*0.5f));
  }
}


inline double getTab1Interpolated(double val)   // By wwelti. Read the table tab1, linear interpolated.
{
  double tabbed_val = val*FUNTAB1_SIZE;           // 1x fmul
  int i = int(tabbed_val);                       // 1x fist
  double rest = tabbed_val - i;                  // 1x fild, 1xfsub

  // return tab[i] * (1.f-rest) + tab[i+1] * rest;  // linear interpolation : 2xfld, 2xfmul, 1xfsub, 1xfadd

  double t0 = funTab1 [i];                       // 1x fld         // faster linear interpolation (same result)
  double t1 = funTab1 [i+1];                     // 1x fld
  return t0 + (t1 - t0) * rest;                  // 1x fsub, 1x fmul, 1x fadd
}


long double wwelti_IIR_IIRlp3pole(tbiquad & b, long double eCutoff, long double eRezo, long double cur, long double freq)
{
	long double result;
//	long double frqdiff;

	if (eCutoff<0.01f) eCutoff = 0.01f;
	if (eCutoff>0.7f) eCutoff = 0.7f;
	// Negative-Rezo clamp removed - the 2002 Delphi UI allowed users to
	// enter negative values, and the "inverted" character they remember
	// depends on negative resonance passing through. Upper clamp kept to
	// prevent outright filter blow-up.
	if (eRezo>1.f) eRezo = 1.f;

	//Bestem min/max resonens og bestem slide!
	freq *= (1.f/44100.f);

  // OOOH NO! Not this way! NOT THIS WAY! (comment by wwelti)
	//eRezo = (eRezo*(0.965f-(freq*0.975f) )) + sin( sin( sin( sin(eCutoff*(pi*0.5f)) *(pi*0.5f)) *(pi*0.5f)) *(pi*0.5f)) *( (0.023f) + (freq*0.42f) );

	eRezo = (eRezo*(0.965f-(freq*0.975f) )) + getTab1Interpolated(eCutoff) *( (0.023f) + (freq*0.42f) );


	eRezo = eRezo*1.01f;

	if (eCutoff<0.05f)
	{
		//eRezo = eRezo* ((eCutoff/(0.05f))*0.1f+0.9f);
		eRezo = eRezo* (eCutoff*2.f+0.9f);
	}

	eRezo = 1.f-eRezo;

	b.lp_2 = cur-b.lp_1*eRezo-b.lp;
	b.lp_1 = b.lp_1 + b.lp_2*(eCutoff);
	b.lp = b.lp + b.lp_1*(eCutoff);
	b.lp_2 = -b.lp_1*eRezo;

	b.lp_1 = b.lp_1 + b.lp_2*(eCutoff);
	b.lp = b.lp + b.lp_1*(eCutoff);
	b.lp_2 = -b.lp_1*eRezo;

	b.lp_1 = b.lp_1 + b.lp_2*(eCutoff);
	b.lp = b.lp + b.lp_1*(eCutoff);
	b.lp_2 = -b.lp_1*eRezo;

	b.lp_1 = b.lp_1 + b.lp_2*(eCutoff);
	b.lp = b.lp + b.lp_1*(eCutoff);
	b.lp_2 = -b.lp_1*eRezo;

	b.lp_1 = b.lp_1 + b.lp_2*(eCutoff);
	b.lp = b.lp + b.lp_1*(eCutoff);
	result = b.lp;

	return result;
}


long double wwelti_IIR_IIRlp3pole_boost(tbiquad & b, long double eCutoff, long double eRezo, long double cur, long double freq)
{
	long double result;

	if (eCutoff<0.01f) eCutoff = 0.01f;
	if (eCutoff>0.7f) eCutoff = 0.7f;
	// The whole point of this variant: the stock function clamps eRezo at
	// 1.0, which sits just above where the knob already lands, so resonance
	// can never cross into self-oscillation. Here the ceiling is 1.10 -
	// enough for the boosted knob mapping in CBEngine and no more.
	if (eRezo>1.10f) eRezo = 1.10f;

	freq *= (1.f/44100.f);

	eRezo = (eRezo*(0.965f-(freq*0.975f) )) + getTab1Interpolated(eCutoff) *( (0.023f) + (freq*0.42f) );

	eRezo = eRezo*1.01f;

	if (eCutoff<0.05f)
	{
		eRezo = eRezo* (eCutoff*2.f+0.9f);
	}

	eRezo = 1.f-eRezo;

	b.lp_2 = cur-b.lp_1*eRezo-b.lp;
	b.lp_1 = b.lp_1 + b.lp_2*(eCutoff);
	b.lp = b.lp + b.lp_1*(eCutoff);
	b.lp_2 = -b.lp_1*eRezo;

	b.lp_1 = b.lp_1 + b.lp_2*(eCutoff);
	b.lp = b.lp + b.lp_1*(eCutoff);
	b.lp_2 = -b.lp_1*eRezo;

	b.lp_1 = b.lp_1 + b.lp_2*(eCutoff);
	b.lp = b.lp + b.lp_1*(eCutoff);
	b.lp_2 = -b.lp_1*eRezo;

	b.lp_1 = b.lp_1 + b.lp_2*(eCutoff);
	b.lp = b.lp + b.lp_1*(eCutoff);
	b.lp_2 = -b.lp_1*eRezo;

	b.lp_1 = b.lp_1 + b.lp_2*(eCutoff);
	b.lp = b.lp + b.lp_1*(eCutoff);
	result = b.lp;

	// --- the bit that makes crossing the stability point survivable ---
	// This topology is five naked integrator passes with no saturating
	// nonlinearity anywhere in the feedback path, so above the stability
	// point b.lp_1 grows geometrically until it hits inf and then NaN - and
	// a NaN in this state is permanent, it never washes out.
	//
	// Applied AFTER `result` is taken, so the current sample's output is not
	// distorted; only the state carried into the next sample is bounded.
	// Soft knee: transparent below T, asymptotic to T+K above it, so low and
	// mid resonance settings behave exactly as they would unbounded.
	{
		const long double T = 2.0L, K = 6.0L;
		long double x = b.lp_1;
		if      (x >  T) b.lp_1 =  T + (x - T) / (1.0L + (x - T) / K);
		else if (x < -T) b.lp_1 = -T - (-x - T) / (1.0L + (-x - T) / K);

		// b.lp is pulled back by its own -b.lp term in the feedback, so it is
		// self-limiting once b.lp_1 is bounded. This is only a backstop.
		const long double LPMAX = 64.0L;
		if      (b.lp >  LPMAX) b.lp =  LPMAX;
		else if (b.lp < -LPMAX) b.lp = -LPMAX;

		// Belt and braces. Should be unreachable given the above, but a
		// wedged filter is bad enough to be worth two compares.
		if (!(b.lp == b.lp) || !(b.lp_1 == b.lp_1)) {
			b.lp = b.lp_1 = b.lp_2 = b.lp_3 = b.lp_4 = b.lp_5 = 0.0L;
			result = 0.0L;
		}
	}

	return result;
}
