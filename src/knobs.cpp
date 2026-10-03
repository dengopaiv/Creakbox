//
// knobs
//

#include "knobs.h"
#include "core.h"
#include  <math.h>

//tKnobSetting * 

tKnob	prevVolume, nextVolume;
tKnob	prevTune, nextTune;
tKnob	prevCutoff, nextCutoff;
tKnob	prevDecay, nextDecay;
tKnob	prevEnvMod, nextEnvMod;
tKnob	prevRezo, nextRezo;
tKnob	prevAccent, nextAccent;

long posVolume,
	posTune,
	posCutoff,
	posDecay,
	posEnvMod,
	posRezo,
	posAccent;

tKnobSetting * AS_Volume = 0;
tKnobSetting * AS_Tune = 0;
tKnobSetting * AS_Cutoff = 0;
tKnobSetting * AS_Res = 0;
tKnobSetting * AS_EnvMod = 0;
tKnobSetting * AS_Decay = 0;
tKnobSetting * AS_Accent = 0;

int AS_VolumeSteps = 0;
int AS_TuneSteps = 0;
int AS_CutoffSteps = 0;
int AS_ResSteps = 0;
int AS_EnvModSteps = 0;
int AS_DecaySteps = 0;
int AS_AccentSteps = 0;

void setAutomationNumSteps(int type, int amount)
{
	switch (type)
	{
	case(AT_Volume): if (AS_VolumeSteps == 0) { AS_Volume = new tKnobSetting[amount]; AS_VolumeSteps = amount; } break;
	case(AT_Tune): if (AS_TuneSteps == 0) { AS_Tune = new tKnobSetting[amount]; AS_TuneSteps = amount; } break;
	case(AT_Cutoff): if (AS_CutoffSteps == 0) { AS_Cutoff = new tKnobSetting[amount]; AS_CutoffSteps = amount; } break;
	case(AT_Res): if (AS_ResSteps == 0) { AS_Res = new tKnobSetting[amount]; AS_ResSteps = amount; } break;
	case(AT_EnvMod): if (AS_EnvModSteps == 0) { AS_EnvMod = new tKnobSetting[amount]; AS_EnvModSteps = amount; } break;
	case(AT_Decay): if (AS_DecaySteps == 0) { AS_Decay = new tKnobSetting[amount]; AS_DecaySteps = amount; } break;
	case(AT_Accent): if (AS_AccentSteps == 0) { AS_Accent = new tKnobSetting[amount]; AS_AccentSteps = amount; } break;
	}
}

void setAutomationStep(int type, int index, int val1, int val2, int val3)
{
	switch (type)
	{
	case(AT_Volume): { AS_Volume[index].val1 = val1; AS_Volume[index].val2 = val2; AS_Volume[index].val3 = val3; } break;
	case(AT_Tune): { AS_Tune[index].val1 = val1; AS_Tune[index].val2 = val2; AS_Tune[index].val3 = val3; } break;
	case(AT_Cutoff): { AS_Cutoff[index].val1 = val1; AS_Cutoff[index].val2 = val2; AS_Cutoff[index].val3 = val3; } break;
	case(AT_Res): { AS_Res[index].val1 = val1; AS_Res[index].val2 = val2; AS_Res[index].val3 = val3; } break;
	case(AT_EnvMod): { AS_EnvMod[index].val1 = val1; AS_EnvMod[index].val2 = val2; AS_EnvMod[index].val3 = val3; } break;
	case(AT_Decay): { AS_Decay[index].val1 = val1; AS_Decay[index].val2 = val2; AS_Decay[index].val3 = val3; } break;
	case(AT_Accent): { AS_Accent[index].val1 = val1; AS_Accent[index].val2 = val2; AS_Accent[index].val3 = val3; } break;
	}
}

void clearAutomationSteps()
{
	if (AS_Volume) delete [] AS_Volume;
	if (AS_Tune) delete [] AS_Tune;
	if (AS_Cutoff) delete [] AS_Cutoff;
	if (AS_Res) delete [] AS_Res;
	if (AS_EnvMod) delete [] AS_EnvMod;
	if (AS_Decay) delete [] AS_Decay;
	if (AS_Accent) delete [] AS_Accent;
}


long double getVolume(int order, long double offset)
{
	long double temp, result;

	temp =order+offset/100.f;
	if (temp>nextTune.offset)
	{	//skip position frame!
		prevVolume = nextVolume;
		posVolume++;
		nextVolume.offset = AS_Volume[posVolume].val1 + AS_Volume[posVolume].val2/100.f;
		nextVolume.value  = AS_Volume[posVolume].val3;
	}
	result = prevVolume.value+((nextVolume.value-prevVolume.value)*((temp-prevVolume.offset)/(nextVolume.offset-prevVolume.offset)));
	result = (result*0.1f);

	return result;
}

long double getTune(int order, long double offset)
{
	long double temp, result;

	temp =order+offset/100.f;
	if (temp>nextTune.offset)
	{ //skub position frem!
		prevTune = nextTune;
		posTune++;
		nextTune.offset = AS_Tune[posTune].val1 + AS_Tune[posTune].val2/100.f;
		nextTune.value  = AS_Tune[posTune].val3;
	}
	result = prevTune.value+((nextTune.value-prevTune.value)*((temp-prevTune.offset)/(nextTune.offset-prevTune.offset)));
	return result;
}

long double getCutoff(int order, long double offset)
{
	long double temp, result;

	temp = order+offset;///100.f);
	if (temp>nextCutoff.offset)
	{
		prevCutoff = nextCutoff;
		posCutoff++;
		nextCutoff.offset = AS_Cutoff[posCutoff].val1 + AS_Cutoff[posCutoff].val2/100.f;
		nextCutoff.value  = AS_Cutoff[posCutoff].val3;
	}
	result = (prevCutoff.value+((nextCutoff.value-prevCutoff.value)*((temp-prevCutoff.offset)/(nextCutoff.offset-prevCutoff.offset))))/100.f;

	result = result*0.5f;
#ifdef MACX
	result = (pow(2.f,result)-1.f); //0-1
	result = (pow(2.f,result*0.80f)-0.987f)*100.f;
#elif MAC || WIN32
	result = (pow(2.0,result)-1.0); //0-1
	result = (pow(2.0,result*0.80)-0.987)*100.0;
#endif

	return result;
}

long double getDecay(int order, long double offset)
{
	long double temp, result;

	temp = order+offset/100.f;
	if (temp>nextDecay.offset)
	{
		prevDecay=nextDecay;
		posDecay++;
		nextDecay.offset = AS_Decay[posDecay].val1 + AS_Decay[posDecay].val2/100.f;
		nextDecay.value  = AS_Decay[posDecay].val3;
	}
	result =prevDecay.value+((nextDecay.value-prevDecay.value)*((temp-prevDecay.offset)/(nextDecay.offset-prevDecay.offset)));
	return result;
}

long double getEnvMod(int order, long double offset)
{
	long double temp, result;

	temp = order+offset/100.f;
	if (temp>nextEnvMod.offset)	
	{
		prevEnvMod = nextEnvMod;
		posEnvMod++;
		nextEnvMod.offset = AS_EnvMod[posEnvMod].val1 + AS_EnvMod[posEnvMod].val2/100.f;
		nextEnvMod.value  = AS_EnvMod[posEnvMod].val3;
	}
	result = (prevEnvMod.value+((nextEnvMod.value-prevEnvMod.value)*	((temp-prevEnvMod.offset)/	(nextEnvMod.offset-prevEnvMod.offset)))	)/100.f;

	return result;
}


long double getRezo(int order, long double offset)
{
	long double temp, result;

	temp = order+offset/100.f;
	if (temp>nextRezo.offset)
	{
		prevRezo = nextRezo;
		posRezo++;
		nextRezo.offset = AS_Res[posRezo].val1 + AS_Res[posRezo].val2/100.f;
		nextRezo.value  = AS_Res[posRezo].val3;
	}
	result = (prevRezo.value+((nextRezo.value-prevRezo.value)*((temp-prevRezo.offset)/(nextRezo.offset-prevRezo.offset)))); // 0-1
	result = result/100.f;

	result = sin(result*halfpi);
	result = result*75.f+25.f;

	return result;
}

long double getAccent(int order, long double offset)
{
	long double temp, result;

	temp = order+offset/100.f;
	if (temp>nextAccent.offset)
	{
		prevAccent=nextAccent;
		posAccent++;
		nextAccent.offset = AS_Accent[posAccent].val1 + AS_Accent[posAccent].val2/100.f;
		nextAccent.value  = AS_Accent[posAccent].val3;
	}

	result = (prevAccent.value+((nextAccent.value-prevAccent.value)*((temp-prevAccent.offset)/(nextAccent.offset-prevAccent.offset))))*0.65f; //result=0-65

	return result;
}

long findgridpos(int type, long startpos)
{
	long row = 0;
	int numSteps = 0;

	tKnobSetting * ksKnob = 0;

	switch (type)
	{
	case(AT_Volume): { ksKnob = AS_Volume; numSteps = AS_VolumeSteps; } break;
	case(AT_Tune): { ksKnob = AS_Tune; numSteps = AS_TuneSteps; } break;
	case(AT_Cutoff): { ksKnob = AS_Cutoff; numSteps = AS_CutoffSteps; } break;
	case(AT_Res): { ksKnob = AS_Res; numSteps = AS_ResSteps; } break;
	case(AT_EnvMod): { ksKnob = AS_EnvMod; numSteps = AS_EnvModSteps; } break;
	case(AT_Decay): { ksKnob = AS_Decay; numSteps = AS_DecaySteps; } break;
	case(AT_Accent): { ksKnob = AS_Accent; numSteps = AS_AccentSteps; } break;
	}

	while ((ksKnob[row].val1 + (ksKnob[row].val2/100.f)) <= startpos)
	//while ((strtolong double(grid.cells[0,row])+ strtolong double(grid.cells[1,row])/100)<=startpos )
	{
		row++;
		if (row == numSteps) break;

	}

	return row;
}

void resetknobs(long startpos)
{
	startpos++;

	posVolume =findgridpos(AT_Volume, startpos);
	prevVolume.offset = AS_Volume[posVolume-1].val1 + AS_Volume[posVolume-1].val2/100.f;
	nextVolume.offset = AS_Volume[posVolume].val1 + AS_Volume[posVolume].val2/100.f;
	prevVolume.value  = AS_Volume[posVolume-1].val3;
	nextVolume.value  = AS_Volume[posVolume].val3;

  	posTune =findgridpos(AT_Tune, startpos);
	prevTune.offset = AS_Tune[posTune-1].val1 + AS_Tune[posTune-1].val2/100.f;
	nextTune.offset = AS_Tune[posTune].val1 + AS_Tune[posTune].val2/100.f;
	prevTune.value  = AS_Tune[posTune-1].val3;
	nextTune.value  = AS_Tune[posTune].val3;

  	posCutoff =findgridpos(AT_Cutoff, startpos);
	prevCutoff.offset = AS_Cutoff[posCutoff-1].val1 + AS_Cutoff[posCutoff-1].val2/100.f;
	nextCutoff.offset = AS_Cutoff[posCutoff].val1 + AS_Cutoff[posCutoff].val2/100.f;
	prevCutoff.value  = AS_Cutoff[posCutoff-1].val3;
	nextCutoff.value  = AS_Cutoff[posCutoff].val3;

  	posDecay =findgridpos(AT_Decay, startpos);
	prevDecay.offset = AS_Decay[posDecay-1].val1 + AS_Decay[posDecay-1].val2/100.f;
	nextDecay.offset = AS_Decay[posDecay].val1 + AS_Decay[posDecay].val2/100.f;
	prevDecay.value  = AS_Decay[posDecay-1].val3;
	nextDecay.value  = AS_Decay[posDecay].val3;

  	posEnvMod =findgridpos(AT_EnvMod, startpos);
	prevEnvMod.offset = AS_EnvMod[posEnvMod-1].val1 + AS_EnvMod[posEnvMod-1].val2/100.f;
	nextEnvMod.offset = AS_EnvMod[posEnvMod].val1 + AS_EnvMod[posEnvMod].val2/100.f;
	prevEnvMod.value  = AS_EnvMod[posEnvMod-1].val3;
	nextEnvMod.value  = AS_EnvMod[posEnvMod].val3;

  	posRezo =findgridpos(AT_Res, startpos);
	prevRezo.offset = AS_Res[posRezo-1].val1 + AS_Res[posRezo-1].val2/100.f;
	nextRezo.offset = AS_Res[posRezo].val1 + AS_Res[posRezo].val2/100.f;
	prevRezo.value  = AS_Res[posRezo-1].val3;
	nextRezo.value  = AS_Res[posRezo].val3;

  	posAccent =findgridpos(AT_Accent, startpos);
	prevAccent.offset = AS_Accent[posAccent-1].val1 + AS_Accent[posAccent-1].val2/100.f;
	nextAccent.offset = AS_Accent[posAccent].val1 + AS_Accent[posAccent].val2/100.f;
	prevAccent.value  = AS_Accent[posAccent-1].val3;
	nextAccent.value  = AS_Accent[posAccent].val3;

}
