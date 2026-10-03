//
// SilverBox.h
//

// FIX: changing patterns from front panel.. if pattern lock mode is on (cant change next pattern cuz its too late)
//		then make sure to unpress the button that was pressed

// TODO:
// -1) fix pattern button lock glitch in cycle mode
// 0) fix cubase reset glitch
// 1) adjust for diff sample rates for float constants
// 2) OPTIMIZE
// 3) REDO UI GFX
// 4) add copy protection scheme - serial # or key file
// 5) make demo

//DONE:
// 1) sync/run/stop/tempo
// 2) only play pattern from beginning.. to play from middle
// 3) add new bank/patch management/ui
// 4) add new octave/key programming
// 5) REDO SEQUENCER
// 6) fixed major glitches
// OCT:
// 1) add midi program change
// 2) added load save fxb/fxp
// NOV:
// 1) added next pattern display
// 2) fixed bank format to store pattern names, may still change format before final release
// 3) added cycling system
// NOV 17:
// 1) cubase reset pos glitch fixed
// NOV 24:
// 1) fixed note off -> octave glitch
// 2) fixed fruity loops glitch
// 3) remove fxb/fxp loading/saving (since ableton live 2.0 will have this feature)


//WONT DO:
// x) ppqPos - optimize - handle time signatures
// x) only set next pattern before the 16th note - maybe
//OCT:
// 5) midi learn - saving/loading settings


// 3/4 timing = 3 1/4 notes per measure

/*
<AFH> no not reliably
<AFH> you need to use the fact that the host sets the plug's resource context b4 calling the constructor
<AFH> you get the resource context, then get the file handle from that, then get the directory from the file handle

  <AFH> yeah... use FSpGetFileLocation
<AFH> there's a file somewhere on developer.apple.com called MoreFiles.c (i think that's the name)
<AFH> anyway it has source for FSpGetFileLocation() and various supporting functions
<AFH> and you can just pass it the result from CurResFile(), it will return you an FSSpec
*/



// SEQUENCER DESIGN
// each step is a button (accent), knob (pitch/off), button (glide)
//
//

#ifndef __SCRATCHA__
#define __SCRATCHA__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "vstfxstore.h"

//#define _FX_VERSION_
#define _PROCESS_DEBUG_
//#define _BANKS_

//#include "CWaveFile.h"
//#include "CreakBox.h"

#ifndef __AudioEffectX__
#include "audioeffectx.h"
#endif

// _LIMITED_ is the demo version with only 1 pattern
//#define _LIMITED_ 
#define _FILE_CHUNK_
//#define _LOG_
//#define _CYCLE_
//#define _RANDOM_PATTERNS_
//#define _DEBUG_STRING_
#define _FRANKS_TWEAK_


#include "core.h"
#include "freq.h"
#include "IIR.h"
#include "dist.h"
//#include "knobs.h"

const double Pi = 3.141592653f;

//e = 2.718281828459045235360287471352662497757247093699959574966
#define ln(x) log (x) //log(x)/log(2.718281828459045235360287471352662497757247093699959574966)
#define log2(x) log(x)/log(2.0f)

#define PITCH_CENTER 0x1FFF
#define NUM_STEPS 16
//#define NUM_PITCHES 13

#define _FRANK_TWEAK_


// parameters
enum
{
	// performance parameters
	kVolume,
	kWaveform,
	kTuning,
	kCutOffFreq,
	kResonance,
	kEnvMod,
	kDecay,
	kAccent,
	kNumParams,
//	kPartSelect,
//	kPartMute,
//	kPartActivity,
/*
	kPart1,
	kPart2,
	kPart3,
	kPart4,
	kPart5,
	kPart6,
	kPart7,
	kPart8,
*/
	/*
	kMute1,
	kMute2,
	kMute3,
	kMute4,
	kMute5,
	kMute6,
	kMute7,
	kMute8,
	*/
#ifdef _MIDI_LEARN_
	kMidiLearn,
#endif
	kTempo,
//	kNextPattern,
#ifndef _LIMITED_
	kClear,
	kCopy,
	kPaste,
#ifdef _RANDOM_PATTERNS_
	kRandom1,
	kRandom2,
#endif
#endif
//	kPatternDisplay,
//	kTempoDisplay,
//	kPattern,
//	kTempo,
//	kValue, // big knob
	kRunStop,
	kSync,
	kAccent01,
	kAccent02,
	kAccent03,
	kAccent04,
	kAccent05,
	kAccent06,
	kAccent07,
	kAccent08,
	kAccent09,
	kAccent10,
	kAccent11,
	kAccent12,
	kAccent13,
	kAccent14,
	kAccent15,
	kAccent16,
  	kPitch01,
	kPitch02,
	kPitch03,
	kPitch04,
	kPitch05,
	kPitch06,
	kPitch07,
	kPitch08,
	kPitch09,
	kPitch10,
	kPitch11,
	kPitch12,
	kPitch13,
	kPitch14,
	kPitch15,
	kPitch16,
  	kSlide01,
	kSlide02,
	kSlide03,
	kSlide04,
	kSlide05,
	kSlide06,
	kSlide07,
	kSlide08,
	kSlide09,
	kSlide10,
	kSlide11,
	kSlide12,
	kSlide13,
	kSlide14,
	kSlide15,
	kSlide16,
//	kNumParams,
	kPitchDisplay01,
	kPitchDisplay02,
	kPitchDisplay03,
	kPitchDisplay04,
	kPitchDisplay05,
	kPitchDisplay06,
	kPitchDisplay07,
	kPitchDisplay08,
	kPitchDisplay09,
	kPitchDisplay10,
	kPitchDisplay11,
	kPitchDisplay12,
	kPitchDisplay13,
	kPitchDisplay14,
	kPitchDisplay15,
	kPitchDisplay16,
	kTempoDisplay,
#ifndef _LIMITED_
	kCurrentPatternDisplay,
	kNextPatternDisplay,
#endif
	kStep01,
	kStep02,
	kStep03,
	kStep04,
	kStep05,
	kStep06,
	kStep07,
	kStep08,
	kStep09,
	kStep10,
	kStep11,
	kStep12,
	kStep13,
	kStep14,
	kStep15,
	kStep16,
#ifndef _LIMITED_
	kBank00,
	kBank01,
	kBank02,
	kBank03,
	kBank04,
	kBank05,
	kBank06,
	kBank07,
	kBank08,
	kBank09,
	kPreset00,
	kPreset01,
	kPreset02,
	kPreset03,
	kPreset04,
	kPreset05,
	kPreset06,
	kPreset07,
	kPreset08,
	kPreset09,
#endif
	kNoteOff,
	kNote01,
	kNote02,
	kNote03,
	kNote04,
	kNote05,
	kNote06,
	kNote07,
	kNote08,
	kNote09,
	kNote10,
	kNote11,
	kNote12,
	kOctave01,
	kOctave02,
	kOctave03,
	kOctave04,
	kOctave05,
	kStepEdit01,
	kStepEdit02,
	kStepEdit03,
	kStepEdit04,
	kStepEdit05,
	kStepEdit06,
	kStepEdit07,
	kStepEdit08,
	kStepEdit09,
	kStepEdit10,
	kStepEdit11,
	kStepEdit12,
	kStepEdit13,
	kStepEdit14,
	kStepEdit15,
	kStepEdit16,
#ifndef _LIMITED_

#ifdef _BANKS_
	kLoadFXP,
	kLoadFXB,
	kSaveFXP,
	kSaveFXB,
#endif
#ifdef _MIDI_LEARN_
	kLoadMidiMap,
	kSaveMidiMap,
#endif
	kCycle,
	kSetCycleStart,
	kSetCycleFinish,
	kStartDisplay,
	kFinishDisplay,
#endif
	kStepEditMode,
	kPage00,
	kPage01,
	kPage02,
	kPage03,
	kAbout
};

enum
{
#ifdef _LIMITED_
	kNumPrograms = 1,
#else
	kNumPrograms = 100,
#endif
	kNumOutputs = 2
};


enum
{
	kEditStep = 0,
	kLastStep
};
// tb-303 sequence should have the following info per step:
// note type: note, sustain, rest
// trans up, down, accent, slide
// pitch

#define CBSTEP_NOTE		0x01
#define CBSTEP_SUSTAIN	0x02
#define CBSTEP_REST		0x04
#define CBSTEP_BLAH		0x08
#define CBSTEP_UP		0x10
#define CBSTEP_DOWN		0x20
#define CBSTEP_ACCENT	0x40
#define CBSTEP_SLIDE	0x80


struct t_CB_Step
{
	char cPitch;
	char bAccent;
	char bSlide;

	t_CB_Step()
	{
		cPitch = 0;
		bAccent = 0;
		bSlide = 0;
	};

	void operator=(t_CB_Step & copy)
	{
		cPitch = copy.cPitch;
		bAccent = copy.bAccent;
		bSlide = copy.bSlide;
	}
};

#define _MAX_STEPS_ 64

struct t_CB_Pattern
{
	int LastStep;
	t_CB_Step Step[_MAX_STEPS_];

	t_CB_Pattern()
	{
		LastStep = 15;
		for (int step=0;step<_MAX_STEPS_; step++)
		{
			Step[step].cPitch = 0;  Step[step].bSlide = 0;  Step[step].bAccent = 0;
		}

		Step[0].cPitch = 13;  Step[0].bSlide = 1;  Step[0].bAccent = 0;
		Step[1].cPitch = 35;  Step[1].bSlide = 0;  Step[1].bAccent = 0;
		Step[2].cPitch = 28;  Step[2].bSlide = 0;  Step[2].bAccent = 1;
		Step[3].cPitch = 13;  Step[3].bSlide = 0;  Step[3].bAccent = 0;
		Step[4].cPitch = 29;  Step[4].bSlide = 1;  Step[4].bAccent = 0;
		Step[5].cPitch = 25;  Step[5].bSlide = 1;  Step[5].bAccent = 0;
		Step[6].cPitch = 0;   Step[6].bSlide = 0;  Step[6].bAccent = 1;
		Step[7].cPitch = 13;  Step[7].bSlide = 0;  Step[7].bAccent = 0;
		Step[8].cPitch = 29;  Step[8].bSlide = 1;  Step[8].bAccent = 1;
		Step[9].cPitch = 35;  Step[9].bSlide = 0;  Step[9].bAccent = 0;
		Step[10].cPitch = 29; Step[10].bSlide = 1; Step[10].bAccent = 0;
		Step[11].cPitch = 35; Step[11].bSlide = 0; Step[11].bAccent = 1;
		Step[12].cPitch = 20; Step[12].bSlide = 0; Step[12].bAccent = 0;
		Step[13].cPitch = 42; Step[13].bSlide = 1; Step[13].bAccent = 0;
		Step[14].cPitch = 25; Step[14].bSlide = 1; Step[14].bAccent = 0;
		Step[15].cPitch = 0;  Step[15].bSlide = 0; Step[15].bAccent = 0;

//		memset(&Name, 0, sizeof(Name));

// rebirth test
		/*
		Step[0].cPitch = 8;  Step[0].bSlide = 1;  Step[0].bAccent = 1;
		Step[1].cPitch = 29;  Step[1].bSlide = 1;  Step[1].bAccent = 0;
		Step[2].cPitch = 18;  Step[2].bSlide = 1;  Step[2].bAccent = 0;
		Step[3].cPitch = 19;  Step[3].bSlide = 1;  Step[3].bAccent = 0;
		Step[4].cPitch = 25;  Step[4].bSlide = 0;  Step[4].bAccent = 0;
		Step[5].cPitch = 0;  Step[5].bSlide = 0;  Step[5].bAccent = 0;
		Step[6].cPitch = 18;   Step[6].bSlide = 0;  Step[6].bAccent = 0;
		Step[7].cPitch = 0;  Step[7].bSlide = 0;  Step[7].bAccent = 0;
		Step[8].cPitch = 0;  Step[8].bSlide = 0;  Step[8].bAccent = 0;
		Step[9].cPitch = 18;  Step[9].bSlide = 0;  Step[9].bAccent = 0;
		Step[10].cPitch = 0; Step[10].bSlide = 0; Step[10].bAccent = 0;
		Step[11].cPitch = 18; Step[11].bSlide = 0; Step[11].bAccent = 0;
		Step[12].cPitch = 0; Step[12].bSlide = 0; Step[12].bAccent = 0;
		Step[13].cPitch = 0; Step[13].bSlide = 0; Step[13].bAccent = 0;
		Step[14].cPitch = 19; Step[14].bSlide = 0; Step[14].bAccent = 1;
		Step[15].cPitch = 0;  Step[15].bSlide = 0; Step[15].bAccent = 0;
		*/
/*
		Step[0].cPitch = 13; // Step[0].bAccent = 1;  Step[0].bSlide = 0;
		Step[1].cPitch = 35; // Step[1].bAccent = 0;  Step[1].bSlide = 0;
		Step[2].cPitch = 28; // Step[2].bAccent = 0;  Step[2].bSlide = 1;
		Step[3].cPitch = 13; // Step[3].bAccent = 0;  Step[3].bSlide = 0;
		Step[4].cPitch = 29; // Step[4].bAccent = 1;  Step[4].bSlide = 0;
		Step[5].cPitch = 25; // Step[5].bAccent = 1;  Step[5].bSlide = 0;
		Step[6].cPitch = 0;  // Step[6].bAccent = 0;  Step[6].bSlide = 1;
		Step[7].cPitch = 13; // Step[7].bAccent = 0;  Step[7].bSlide = 0;
		Step[8].cPitch = 29; // Step[8].bAccent = 1;  Step[8].bSlide = 1;
		Step[9].cPitch = 35; // Step[9].bAccent = 0;  Step[9].bSlide = 0;
		Step[10].cPitch = 29;// Step[10].bAccent = 1; Step[10].bSlide = 0;
		Step[11].cPitch = 35;// Step[11].bAccent = 0; Step[11].bSlide = 1;
		Step[12].cPitch = 20;// Step[12].bAccent = 0; Step[12].bSlide = 0;
		Step[13].cPitch = 42;// Step[13].bAccent = 1; Step[13].bSlide = 0;
		Step[14].cPitch = 25;// Step[14].bAccent = 1; Step[14].bSlide = 0;
		Step[15].cPitch = 0; // Step[15].bAccent = 0; Step[15].bSlide = 0;
*/
	};
};

/*
bark1:
00	C^2	NOTE	DOWN
01		SUSTAIN
02	C^2	NOTE	ACCENT
03		SUSTAIN

bark2:
00	C^2	NOTE	down slide
01		SUSTAIN
02	C^2 NOTE	UP ACCENT
03		SUSTAIN
*/
/* testes.cb
step	note	slide	accent
00		13		1		0	c-1
01		35		0		0	a#2
02		28		0		1	d#2
03		13		0		0	c-1
04		29		1		0	e-2
05		25		1		0	c-2
06		0		0		1
07		13		0		0	c-1
08		29		1		1	e-2
09		35		0		0	a#2
10		29		1		0	e-2
11		35		0		1	a#2
12		20		0		0	g-1
13		42		1		0	f-3
14		25		1		0	c-2
15		0		0		0
*/
/*
struct t_CB_Bank
{
	t_CB_Pattern Pattern[128];
};
*/
struct t_CB_Part
{
	float	fVolume;
	float	fWaveform;
	float	fTuning;
	float	fCutOffFreq;
	float	fResonance;
	float	fEnvMod;
	float	fDecay;
	float	fAccent;

	t_CB_Part()
	{
		fVolume = 0.0f;
		fWaveform = 0.0f;
		fTuning = 0.0f;
		fCutOffFreq = 0.0f;
		fResonance = 0.0f;
		fEnvMod = 0.0f;
		fDecay = 0.0f;
		fAccent = 0.0f;
		
		// temp init
		fVolume = 1.0f;
		fTuning = 0.5f;
		fCutOffFreq = 0.15f;
		fResonance = 1.0f;
		fEnvMod = 0.1f;
		fDecay = 0.90f;
		fAccent = 1.0f;
		fWaveform = 1.0f;

		// volume - 100
		// tune - 50
		//cutoff - 15
		// rezo - 100
		// env mod - 10
		// decay - 90
		// accent - 100
	}
};

class SilverBoxProgram
{
friend class SilverBox;
public:
	SilverBoxProgram();
	~SilverBoxProgram() {}
private:
	/*
	float fWaveform1;
	float fFreq1;
	float fVolume1;
	float fWaveform2;
	float fFreq2;
	float fVolume2;
	*/
	float m_fVolume;
	char name[24];
};

struct t_CBX_Preset  //used for saving 1 pattern to disk only
{
	char cId[4];
	char iVersion[2];
	t_CB_Pattern tPattern;
};

struct t_CBX_Bank_Old // old format that will go away with official 1.0 release
{
	char cId[4];
	char iVersion[2];
	t_CB_Pattern tPattern[100];
};

// 100 program bank
struct t_CBX_Bank
{
	char cId[4];
	char iVersion[2];
	char Name[kNumPrograms][24];
	t_CB_Pattern tPattern[kNumPrograms];
};
/*
	kPower = 0,
	kMute,
	kPitchAmount,
	kPitchRange,
	kDirection,
	kNPTrack,
	kScratchAmount,
	kScratchSpeed,
	kTorque,

	// support parameters
	kSpinUpSpeed,
	kSpinDownSpeed,
	kNoteMode,
	kLoopMode,
	kToneArmWeight,
	kKeyTracking,
	kRootKey,
	kVolume,
	kSampleRateDisplay, // for displaying purposes only
	kLoad, // load button
	kPlay, // play/note activity
	kOptions,
	kSave,
	kAbout,
	kFileName,
	kNumParams
*/

#define NUM_PARTS 8

//------------------------------------------------------------------------------------------
class SilverBox : public AudioEffectX
{
friend class MoogProgram;
public:
	SilverBox(audioMasterCallback audioMaster);
	~SilverBox();

	virtual void process(float **inputs, float **outputs, long sampleframes);
	virtual void processReplacing(float **inputs, float **outputs, long sampleframes);
	void doTheProcess(float *inputs1, float *inputs2, float **outputs, long sampleFrames);
	virtual long processEvents(VstEvents* events);

	virtual long getProgram();
#ifndef _LIMITED_
	virtual void setProgram(long program);
#endif
	virtual void setProgramName(char *name);
	virtual void getProgramName(char *name);
	virtual void setParameter(long index, float value);
	virtual float getParameter(long index);
	virtual void getParameterLabel(long index, char *label);
	virtual void getParameterDisplay(long index, char *text);
	virtual void getParameterName(long index, char *text);
	virtual void setSampleRate(float sampleRate);
	virtual void setBlockSize(long blockSize);
	virtual void resume();

	virtual bool getOutputProperties (long index, VstPinProperties* properties);
	virtual bool getProgramNameIndexed (long category, long index, char* text);
	virtual bool copyProgram (long destination);
	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual long getVendorVersion () {return 1;}
	virtual long canDo (char* text);
	virtual long fxIdle ();
	virtual bool canParameterBeAutomated (long index) { return true; }

#ifdef _FILE_CHUNK_
	virtual long getChunk(void** data, bool isPreset = false); // {return 0;}	// returns byteSize
	virtual long setChunk(void* data, long byteSize, bool isPreset = false); // {return 0;}
#endif
//	virtual long getChunk(void** data, bool isPreset = false); // {return 0;}	// returns byteSize
//	virtual long setChunk(void* data, long byteSize, bool isPreset = false); // {return 0;}

	void	processMidiEvent(long currEvent);

	// part functions
	void	SetPartMode(int tag);
	void	SetCurrentPart(int part);

	void	UpdatePartSynth(int part);
	void	UpdatePartSequencer(int part);


	// pattern function
//	void	SetCurrentPattern(int pattern);
//	void	SetNextPattern(int pattern);

	// value knob / pattern / tempo
//	void SetValueMode(bool bPattern);

	//
	void PatternClear();
	void PatternCopy();
	void PatternPaste();
#ifdef _RANDOM_PATTERNS_
	void PatternRandom1();
	void PatternRandom2();
#endif
	void RefreshPatternDisplay();
#ifndef _LIMITED_
	void RefreshPresetDisplay();
#endif

	void refreshSequencer();
	void setParameterInt(int index, int value);

	void refreshStepEdit();

	void LoadChunk(char * filename, bool isPreset);
	void SaveChunk(char * filename, bool isPreset);

	void SwapBytes(long &l);
	void SwapBytes(float &f);

	void openlog();
	void closelog();
	void writelog(const char * fmt, ...);

	inline bool GetNextPatternLock() { return bNextPatternLock; };
private:
	FILE * logfile;

private:
	void dumpVstTimeInfo(VstTimeInfo timeinfo);
	void dumpStep(int step, t_CB_Step & myStep);
	void initProcess();
	void noteOn(long note, long velocity, long delta);
	void PlayNote(float value);
	long FixMidiData(long param, char value);
	float FixMidiData(long param, float value);

	void setCurrentGroup(int group);
	void setCurrentPattern(int pattern);
	void SetCurrentStep(int step);

	void setLastStep(int step);
	void setCurrentPage(int page);
	void setStepEditMode(int mode);

	void SetStepPitch(int step, int pitch);
	void SetStepEdit(int step);

	long currentNote;
	long currentVelocity;
	long currentDelta;
	bool noteIsOn;

	float m_fRunStop;
	float m_fSync;

	// SCRATCHA STUFF y0!

	char m_szProgramName[24];


	float m_fNoteVolume;  // recalculated on note on and volume changes
	VstEvents * m_pVstEvents;

	bool m_bProcessing;


	t_CBX_Preset Preset;
	t_CBX_Bank Bank;

	t_CB_Part m_Part[NUM_PARTS];
	t_CB_Pattern m_CopyPattern;
	//t_CB_Pattern m_CurrPattern;

	
	// CB3O3 NON UI VARIABLES
	bool	_bPattern; // set to true of value knob is in pattern mode, if false then it is in tempo mode
	int		_nPartMode; // holds the enum tag of the mode
	int		_nCurrPart;	// current selected part, that the UI shows
	int		_nTempo;

	float *replacingInputs1, *replacingInputs2;

	double samples_per_beat; 
	int minibeat; 
	double oldppq; 
	long oldSampleFrames;
	bool nevercalled; 
	bool forceresynch; 
	bool restart_gate;
	double oldtempo; 
	
	double gate_samplestillfinished;
	double gate_samplestillgatefinished;
	float gate_gain;
	float gate_vector;
	int resynchgate;

	VstTimeInfo _tInfo;

	//
	// CreakBox code	
	//
	void CBConstruct();
	void CBDestroy();

	void CBInit(char * inFilename); // load wav file
#ifdef _FX_VERSION_
	//ignore inputs for now
	//void CBProcess(float *inputs1, float *inputs2, float **outputs, long sampleFrames);
	void CBProcess(float * buffer, int sampleFrames);
#else
	void CBProcess(float * buffer, int sampleFrames);
#endif

	long double inputKnobVolume; // Knobpositions
	long double inputKnobCutoff;
	long double inputKnobRezo;
	long double inputKnobEnvMod;
	long double inputKnobAccent;
	long double inputKnobTune;
	long double inputKnobDecay;
	char boxSndType;

	int eSBN; // samples between notes!
	long double boxBPM;
	double sampleRate;
	long double currPos;



private:
	
	char * readline();
	bool CBgetnextslide(int currnote);
	long double CBgetnextnote(int currnote);
	void CBReset(); // reset pattern

	FILE * inFile;
	char line[100];

	long c_pattern;  // counts patterns

	long double eNextNote; // used for calculating slide!
	int bNoteSlide;
	int bSlideNext;

	long liStartPos;
	long liEndPos;  // Start and end-pattern to render (songposition)

	tbiquad lp1_biquad;    //lowpass 1
	tbiquad lp2_biquad;    //lowpass 2
	tbiquadFIR lp3_biquadFIR;
	tbiquad hp1_biquad;    //highpass 1
	tbiquad hp2_biquad;    //highpass 2
	char count;

	long double temp_boxAccentFrq;
	long double temp_boxAccentEnv;


	char gateStat;   // Current status of gate

	// freq stuff
	long double boxGate;


	long double boxMEG; // main envelope generator
	long double boxDecay;
	long double boxVolume;
	long double boxRezo;
	char boxAccent;
	long double boxAccentDecay;
	long double boxSlide;
	char boxLedError;


	long double boxVolEnv;
	long double boxAccentEnv;
	long double boxAccentFrq;

	long double boxOsc; //oscilator to generate sound;
	long double boxOscFrq; //oscilator frequency

	bool bPlaying;
	int iProcessCounter;
	float _fInternalClock;
	bool _bPlayingFromStart;

	int iCurStep;
	//int iCurPatternXX;
	//int iCurBank;
	//int iCurPreset;

	int iNextPatternXX;

	int iTemp;

	// note/octave editing
	int iStepEdit;
	int iCurPitch;
	int iCurNote;
	int iCurOctave;

	float fGlobalDebug1;
	float fGlobalDebug2;

	int iGlobalDebug1;
	int iGlobalDebug2;

	bool bGlobalDebug1;

	int iDebugPreviousStep;
	int iCurPosLast;
	int iDebugPattern;

#ifdef _CYCLE_
	int iCycleStart;
	int iCycleFinish;
	bool bCycle;
#endif

	double dPPQPlayed;
	double dSavedTempo;

	bool	bNextPatternLock; // can't change next pattern when this is true

	bool	bFirstTimeDebug;

	int iStepEditMode; // 0 = step select, 1 = last step select

	// NEW SEQS STUFF
	int currentGroup;
	int currentPattern;
	int currentStep;
	int currentPage;

	bool bLastStepLock;		// lock changing patterns when we hit the last step in a pattern
	int nextGroup;
	int nextPattern;
	int nextStep;

	int buttonGroup;	// what the UI is
	int buttonPattern;	// what the UI is

//	int songTempo;
//	long timeSigNumerator;		// time signature
//	long timeSigDenominator;

	t_CB_Step _currStep;
	t_CB_Step _nextStep;
};

#endif
