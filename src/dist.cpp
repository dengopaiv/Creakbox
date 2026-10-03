// USE THIS FILE.. DO NOT USE DIST.C
//unit distort;
// uses sysutils;
// uses pcb, iir, math, freq;

#include <math.h>

//#include "core.h"
//#include "freq.h"
#include "dist.h"
#include "IIR.h"

long double d_ClampThreshold;
long double d_ClampLevel;
long double d_Distortout;
long double d_dryout;
long double old_x;
long double d_amp;
long double d_prevdist;
long double d_highpass;
tbiquad d_hibi;
long double d_lowpass;

void dist_initdist()
{
	//distortion!!
	IIR_resetIIR(d_hibi);
}

long double dist_dist(long double x)
{
	/*
	function distort(x:extended):extended;
var
  temp:extended;
begin
{  if x>0 then result:=0.3 else
    if result<0 then result:=-0.3;

  result:=doIIRhigh(dist_biquad,0.01,0,result);

  result:=(x*(1-form1.sbdistort.position/100))+ (result*(form1.sbdistort.position/100));

  if result<-1 then result:=-1;
  if result>1 then result:=1;
  result:=doIIRcycle(dist_biquad2,0.4,0,result,0);}

  result:=x;

end;

end.
	*/
	long double result;

	// Clamp!!
	if (fabs(x)>0.8f)
	{
		if (x>0.8f)
		{
			x = 0.8f;
		}
		else if (x<-0.78f) x = -0.78f;
	}

	//HIGHPASS A LITTLE
	x = IIR_doIIRhigh(d_hibi,0.001f,x);
	result = x;
	if (result<-1.f)
	{
		result = -1.f;
	}
	else if (result>1.f)
	{
		result = 1.f;
	}

	return result;
}