//
// knobs
//

#ifndef _KNOBS_H_
#define _KNOBS_H_

struct tKnobSetting
{
	long double val1;
	long double val2;
	long double val3;
};

struct tKnob
{
	long double offset; //offset (lborder.order+lborder.offset/100)
	long double value;   //knob-pos (lborder.value)
};


enum // Automation Type
{
	AT_Volume,
	AT_Tune,
	AT_Cutoff,
	AT_Res,
	AT_EnvMod,
	AT_Decay,
	AT_Accent
};

void resetknobs(long startpos);
long double getVolume(int order, long double offset);
long double getTune(int order, long double offset);
long double getCutoff(int order, long double offset);
long double getDecay(int order, long double offset);
long double getEnvMod(int order, long double offset);
long double getRezo(int order, long double offset);
long double getAccent(int order, long double offset);
long findgridpos(int type, long startpos/*, grid:tStringGrid*/);

void setAutomationNumSteps(int type, int amount);
void setAutomationStep(int type, int index, int val1, int val2, int val3);
void clearAutomationSteps();

#endif _KNOBS_H_










