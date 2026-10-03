#include <stdio.h>
#include <math.h>

#ifndef __SILVERBOX__
#include "SilverBox.h"
#endif

#ifndef __SILVERBOX_EDITOR__
#include "SilverBoxEditor.h"
#endif

#include "AEffEditor.hpp"
#if WIN32
#include <mmsystem.h>
#endif
#include <time.h>

#if MACX
#define powf pow
#endif

const double midiScaler = (1. / 127.);

#define ROOT_KEY 60

// C-0 = 32.7, C-1 = 65.4

long double cNoteFreq[61] = { 0,
//	0032.703, 0034.648, 0036.708, 0038.891, 0041.203, 0043.654, 0046.249, 0048.999, 0051.913, 0055.000, 0058.270, 0061.735,
//	0065.406, 0069.296, 0073.416, 0077.782, 0082.407, 0087.307, 0092.499, 0097.999, 0103.826, 0110.000, 0116.541, 0123.471,
	0130.813, 0138.591, 0146.832, 0155.563, 0164.814, 0174.614, 0184.997, 0195.998, 0207.652, 0220.000, 0233.082, 0246.942,
	0261.626, 0277.183, 0293.665, 0311.127, 0329.628, 0349.228, 0369.994, 0391.995, 0415.305, 0440.000, 0466.164, 0493.883,
	0523.251, 0554.365, 0587.330, 0622.254, 0659.255, 0698.456, 0739.989, 0783.991, 0830.609, 0880.000, 0932.328, 0987.767,
	1046.502, 1108.731, 1174.659, 1244.508, 1318.510, 1396.913, 1479.978, 1567.982, 1661.219, 1760.000, 1864.655, 1975.533,
	2093.005, 2217.461, 2349.318, 2389.016, 2637.020, 2793.826, 2959.955, 3135.963, 3322.438, 3520.000, 3729.310, 3951.066,
//	0032.703, 0034.648, 0036.708, 0038.891, 0041.203, 0043.654, 0046.249, 0048.999, 0051.913, 0055.000, 0058.270, 0061.735 // temp
};

char * cNoteDesc[61] = { "   ",
	"C-0","C#0","D-0","D#0","E-0","F-0","F#0","G-0","G#0","A-0","A#0","B-0", //1
	"C-1","C#1","D-1","D#1","E-1","F-1","F#1","G-1","G#1","A-1","A#1","B-1", //13
	"C-2","C#2","D-2","D#2","E-2","F-2","F#2","G-2","G#2","A-2","A#2","B-2", //25
	"C-3","C#3","D-3","D#3","E-3","F-3","F#3","G-3","G#3","A-3","A#3","B-3",
	"C-4","C#4","D-4","D#4","E-4","F-4","F#4","G-4","G#4","A-4","A#4","B-4"
//	"C-5","C#5","D-5","D#5","E-5","F-5","F#5","G-5","G#5","A-5","A#5","B-5"
};

//int cKeyb[12] = {81,50,87,51,69,82,53,84,54,89,55,85};

// Hacks by wwelti: ***************************************************************************************************************************** 
bool wwelti_algorithm = true;

// Here a table for 2^x function, value range -8 to +8

#define FUNTAB2_RANGE 16       // ranges from -8 to +8, that's a total range of 16
#define FUNTAB2_FIRSTVAL -8   // start value
#define FUNTAB2_ACCURACITY 32 // This table works with 1/32 steps. For Frequency stuff we want to be not too unprecise.
#define FUNTAB2_SIZE (FUNTAB2_RANGE*FUNTAB2_ACCURACITY)

double funTab2[FUNTAB2_SIZE + 1];

void initFunTab2()
{
  int i;
  for (i=0; i<FUNTAB2_SIZE+1; i++)
  {
    double v = double(i)/FUNTAB2_ACCURACITY + FUNTAB2_FIRSTVAL;
    funTab2[i] = powf(2.f,v);
  }
}

inline double getTab2Interpolated(double val)   // By wwelti. Read the table tab2, linear interpolated.
{
  double tabbed_val = (val-FUNTAB2_FIRSTVAL)*FUNTAB2_ACCURACITY;    // 1x fsub, 1xfmul
  int i = int(tabbed_val);                                          // 1x fist
  double rest = tabbed_val - i;                                    // 1x fild, 1xfsub

  double t0 = funTab2 [i];                       // 1x fld     
  double t1 = funTab2 [i+1];                     // 1x fld
  return t0 + (t1 - t0) * rest;                  // 1x fsub, 1x fmul, 1x fadd
}


// End of Haxxoring-Section from wwelti **********************************************************************************************************



//-----------------------------------------------------------------------------------------
// SilverBoxProgram
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
SilverBoxProgram::SilverBoxProgram ()
{
//	fWaveform1 = 0.f;	// saw
//	fFreq1 =.0f;
//	fVolume1 = .33f;
//	fWaveform2 = 1.f;	// pulse
//	fFreq2 = .01f;		// slightly higher
//	fVolume2 = .33f;
	m_fVolume = 1.0f;
	strcpy (name, "Basic");
}

//-----------------------------------------------------------------------------------------
// SilverBox
//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
SilverBox::SilverBox (audioMasterCallback audioMaster) : AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{

	//programs = new SilverBoxProgram[kNumPrograms];
	//if (programs)
		//setProgram (0);
	/* - from antigate
			setNumInputs(2);		// stereo in
		setNumOutputs(2);		// stereo out
		canMono();				// makes sense to feed both inputs with the same signal
		canProcessReplacing();	// supports both accumulating and replacing output
		noTail();	// there is no audio output when the audio input is silence
		setUniqueID('BAG0');	// identify

		replacingInputs1 = new float[16384];
		replacingInputs2 = new float[16384];

		strcpy(programName, "default");
	*/
	if (audioMaster)
	{
		currentGroup = 0;
		currentPattern = 0;
		currentPage = 0;
		currentStep = -1;

		bLastStepLock = false;
		nextGroup = 0;
		nextPattern = 0;
		nextStep = 0;

		buttonGroup = 0;
		buttonPattern = 0;

		// initialize pattern names
		char holder[256];
		for (int p=0; p<kNumPrograms; p++)
		{
			sprintf(holder, "pattern%00d",p);
			strcpy(Bank.Name[p], holder);
		}

		iStepEditMode = kEditStep;
		currentPage = 0;

#ifdef _FX_VERSION_
		setNumInputs(2);
//		setUniqueID ('BD3F');			// <<<! *must* change this!!!!
#else
		setNumInputs (0);				// no inputs
		isSynth ();
#endif
#ifdef _LIMITED_
		setUniqueID ('cblx');
#else
		setUniqueID ('cbox');			// <<<! *must* change this!!!!
#endif

		setNumOutputs (kNumOutputs);	// 2 outputs, 1 for each osciallator
		canProcessReplacing ();
		hasVu (false);
		hasClip (false);
		needIdle();
		noTail();
#ifdef _FILE_CHUNK_
		programsAreChunks();
#endif

		// SCRATCHA
		strcpy(m_szProgramName, "default");

		//_nCurrPart = kPartSelect;

		_nCurrPart = 0;

		_nTempo = 120;

		// set the ui display
	//	SetValueMode(true);
		//SetPartMode(kPartSelect);
		SetCurrentPart(0);
		//SetEditMode(kEditPitch);
		// set pattern - maybe set when part is set
		// set current step to 0 - maybe set when pattern is changes

		//??selectpart(kPart1);

		//memset((void *)&m_Part, 0, sizeof(m_Part)*8);
		//memset((void *)&m_Bank, 0, sizeof(m_Bank));

		replacingInputs1 = new float[16384];
		replacingInputs2 = new float[16384];

		samples_per_beat=0; 
		minibeat=0; 
		oldppq=-1; 
		nevercalled=true; 
		forceresynch=true; 
		restart_gate=true;
		oldtempo=-1; 
	
		gate_samplestillfinished=0;
		gate_samplestillgatefinished=0;
		gate_gain=0;
		gate_vector=0;

		resynchgate=0;

		m_fRunStop = 0.0f;
		m_fSync = 1.0f;

		CBConstruct();
		CBInit("c:\\code\\cbx\\debug\\testes.cb");

		openlog();
		for (int x=0; x<16; x++)
		{
			writelog("step[%d] %d %d %d\n", x, Bank.tPattern[0].Step[x].cPitch,
												Bank.tPattern[0].Step[x].bSlide,
												Bank.tPattern[0].Step[x].bAccent);
		}

		bPlaying = false;
		iProcessCounter=0;
		_bPlayingFromStart = false;
		iCurStep = 0;
//		iCurPatternXX = 00;
//		iCurBank = 0;
//		iCurPreset = 0;
		iStepEdit = 0;

		// make sure these match the first note of the default pattern
		iCurPitch = 13;
		iCurNote = 0;
		iCurOctave = 1;

		//SetStepPitch(0,Bank.tPattern[m_Part[0].cPattern].Step[0].cPitch);
		iGlobalDebug1 = 0;
		iDebugPattern = 0;
#ifdef _CYCLE_
		iCycleStart = 0;
		iCycleFinish = 0;
		bCycle = false;
#endif
//		bMidiLearn = false;
		dPPQPlayed = 0.0;
		oldSampleFrames = 0;
		dSavedTempo = 0;

		bNextPatternLock = false;
		bFirstTimeDebug = true;

    // hacking by wwelti: Need those tables initialized!
    initFunTab1();
    initFunTab2();
	}
	initProcess ();
	suspend ();
}

//-----------------------------------------------------------------------------------------
SilverBox::~SilverBox ()
{
	for (int x=0; x<16; x++)
		{
			writelog("step[%d] %d %d %d\n", x, Bank.tPattern[0].Step[x].cPitch,
												Bank.tPattern[0].Step[x].bSlide,
												Bank.tPattern[0].Step[x].bAccent);
		}

	closelog();
	CBDestroy();

	if (replacingInputs1)
		delete[] replacingInputs1;
	if (replacingInputs2)
		delete[] replacingInputs2;


//	if (programs)
//		delete[] programs;
}
//-----------------------------------------------------------------------------------------
long SilverBox::getProgram()
{
	return currentPattern;
}


//-----------------------------------------------------------------------------------------
/*
void SilverBox::SetBank(int bank)
{
	writelog("SetBank(%d)\n", bank);
	int next_bank, next_preset, next_program, new_program;

	next_program = m_Part[0].cNextPattern;

	next_bank = next_program/10; // 0-9
	next_preset = next_program - (next_bank*10); // 0-9

	new_program = (bank*10) + next_preset;

	setProgram(new_program);
}

//-----------------------------------------------------------------------------------------
void SilverBox::SetPreset(int preset)
{
	writelog("SetPreset(%d)\n", preset);
	int next_bank, next_preset, next_program, new_program;

	next_program = m_Part[0].cNextPattern;

	next_bank = next_program/10; // 0-9
	next_preset = next_program - (next_bank*10); // 0-9

	new_program = (next_bank*10) + preset;

	setProgram(new_program);
}
*/
void	SilverBox::setCurrentGroup(int bank)
{
	int oldBank = buttonGroup;
	buttonGroup = bank;
	if (!bPlaying)
		currentGroup = buttonGroup;
	if (editor) ((SilverBoxEditor*)editor)->setParameter(kBank00 + oldBank, 0.0);
	if (editor) ((SilverBoxEditor*)editor)->setParameter(kBank00 + buttonGroup, 1.0);
	refreshSequencer();
	RefreshPresetDisplay();
}

void	SilverBox::setCurrentPattern(int pattern)
{
	int oldPattern = buttonPattern;
	buttonPattern = pattern;
	if (!bPlaying)
		currentPattern = buttonPattern;
	if (editor) ((SilverBoxEditor*)editor)->setParameter(kPreset00 + oldPattern, 0.0); // the 0.0 and 1.0 values are used
	if (editor) ((SilverBoxEditor*)editor)->setParameter(kPreset00 + buttonPattern, 1.0);
	refreshSequencer();
	RefreshPresetDisplay();
}
//-----------------------------------------------------------------------------------------
#ifndef _LIMITED_
void SilverBox::setProgram (long program)
{
	/*
	// assume that program goes from 0-99;
	writelog("setProgram(%d)\n", program);
	if (program > 99) return; // we ignore invalid programs
	int new_bank, new_preset;
	int next_program, next_bank, next_preset;
	
	new_bank = program/10; // 0-9
	new_preset = program - (new_bank*10); // 0-9

	next_program = m_Part[0].cNextPattern;
	next_bank = next_program/10; // 0-9
	next_preset = next_program - (next_bank*10); // 0-9

	// program = (bank*8) + preset
	writelog("new program %d bank %d preset %d\n", program, new_bank, new_preset);
	writelog("next program %d bank %d preset %d\n", next_program, next_bank, next_preset);

	if (bNextPatternLock) // cannot set next pattern.. unpress any buttons
	{

		if (bPlaying)
		{			
			if (new_bank != next_bank)
			{
				((SilverBoxEditor*)editor)->setParameter(kBank00 + new_bank, 0.0f); // turn off new bank
				((SilverBoxEditor*)editor)->setParameter(kBank00 + next_bank, 1.0f); // turn on old bank
			}

			if (new_preset != next_preset)
			{
				((SilverBoxEditor*)editor)->setParameter(kPreset00 + new_preset, 0.0f); // turn off new preset
				((SilverBoxEditor*)editor)->setParameter(kPreset00 + next_preset, 1.0f); // turn on old preset
			}

			if (editor)
			{
				editor->postUpdate();
			}

			updateDisplay();

			return;
		}
	}


	SetNextPattern(program);
//	m_Part[0].fNextPattern = program/127.f;
	if (editor)
	{
		if (new_bank != next_bank)
		{
			((SilverBoxEditor*)editor)->setParameter(kBank00 + next_bank, 0.0f); // turn off old bank
			((SilverBoxEditor*)editor)->setParameter(kBank00 + new_bank, 1.0f); // turn on new bank
		}

		if (new_preset != next_preset)
		{
			((SilverBoxEditor*)editor)->setParameter(kPreset00 + next_preset, 0.0f); // turn off old preset
			((SilverBoxEditor*)editor)->setParameter(kPreset00 + new_preset, 1.0f); // turn on new preset
		}
//		((SilverBoxEditor*)editor)->setParameter(kNextPattern, m_Part[0].fNextPattern);
	}
	if (!bPlaying) 
	{
		SetCurrentPattern(m_Part[0].cNextPattern);
	//	m_Part[0].fPattern = m_Part[0].fNextPattern;
		m_Part[0].cPattern = m_Part[0].cNextPattern;
		RefreshPatternDisplay();
		SetStepEdit(iStepEdit);
	}
	*/
}
#endif
//-----------------------------------------------------------------------------------------
void SilverBox::setProgramName (char *name)
{
	strcpy(Bank.Name[currentPattern], name);
	//strcpy(m_szProgramName, name);
}

//-----------------------------------------------------------------------------------------
void SilverBox::getProgramName (char *name)
{
	strcpy(name, Bank.Name[currentPattern]);
//	strcpy(name, m_szProgramName);
}

//-----------------------------------------------------------------------------------------
void SilverBox::getParameterLabel (long index, char *label)
{
	/*
	switch (index)
	{
		case kPitchAmount:		strcpy (label, "  %  ");		break;
		case kPitchRange:		strcpy (label, "  %  ");		break;
		case kToneArmWeight:	strcpy (label, "grams");		break;
		case kVolume:			strcpy (label, "  dB  ");		break;
		default:
			strcpy(label, " "); break;
	}
	*/
}


//-----------------------------------------------------------------------------------------
void SilverBox::getParameterDisplay (long index, char *text)
{
	char ctemp[256];
	text[0] = 0;

	switch (index)
	{
#ifndef _LIMITED_
		case kCurrentPatternDisplay:
			{
				sprintf(ctemp, "%02d", (currentGroup*10) + currentPattern);
				strcpy(text, ctemp);
			}
			break;

		case kNextPatternDisplay:
			{
				sprintf(ctemp, "%02d", (buttonGroup*10) + buttonPattern);
				strcpy(text, ctemp);
			}
			break;
#ifdef _CYCLE_
		case kStartDisplay:
			{
				sprintf(ctemp, "%02d", iCycleStart);
				strcpy(text, ctemp);
			}
			break;

		case kFinishDisplay:
			{
				sprintf(ctemp, "%02d", iCycleFinish);
				strcpy(text, ctemp);
			}
			break;
#endif
#endif
	}

}




//-----------------------------------------------------------------------------------------
void SilverBox::getParameterName (long index, char *label)
{
	switch (index)
	{
		case kVolume:			strcpy (label, "Volume");		break;
		case kWaveform:			strcpy (label, "Waveform");		break;
		case kTuning:			strcpy (label, "Tuning");		break;
		case kCutOffFreq:		strcpy (label, "CutOff");		break;
		case kResonance:		strcpy (label, "Resonance");		break;
		case kEnvMod:			strcpy (label, "EnvMod");		break;
		case kDecay:			strcpy (label, "Decay");		break;
		case kAccent:			strcpy (label, "Accent");		break;
		default:
			strcpy (label, "invalid");		break;
	}

}

void SilverBox::setParameterInt(int index, int value)
{
	switch(index)
	{
	case kPitch01:
	case kPitch02:
	case kPitch03:
	case kPitch04:
	case kPitch05:
	case kPitch06:
	case kPitch07:
	case kPitch08:
	case kPitch09:
	case kPitch10:
	case kPitch11:
	case kPitch12:
	case kPitch13:
	case kPitch14:
	case kPitch15:
	case kPitch16:
		{
			int step = index - kPitch01;
			int currpattern = currentPattern;
			iTemp = value;
			char pitch = value;
			if (pitch < 0) pitch = 0;
			if (pitch > 60) pitch = 60;
			SetStepPitch(step + (currentPage*16), pitch);
			Bank.tPattern[currpattern].Step[step + (currentPage*16)].cPitch = pitch;
			// remember to update the keyboard
		}
		break;
	}
}
//-----------------------------------------------------------------------------------------
void SilverBox::SetStepEdit(int step)
{
	SetStepPitch(step, Bank.tPattern[currentPattern].Step[step].cPitch);
}

//-----------------------------------------------------------------------------------------
void SilverBox::SetStepPitch(int step, int pitch)
{
	int cur_pitch, cur_note, cur_octave;
	int new_pitch, new_note;
	int new_octave = 0;

	cur_pitch = iCurPitch; //Bank.tPattern[m_Part[_nCurrPart].cPattern].Step[iStepEdit].cPitch;
	if (cur_pitch == 0)
	{
		cur_note = -1;
		cur_octave = -1;
	}
	else
	{
		cur_octave = (cur_pitch-1)/12; // 0-4
		cur_note = (cur_pitch-1) - (cur_octave*12); // 0-11	
	}

	iStepEdit = step;

	new_pitch = pitch;
	if (new_pitch == 0)
	{
		new_note = -1;
		cur_octave = -1;
	}
	else
	{
		new_octave = (new_pitch-1)/12; // 0-4
		new_note = (new_pitch-1) - (new_octave*12); // 0-11	
	}

	iCurPitch = new_pitch;
	iCurNote = new_note;
	iCurOctave = new_octave;

	refreshStepEdit();
}

//-----------------------------------------------------------------------------------------
/*
*/

void SilverBox::setParameter(long index, float value)
{
	
	//value = FixMidiData(index, value);
	
	//SilverBoxProgram *ap = &programs[curProgram];
	switch (index)
	{
	case kVolume:		m_Part[_nCurrPart].fVolume = value;	break;
	case kWaveform:		m_Part[_nCurrPart].fWaveform = value;	break;
	case kTuning:		m_Part[_nCurrPart].fTuning = value;	break;
	case kCutOffFreq:	m_Part[_nCurrPart].fCutOffFreq = value;	break;
	case kResonance:	m_Part[_nCurrPart].fResonance = value;	break;
	case kEnvMod:		m_Part[_nCurrPart].fEnvMod = value;	break;
	case kDecay:		m_Part[_nCurrPart].fDecay = value;	break;
	case kAccent:		m_Part[_nCurrPart].fAccent = value;	break;

	case kTempo:
		{
			//value = 0.25f;
			_nTempo = ((int)(value * 260.0f)) + 40; // 40 to 300 bpm
			if (editor)
			{
				((SilverBoxEditor*)editor)->setParameter(kTempo, value);
			}
		}
		break;

#ifndef _LIMITED_
	case kClear:	PatternClear(); break;
	case kCopy:		PatternCopy(); break;
	case kPaste:	PatternPaste(); break;
#ifdef _RANDOM_PATTERNS_
	case kRandom1:	PatternRandom1(); break;
	case kRandom2:	PatternRandom2(); break;
#endif
#endif
	case kAccent01:
	case kAccent02:
	case kAccent03:
	case kAccent04:
	case kAccent05:
	case kAccent06:
	case kAccent07:
	case kAccent08:
	case kAccent09:
	case kAccent10:
	case kAccent11:
	case kAccent12:
	case kAccent13:
	case kAccent14:
	case kAccent15:
	case kAccent16:
		{
			int currpattern = currentPattern;
			int step = index - kAccent01;
			if (value == 1.0f)
				Bank.tPattern[currpattern].Step[step + (currentPage*16)].bAccent = 1;
			else
				Bank.tPattern[currpattern].Step[step + (currentPage*16)].bAccent = 0;
		}
		break;

	case kSlide01:
	case kSlide02:
	case kSlide03:
	case kSlide04:
	case kSlide05:
	case kSlide06:
	case kSlide07:
	case kSlide08:
	case kSlide09:
	case kSlide10:
	case kSlide11:
	case kSlide12:
	case kSlide13:
	case kSlide14:
	case kSlide15:
	case kSlide16:
		{
			int currpattern = currentPattern;
			int step = index - kSlide01;
			if (value == 1.0f)
				Bank.tPattern[currpattern].Step[step + (currentPage*16)].bSlide = 1;
			else
				Bank.tPattern[currpattern].Step[step + (currentPage*16)].bSlide = 0;
		}
		break;

	case kRunStop:	
		{
			m_fRunStop = value; 
			if (m_fRunStop == 1.0) // run
			{
				if (m_fSync == 1.0) // can run since we are syncing to host
				{
					m_fSync = 0.0;
					if (editor)	((SilverBoxEditor*)editor)->setParameter(kSync, 0.0f);
					//m_fRunStop = 0.0;
					//if (editor)	((SilverBoxEditor*)editor)->setParameter(kRunStop, 0.0f);
					_bPlayingFromStart = false;
					_fInternalClock = 0.0f;
				//	_bInternalClockStart = true;
				}
			}
			else // stop
			{
				_fInternalClock = 0.0f;
			}
		}
		break;
	case kSync:		
		{
			m_fSync = value; 
			if (m_fSync == 1.0) // sync
			{
				if (m_fRunStop == 1.0) // turn off internal clock
				{
					m_fRunStop = 0.0;
					_fInternalClock = 0.0f;
					_bPlayingFromStart = false;
					if (editor)	((SilverBoxEditor*)editor)->setParameter(kRunStop, 0.0f);
				}
			}
			else // stop syncing
			{
			}
		}
		break;
#ifndef _LIMITED_
	case kBank00:
	case kBank01:
	case kBank02:
	case kBank03:
	case kBank04:
	case kBank05:
	case kBank06:
	case kBank07:
	case kBank08:
	case kBank09:
		{
			int bank = index - kBank00;
			setCurrentGroup(bank);
		}
		break;

	case kPreset00:
	case kPreset01:
	case kPreset02:
	case kPreset03:
	case kPreset04:
	case kPreset05:
	case kPreset06:
	case kPreset07:
	case kPreset08:
	case kPreset09:
		{
			int preset = index - kPreset00;
			setCurrentPattern(preset);
		}
		break;
#endif
	case kNoteOff:
		{
			SetStepPitch(iStepEdit, 0);
			Bank.tPattern[currentPattern].Step[iStepEdit + (currentPage*16)].cPitch = 0;
			int step = iStepEdit%16;
			setParameter(kPitch01+step, 0.0f);
		}
		break;
	case kNote01:
	case kNote02:
	case kNote03:
	case kNote04:
	case kNote05:
	case kNote06:
	case kNote07:
	case kNote08:
	case kNote09:
	case kNote10:
	case kNote11:
	case kNote12:
		{
			int cur_pitch, cur_note, cur_octave, new_pitch;
			int new_note = index - kNote01;

			cur_pitch = iCurPitch; //Bank.tPattern[m_Part[_nCurrPart].cPattern].Step[iStepEdit].cPitch;
			if (cur_pitch == 0)
			{
				cur_octave = 0;
				cur_note = new_note;
			}
			else
			{
				cur_octave = (cur_pitch-1)/12; // 0-4
				cur_note = (cur_pitch-1) - (cur_octave*12); // 0-11	
			}

			new_pitch = (cur_octave*12)+new_note+1;

			SetStepPitch(iStepEdit, new_pitch);
			Bank.tPattern[currentPattern].Step[iStepEdit].cPitch = new_pitch;
			int step = iStepEdit%16;
			setParameter(kPitch01+step, ((float)new_pitch)/60.f);			
		}
		break;
	case kOctave01:
	case kOctave02:
	case kOctave03:
	case kOctave04:
	case kOctave05:
		{
			int cur_pitch, cur_note, cur_octave, new_pitch;
			int new_octave = index - kOctave01;

			cur_pitch = iCurPitch; //Bank.tPattern[m_Part[_nCurrPart].cPattern].Step[iStepEdit].cPitch;
			if (cur_pitch == 0)
			{
				cur_octave = new_octave;
				cur_note = 0;
			}
			else
			{
				cur_octave = (cur_pitch-1)/12; // 0-4
				cur_note = (cur_pitch-1) - (cur_octave*12); // 0-11	
			}


			new_pitch = (new_octave*12)+cur_note+1;

			SetStepPitch(iStepEdit, new_pitch);
			Bank.tPattern[currentPattern].Step[iStepEdit].cPitch = new_pitch;
			int step = iStepEdit%16;
			setParameter(kPitch01+step, ((float)new_pitch)/60.f);
		}
		break;

	case kStepEdit01:
	case kStepEdit02:
	case kStepEdit03:
	case kStepEdit04:
	case kStepEdit05:
	case kStepEdit06:
	case kStepEdit07:
	case kStepEdit08:
	case kStepEdit09:
	case kStepEdit10:
	case kStepEdit11:
	case kStepEdit12:
	case kStepEdit13:
	case kStepEdit14:
	case kStepEdit15:
	case kStepEdit16:
		{
			int step = index - kStepEdit01;
			if (iStepEditMode == kEditStep)
				SetStepEdit(step + (currentPage*16));
			else
				setLastStep(step + (currentPage*16));
		}
		break;
	case kStepEditMode:
		{
			if (value == 1.0)	setStepEditMode(kLastStep);
			else				setStepEditMode(kEditStep);
		}
		break;
	case kPage00:
	case kPage01:
	case kPage02:
	case kPage03:
		{
			int page = index - kPage00;
			setCurrentPage(page);
		}
		break;
#ifdef _MIDI_LEARN_
	case kMidiLearn:
		{
			if (value == 1.0)	bMidiLearn = true;
			else				bMidiLearn = false;
		}
		break;
#endif
#ifndef _LIMITED_
#ifdef _CYCLE_
	case kCycle:
		{
			if (value == 1.0)	bCycle = true;
			else				bCycle = false;
		}
		break;

	case kSetCycleStart:
		{
			iCycleStart = iCurPatternXX;
			if (editor)	((SilverBoxEditor*)editor)->setParameter(kStartDisplay, (float)iCycleStart);
		}
		break;
	case kSetCycleFinish:
		{
			iCycleFinish= iCurPatternXX;
			if (editor)	((SilverBoxEditor*)editor)->setParameter(kFinishDisplay, (float)iCycleFinish);
		}
		break;
#endif
#endif
	}


	if (editor)
	{
//		((SilverBoxEditor*)editor)->setParameter(index, value);
		editor->postUpdate();
	}

	updateDisplay();
}

//-----------------------------------------------------------------------------------------
float SilverBox::getParameter (long index)
{
	float value = 0.0f;
	
	switch (index)
	{
		case kVolume:		value = m_Part[_nCurrPart].fVolume;	break;
		case kWaveform:		value = m_Part[_nCurrPart].fWaveform;	break;
		case kTuning:		value = m_Part[_nCurrPart].fTuning;	break;
		case kCutOffFreq:	value = m_Part[_nCurrPart].fCutOffFreq;	break;
		case kResonance:	value = m_Part[_nCurrPart].fResonance;	break;
		case kEnvMod:		value = m_Part[_nCurrPart].fEnvMod;	break;
		case kDecay:		value = m_Part[_nCurrPart].fDecay;	break;
		case kAccent:		value = m_Part[_nCurrPart].fAccent;	break;

		case kTempo:			value = ((float)(_nTempo - 40)/260.0f); break;
#ifndef _LIMITED_			
		case kCurrentPatternDisplay: value = (float)((currentGroup*10)+currentPattern); break; //value = ((float)m_Part[_nCurrPart].cPattern)/127.0f; break;
		case kNextPatternDisplay: value = (float)((buttonGroup*10)+buttonPattern); break;
#ifdef _CYCLE_

		case kStartDisplay: value = (float)iCycleStart; break;
		case kFinishDisplay: value = (float)iCycleFinish; break;
#endif
#endif
		case kTempoDisplay:		value = (float)_nTempo; break;

		case kAccent01:
		case kAccent02:
		case kAccent03:
		case kAccent04:
		case kAccent05:
		case kAccent06:
		case kAccent07:
		case kAccent08:
		case kAccent09:
		case kAccent10:
		case kAccent11:
		case kAccent12:
		case kAccent13:
		case kAccent14:
		case kAccent15:
		case kAccent16:
			{
				int currpattern = currentPattern;
				if (Bank.tPattern[currpattern].Step[(index - kAccent01) + (currentPage*16)].bAccent == 1)
					value = 1.0f;
				else
					value = 0.0f;
			}
			break;

		case kPitch01:
		case kPitch02:
		case kPitch03:
		case kPitch04:
		case kPitch05:
		case kPitch06:
		case kPitch07:
		case kPitch08:
		case kPitch09:
		case kPitch10:
		case kPitch11:
		case kPitch12:
		case kPitch13:
		case kPitch14:
		case kPitch15:
		case kPitch16:
			{
				int currpattern = currentPattern;
				value = ((float)(Bank.tPattern[currpattern].Step[(index - kPitch01) + (currentPage*16)].cPitch))/ 60.0f;
			}
			break;


		case kPitchDisplay01:
		case kPitchDisplay02:
		case kPitchDisplay03:
		case kPitchDisplay04:
		case kPitchDisplay05:
		case kPitchDisplay06:
		case kPitchDisplay07:
		case kPitchDisplay08:
		case kPitchDisplay09:
		case kPitchDisplay10:
		case kPitchDisplay11:
		case kPitchDisplay12:
		case kPitchDisplay13:
		case kPitchDisplay14:
		case kPitchDisplay15:
		case kPitchDisplay16:
			{
				int currpattern = currentPattern;
				value = ((float)(Bank.tPattern[currpattern].Step[(index - kPitch01) + (currentPage*16)].cPitch))/ 60.0f;
			}
			break;

		case kSlide01:
		case kSlide02:
		case kSlide03:
		case kSlide04:
		case kSlide05:
		case kSlide06:
		case kSlide07:
		case kSlide08:
		case kSlide09:
		case kSlide10:
		case kSlide11:
		case kSlide12:
		case kSlide13:
		case kSlide14:
		case kSlide15:
		case kSlide16:
			{
				int currpattern = currentPattern;
				if (Bank.tPattern[currpattern].Step[(index - kSlide01) + (currentPage*16)].bSlide == 1)
					value = 1.0f;
				else
					value = 0.0f;
			}
			break;

		case kRunStop:	value = m_fRunStop; break;
		case kSync:		value = m_fSync; break;

		case kStep01:
		case kStep02:
		case kStep03:
		case kStep04:
		case kStep05:
		case kStep06:
		case kStep07:
		case kStep08:
		case kStep09:
		case kStep10:
		case kStep11:
		case kStep12:
		case kStep13:
		case kStep14:
		case kStep15:
		case kStep16:
			{
				int step = index - kStep01;
				//if (step == iCurStep)
				if (bPlaying)
				{
					if ((step + (currentPage*16)) == currentStep)
						value = 1.0f;
					else 
						value = 0.0f;
				}
				else
					value = 0.0f;
			}
			break;
#ifndef _LIMITED_	
		case kBank00:
		case kBank01:
		case kBank02:
		case kBank03:
		case kBank04:
		case kBank05:
		case kBank06:
		case kBank07:
		case kBank08:
		case kBank09:
			{
				int bank = index - kBank00;
				if (bank == buttonGroup)	value = 1.0f;
				else					value = 0.0f;
			}
			break;

		case kPreset00:
		case kPreset01:
		case kPreset02:
		case kPreset03:
		case kPreset04:
		case kPreset05:
		case kPreset06:
		case kPreset07:
		case kPreset08:
		case kPreset09:
			{
				int preset = index - kPreset00;
				if (preset == buttonPattern)	value = 1.0f;
				else						value = 0.0f;
			}
			break;
#endif
		case kNoteOff:
			{
				if (iCurPitch == 0) value = 1.0f;
				else				value = 0.0f;
			}
			break;
		case kNote01:
		case kNote02:
		case kNote03:
		case kNote04:
		case kNote05:
		case kNote06:
		case kNote07:
		case kNote08:
		case kNote09:
		case kNote10:
		case kNote11:
		case kNote12:
			{
				int note = index - kNote01;
				if (iCurNote == note)	value = 1.0f;
				else				value = 0.0f;			
			}
			break;
		case kOctave01:
		case kOctave02:
		case kOctave03:
		case kOctave04:
		case kOctave05:
			{
				int octave = index - kOctave01;
				if (iCurOctave == octave) value = 1.0f;
				else				 value = 0.0f;
			}
			break;

		case kStepEdit01:
		case kStepEdit02:
		case kStepEdit03:
		case kStepEdit04:
		case kStepEdit05:
		case kStepEdit06:
		case kStepEdit07:
		case kStepEdit08:
		case kStepEdit09:
		case kStepEdit10:
		case kStepEdit11:
		case kStepEdit12:
		case kStepEdit13:
		case kStepEdit14:
		case kStepEdit15:
		case kStepEdit16:
			{
				int step = index - kStepEdit01;
				if (iStepEditMode == kEditStep)
				{
					if ((step + (currentPage*16)) == iStepEdit)
						value = 1.0f;
					else	
						value = 0.0f;
				}
				else // kLastStep
				{
					if ((step + (currentPage*16)) == Bank.tPattern[(currentGroup*10)+currentPattern].LastStep)
						value = 1.0f;
					else
						value = 0.0f;
				}
			}
			break;

		case kStepEditMode:
			{
				if (iStepEditMode == kEditStep) value = 0.0f;
				else					value = 1.0f;
			}
			break;
		case kPage00:
		case kPage01:
		case kPage02:
		case kPage03:
			{
				int page = index - kPage00;
				if (page == currentPage)	value = 1.0f;
				else						value = 0.0f;
			}
			break;
#ifdef _MIDI_LEARN_
		case kMidiLearn:
			{
				if (bMidiLearn)	value = 1.0f;
				else			value = 0.0f;
			}
			break;
#endif
#ifndef _LIMITED_
#ifdef _CYCLE_
		case kCycle:
			{
				if (bCycle)	value = 1.0f;
				else		value = 0.0f;
			}
			break;
#endif
#endif
	}
	return value;
}

//-----------------------------------------------------------------------------------------
bool SilverBox::getOutputProperties (long index, VstPinProperties* properties)
{
	if (index < kNumOutputs)
	{
		sprintf (properties->label, "cblx %1d", index + 1);
		properties->flags = kVstPinIsActive;
		if (index < 2)
			properties->flags |= kVstPinIsStereo;	// test, make channel 1+2 stereo
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------
bool SilverBox::getProgramNameIndexed (long category, long index, char* text)
{
	bool returnCode = false;
	
	if (index < kNumPrograms)    
	{        
		strcpy(text, Bank.Name[index]);       
		returnCode = true;    
	}    
	
	return (returnCode);
}

//-----------------------------------------------------------------------------------------
bool SilverBox::copyProgram (long destination)
{
	return false;
}

//-----------------------------------------------------------------------------------------
bool SilverBox::getEffectName (char* name)
{
#ifdef _LIMITED_
	strcpy (name, "creakbox bassline demo");
#else
	strcpy (name, "creakbox bassline");
#endif
	return true;
}

//-----------------------------------------------------------------------------------------
bool SilverBox::getVendorString (char* text)
{
	strcpy (text, "bioroid");
	return true;
}

//-----------------------------------------------------------------------------------------
bool SilverBox::getProductString (char* text)
{
#ifdef _LIMITED_
	strcpy (text, "bioroid creakbox bassline demo");
#else
	strcpy (text, "bioroid creakbox bassline");
#endif
	return true;
}

//-----------------------------------------------------------------------------------------
long SilverBox::canDo (char* text)
{
	if (!strcmp (text, "receiveVstEvents"))
		return 1;
	if (!strcmp (text, "receiveVstMidiEvent"))
		return 1;
	return -1;	// explicitly can't do; 0 => don't know
}



//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
void SilverBox::setSampleRate (float sampleRate)
{
	AudioEffectX::setSampleRate (sampleRate);
}

//-----------------------------------------------------------------------------------------
void SilverBox::setBlockSize (long blockSize)
{
	AudioEffectX::setBlockSize (blockSize);
	// you may need to have to do something here...
}

//-----------------------------------------------------------------------------------------
void SilverBox::resume ()
{
	wantEvents ();
}

//-----------------------------------------------------------------------------------------
void SilverBox::initProcess ()
{
	noteIsOn = false;
	currentDelta = 0;
	long i;
}

//-----------------------------------------------------------------------------------------
void SilverBox::processMidiEvent(long currEvent)
{
	VstMidiEvent* event = (VstMidiEvent*)m_pVstEvents->events[currEvent];

	char* midiData = event->midiData;
	long status = midiData[0] & 0xf0;		// ignoring channel
	if (status == 0x90 || status == 0x80)	// we only look at notes
	{
		 //ignore note on/off
	}
	else if (status == 0xb0) // controller change
	{
//		if (midiData[1] == 0x7e)	// all notes off
//		{
//			noteIsOn = false;
//		}

		if ((midiData[1]>=64) && (midiData[1]<=64 + kNumParams - 1))
		{
			long param = midiData[1]-64;
			long new_data = FixMidiData(param, midiData[2]);
			float value = ((float)new_data)/127.f;
			setParameterAutomated(param, value);
		}

		/*
007		kVolume,
070		kWaveform,
071		kTuning,
074		kCutOffFreq,
075		kResonance,
076		kEnvMod,
077		kDecay,
078		kAccent,
		*/

	}
#ifndef _LIMITED_
	else if (status == 0xc0) // program change
	{
		char newprog = midiData[1];
		setProgram(newprog);
	}
#endif
}


//-----------------------------------------------------------------------------------------
//logic
void SilverBox::process(float **inputs, float **outputs, long sampleFrames)
{
	doTheProcess(inputs[0], inputs[1], outputs, sampleFrames);
}

//-----------------------------------------------------------------------------------------
void SilverBox::processReplacing(float **inputs, float **outputs, long sampleFrames)
{	
	// copy the inputs array because Cubase shares the same pointer for inputs & outputs
//	memcpy(replacingInputs1, inputs[0], sampleFrames * sizeof(float));
//	memcpy(replacingInputs2, inputs[1], sampleFrames * sizeof(float));

	// whipe out what's in the output array ...
//	memset(outputs[0], 0, sampleFrames * sizeof(float));
//	memset(outputs[1], 0, sampleFrames * sizeof(float));

	// ... & then call the regular process function
	doTheProcess(replacingInputs1, replacingInputs2, outputs, sampleFrames);
}

//-----------------------------------------------------------------------------------------
void SilverBox::doTheProcess(float *inputs1, float *inputs2, float **outputs, long sampleFrames) 
{
//     float *in1  =  inputs1;  
//     float *in2  =  inputs2;  
	float *out1 = outputs[0];  
	float *out2 = outputs[1]; 
	int iTempDebug;
	bool bNoSync = false;

	iProcessCounter++;

	m_bProcessing = true;

	VstTimeInfo *timecode;  
	timecode=getTimeInfo(kVstTempoValid | kVstPpqPosValid | kVstTimeSigValid | kVstBarsValid);  
	//timecode->barStartPos = floor(timecode->barStartPos); // fix for cubase sx/nuendo

	//timecode->barStartPos = (float)((int)(timecode->ppqPos/4.0)) * 4.0;
	
	oldSampleFrames = sampleFrames;
	dSavedTempo = timecode->tempo;

	dPPQPlayed = (timecode->tempo/60.0)*((double)sampleFrames/timecode->sampleRate);
	//float fRem = fmod(timecode->ppqPos, 4.0);

	bGlobalDebug1 = bPlaying;
	bPlaying = true;

	/*
	kVstTransportChanged 		= 1,		// Indicates that Playing, Cycle or Recording has changed
	kVstTransportPlaying 		= 1 << 1,
	kVstTransportCycleActive	= 1 << 2,
	kVstTransportRecording		= 1 << 3,
		*/
	/*
	if ((m_fSync == 1.0))// && ((timecode->flags & kVstTransportPlaying)||(timecode->flags & kVstTransportChanged)))
	{
	//	if (bFirstTimeDebug)
	//	{
			writelog("================== sampleFrames: %ld\n", sampleFrames);
			if (timecode->flags & kVstTransportChanged)
				writelog("************* kVstTransportChanged\n");
			if (timecode->flags & kVstTransportPlaying)
				writelog("************* kVstTransportPlaying\n");
			if (timecode->flags & kVstTransportCycleActive)
				writelog("************* kVstTransportCycleActive\n");
			dumpVstTimeInfo(*timecode);
	//		bFirstTimeDebug = false;
	//	}
	}
	*/

#define CBX_CLEAR_BUFFER memset(out1, 0, sizeof(float) * sampleFrames); memset(out2, 0, sizeof(float) * sampleFrames); 
	if (timecode->ppqPos == 0 || oldppq==timecode->ppqPos)   // used to be ||
	{  
		oldppq = timecode->ppqPos;
	//	currFrame = 0; // reset
		if (m_fSync == 1.0) // we are syncing and the host aint playing so exit
		{
			if (!(timecode->flags & kVstTransportPlaying))
			{
				//dPPQPlayed = 0;
				bPlaying = false;
				_bPlayingFromStart = false;
				CBX_CLEAR_BUFFER
				return;  
			}
		}
	}  

	if (m_fSync == 1.0) // syncing to host
	{
		// check if transport has changed from FFW or REW
		//oldSampleFrames
		/*	else
	{
				temp = (_tInfo.ppqPos - _tInfo.barStartPos)* 4.0f;  // this has the [step].[position in step]

		samples_per_beat=(_tInfo.sampleRate*60.0)/(float)(int)(_tInfo.tempo);     // samplesperbeat=(samples_per_minute / beats_per_minute) 
		fSamplesPerStep = (_tInfo.sampleRate*15.0)/(float)(int)(_tInfo.tempo);
		eSBN = (int)fSamplesPerStep;

		//currFrame = fSamplesPerStep * temp;

		iCurPos = fmod(temp, 1)*fSamplesPerStep;//(float)eSBN;
		current_step = temp;
	 }
	
		double fppqPlayed = (double)sampleFrames/fSamplesPerStep;
		dLastPPQPosEnd = _tInfo.ppqPos + fppqPlayed;
	}
		*/
		
		//double dDiff;
	/*
		if ((oldppq + dPPQPlayed) != timecode->ppqPos)
		{
			oldppq = timecode->ppqPos;
			//dPPQPlayed = 0;
			bPlaying = false;
			_bPlayingFromStart = false;
			return; 
		}
	*/
/*
		if (oldppq > timecode->ppqPos) // transport has rewound
		{
			oldppq = timecode->ppqPos;
			bPlaying = false;
			_bPlayingFromStart = false;
			return; 
		}
*/
		oldppq = timecode->ppqPos;

		sampleRate = timecode->sampleRate;
//		eSBN = timecode->sampleRate/(((timecode->tempo/4.f)*16.f)/60.f);
		eSBN = timecode->sampleRate/(((timecode->tempo/timecode->timeSigNumerator)*(timecode->timeSigDenominator*timecode->timeSigNumerator))/60.);
		boxBPM = timecode->tempo;

#ifndef	_FX_VERSION_
		if (!(timecode->flags & kVstTransportPlaying))
		{
			bPlaying = false;
			_bPlayingFromStart = false;
			//dPPQPlayed = 0;
			CBX_CLEAR_BUFFER
			return;  
		}
#endif

		// if the timing is x/2, x/4, x/8, x/16 then we dont sync
		//if (!((timecode->timeSigDenominator==2)||(timecode->timeSigDenominator==4)||(timecode->timeSigDenominator==8)||(timecode->timeSigDenominator==16)))

		// we only support 4/4 timing for the 1.0 release
		if (!((timecode->timeSigNumerator==4)&&(timecode->timeSigDenominator==4)))
		{
			bPlaying = false;
			_bPlayingFromStart = false;
			//dPPQPlayed = 0;
			CBX_CLEAR_BUFFER
			return;  
		}


		if (bNoSync)
		{
			//bPlaying = false;
			_bPlayingFromStart = false;
			//return;
		}
/*
		if ((dLastPPQPosEnd != 0.0) && (dLastPPQPosEnd != timecode->ppqPos))
		{
			dLastPPQPosEnd = 0.0;
			bPlaying = false;
			_bPlayingFromStart = false;
			return; 
		}
*/
	 }
	 else  if(m_fRunStop == 1.0) // running at internal clock
	 {
		sampleRate = timecode->sampleRate;
		eSBN = timecode->sampleRate/(((_nTempo/4.0)*16.0)/60.0);
		boxBPM = _nTempo;
	 }
	 else // we are not syncing and not running so exit!
	 {
		 bPlaying = false;
		 _bPlayingFromStart = false;
		 dPPQPlayed = 0;
		 currentStep = -1;
		 CBX_CLEAR_BUFFER
		 return;
	 }


	//setParameter(kValue, (timecode->ppqPos - timecode->barStartPos) /127.f * 4.0f); // display current note (0-15)
	//m_Part[_nCurrPart].cPattern = timecode->tempo;
//	currFrame = ((timecode->ppqPos - timecode->barStartPos) /127.f * 4.0f) * eSBN;
		/*
		struct VstTimeInfo
{
	double samplePos;			// current location
	double sampleRate;
	double nanoSeconds;			// system time
	double ppqPos;				// 1 ppq
	double tempo;				// in bpm
	double barStartPos;			// last bar start, in 1 ppq
	double cycleStartPos;		// 1 ppq
	double cycleEndPos;			// 1 ppq
	long timeSigNumerator;		// time signature
	long timeSigDenominator;
	long smpteOffset;
	long smpteFrameRate;		// 0:24, 1:25, 2:29.97, 3:30, 4:29.97 df, 5:30 df
	long samplesToNextClock;	// midi clock resolution (24 ppq), can be negative
	long flags;					// see below
};
		*/

	//update knobs
	inputKnobVolume = m_Part[_nCurrPart].fVolume; // Knobpositions
	inputKnobCutoff = m_Part[_nCurrPart].fCutOffFreq;
	inputKnobRezo = m_Part[_nCurrPart].fResonance;
	inputKnobEnvMod = m_Part[_nCurrPart].fEnvMod;
	inputKnobAccent = m_Part[_nCurrPart].fAccent;
	inputKnobTune = m_Part[_nCurrPart].fTuning;
	inputKnobDecay = m_Part[_nCurrPart].fDecay;
	if (m_Part[_nCurrPart].fWaveform == 0.0f)
		boxSndType = 0;
	else
		boxSndType = 1;


	//void SilverBoxEditor::SetDebugString(char * string)

	//	if (editor) ((SilverBoxEditor*)editor)->setParameter(kPlay, m_fPlay);

	//float temp = (timecode->ppqPos - timecode->barStartPos) * 4.0f;  // this has the [step].[position in step]
	//float temp = (timecode->ppqPos - timecode->barStartPos) * timecode->timeSigDenominator;
	float temp = ((timecode->ppqPos * (4.0/timecode->timeSigDenominator)) - timecode->barStartPos) * timecode->timeSigDenominator;
	// once inside creakbox we should calculate how many samples till next note and then do next note processin
	// once we hit that mark

	
	int step = temp;

#ifdef _DEBUG_STRING_

	char szString[256];

	float samples_per_beat=(timecode->sampleRate*60.0f)/(timecode->tempo);     // samplesperbeat=(samples_per_minute / beats_per_minute) 
	float samples_per_note = (timecode->sampleRate*(60.0/timecode->timeSigDenominator))/(timecode->tempo);
    float beatpos=(1-fmod(timecode->ppqPos,1))*samples_per_beat;  

	if (editor)
	{
		int step = (int)temp;
		int frame = (int)(float)(temp * eSBN);
//		if (bGlobalDebug1 == false && bPlaying == true)
//		{
			sprintf(szString, "%d %f P%f P%d S%d pos: %d pc: %d pos: %f temp: %f ppqP %f", //fr: %d %s %d %d %f %d", 
			sampleFrames,
//			ppqDiff,
			timecode->ppqPos,
			dPPQPlayed, 
			//fGlobalDebug1,
			//	bNoteSlide, bSlideNext,
			//iGlobalDebug1, iGlobalDebug2,
			//											timecode->barStartPos,
														bPlaying, _bPlayingFromStart,
														iTemp,
														iProcessCounter, timecode->samplePos, temp,
														timecode->ppqPos
													//	timecode->tempo

													//	frame, 
													//	cNoteDesc[m_CurrPattern.Step[step].cPitch], 
													//	m_CurrPattern.Step[step].bSlide, 
													//	m_CurrPattern.Step[step].bAccent, beatpos, step
													);
			((SilverBoxEditor*)editor)->SetDebugString(szString);
			//editor->postUpdate();
			updateDisplay();
//		}
		//setParameter(666,0);
	}
#endif

	currPos = temp;

	memcpy(&(_tInfo), timecode, sizeof(VstTimeInfo));

#ifdef _FX_VERSION_
	CBProcess(out1, sampleFrames);
#else
	CBProcess(out1, sampleFrames);
#endif
	memcpy(out2, out1, sizeof(float)*sampleFrames);

	m_bProcessing = false;
}

//-----------------------------------------------------------------------------------------
long SilverBox::processEvents (VstEvents* ev)
{

	m_pVstEvents = ev;

	return 1;	// want more
}

//-----------------------------------------------------------------------------------------
void SilverBox::noteOn(long note, long velocity, long delta)
{
	currentNote = note;
	currentVelocity = velocity;
	currentDelta = delta;
	if (velocity == 0)
	{
		noteIsOn = false;
	}
	else
	{
		noteIsOn = true;
	}

//	if (editor) ((SilverBoxEditor*)editor)->setParameter(kPlay, m_fPlay);

	if (editor)
		editor->postUpdate();
}

void SilverBox::PlayNote(float value)
{
	if (value == 0.0f)
	{
		noteOn(currentNote, 0, 0); // note off
	}
	else
	{
		//currentNote - m_nRootKey = ?
		// - 64       - 0          = -64
		// 64          - 64         = 0;
		// 190           - 127        = 63;
		noteOn(currentNote, 127, 0); // note on
	}
}

long SilverBox::FixMidiData(long param, char value)
{
	long new_value = value;

	if (param == kWaveform)
	{
		if (value < 64)		new_value = 0;
		else				new_value = 127;
	}
	return new_value;
}

float SilverBox::FixMidiData(long param, float value)
{
	float new_value = value;
	return new_value;
}

long SilverBox::fxIdle () 
{
	if (editor)
		editor->postUpdate();

	updateDisplay();

	return 0;
}

/*
void SilverBox::SetCurrentPattern(int pattern)
{
	int step;

	for (step=0;step<64;step++)
	{
		m_CurrPattern.Step[step].cPitch =  Bank.tPattern[pattern].Step[step].cPitch;
		m_CurrPattern.Step[step].bAccent =  Bank.tPattern[pattern].Step[step].bAccent;
		m_CurrPattern.Step[step].bSlide =  Bank.tPattern[pattern].Step[step].bSlide;
	}

	iCurBank = (pattern/10); // 0-9
	iCurPreset = pattern - (iCurBank*10); // 0-9

	iCurPatternXX = ((iCurBank)*10)+iCurPreset;

	RefreshPatternDisplay();
	SetStepEdit(iStepEdit);
}

void SilverBox::SetNextPattern(int pattern)
{
	m_Part[_nCurrPart].cNextPattern = pattern;
	iNextPatternXX = pattern;
	RefreshPatternDisplay();
}
*/

void SilverBox::SetCurrentPart(int part)
{
	// set this based on mode
	int nPart = 0;
	//int nPart = kPart1 + part;
	int oldpart = _nCurrPart;

	//m_Part[part].bMute = true;
	//if (editor)
	//	((SilverBoxEditor*)editor)->setParameter(kVolume,		1.0f);

	oldpart = _nCurrPart;
	_nCurrPart = part;
	if (editor)
	{
//		((SilverBoxEditor*)editor)->setParameter(oldpart+kPart1, 0.0f);
//		((SilverBoxEditor*)editor)->setParameter(nPart, 1.0f);	
		UpdatePartSynth(part);
		UpdatePartSequencer(part);
	}

}

void SilverBox::UpdatePartSynth(int part)
{
	if (editor)
	{
		((SilverBoxEditor*)editor)->setParameter(kVolume,		m_Part[part].fVolume);
		((SilverBoxEditor*)editor)->setParameter(kWaveform,		m_Part[part].fWaveform);
		((SilverBoxEditor*)editor)->setParameter(kTuning,		m_Part[part].fTuning);
		((SilverBoxEditor*)editor)->setParameter(kCutOffFreq,	m_Part[part].fCutOffFreq);
		((SilverBoxEditor*)editor)->setParameter(kResonance,		m_Part[part].fResonance);
		((SilverBoxEditor*)editor)->setParameter(kEnvMod,		m_Part[part].fEnvMod);
		((SilverBoxEditor*)editor)->setParameter(kDecay,			m_Part[part].fDecay);
		((SilverBoxEditor*)editor)->setParameter(kAccent,		m_Part[part].fAccent);
	}

}

void SilverBox::UpdatePartSequencer(int part)
{
	RefreshPatternDisplay();
}


void SilverBox::PatternClear()
{
	for (int step = 0; step<NUM_STEPS; step++)
	{
		Bank.tPattern[currentPattern].Step[step].cPitch = 0;
		Bank.tPattern[currentPattern].Step[step].bAccent = false;
		Bank.tPattern[currentPattern].Step[step].bSlide = false;
//		memset(Bank.tPattern[currpattern].szName, 0, 30);
	}

	//SetCurrentPattern(currpattern);
	RefreshPatternDisplay();
}

void SilverBox::PatternCopy()
{
	memcpy((void *)&m_CopyPattern, (void *)&Bank.tPattern[currentPattern], sizeof(t_CB_Pattern));
}

void SilverBox::PatternPaste()
{
	memcpy((void *)&Bank.tPattern[currentPattern], (void *)&m_CopyPattern,  sizeof(t_CB_Pattern));
//	SetCurrentPattern(currpattern);
	RefreshPatternDisplay();
}

int random(int topnum)
{
	int iRand =  rand();

	float temp = (float)iRand/(float)RAND_MAX;

	iRand = temp*topnum;

	return iRand;
}

#ifdef _RANDOM_PATTERNS_

void SilverBox::PatternRandom1()
{
	srand( (unsigned)time( NULL ) );

	int step;
	int currpattern = currentPattern;

	for (step=0; step<16; step++)
	{
		Bank.tPattern[currpattern].Step[step].cPitch = random(15)+15;
		if (random(100) > 75)
			Bank.tPattern[currpattern].Step[step].bAccent = true;
		else
			Bank.tPattern[currpattern].Step[step].bAccent = false;

		if (random(100) > 75)
			Bank.tPattern[currpattern].Step[step].bSlide = true;
		else
			Bank.tPattern[currpattern].Step[step].bSlide = false;
	}

	RefreshPatternDisplay();
}

void SilverBox::PatternRandom2()
{
	srand( (unsigned)time( NULL ) );
	
	int step;
	int currpattern = currentPattern;

	for (step=0; step<16; step++)
	{
		int temp = random(10);
		int note;
		switch(temp)
		{
		case(0):
		case(7):
		case(8):
		case(9):
		case(10): note = 1 + 12*(random(2)+1); break;
			break;
		case(1): note = 4 + 12*(random(3)+1); break;
		case(2): note = 5 + 12*(random(3)+1); break;
		case(3): note = 6 + 12*(random(3)+1); break;
		case(4): note = 8 + 12*(random(3)+1); break;
		case(5): note = 11 + 12*(random(3)+1); break;
		case(6): note = 0;
		}

		Bank.tPattern[currpattern].Step[step].cPitch = note;
		if (random(100) > 75)
			Bank.tPattern[currpattern].Step[step].bAccent = true;
		else
			Bank.tPattern[currpattern].Step[step].bAccent = false;

		if (random(100) > 75)
			Bank.tPattern[currpattern].Step[step].bSlide = true;
		else
			Bank.tPattern[currpattern].Step[step].bSlide = false;
	}

	RefreshPatternDisplay();
}

#endif

void SilverBox::RefreshPatternDisplay()
{
	int step;
	for (step=0; step<16; step++)
	{
		if (editor)
		{
			((SilverBoxEditor*)editor)->setParameter(kAccent01+step, getParameter(kAccent01+step));
			((SilverBoxEditor*)editor)->setParameter(kPitch01+step,  getParameter(kPitch01+step));
			((SilverBoxEditor*)editor)->setParameter(kSlide01+step,  getParameter(kSlide01+step));
		}
	}

#ifndef _LIMITED_
	if (editor) ((SilverBoxEditor*)editor)->setParameter(kCurrentPatternDisplay,  getParameter(kCurrentPatternDisplay));
	if (editor) ((SilverBoxEditor*)editor)->setParameter(kNextPatternDisplay,  getParameter(kNextPatternDisplay));
#endif
	SetStepEdit(iStepEdit);
}

#ifndef _LIMITED_
void SilverBox::RefreshPresetDisplay()
{
  	int step;
  
  	int next_program, next_bank, next_preset;
  	
  	next_program = buttonPattern;
  	next_bank = next_program/10; // 0-9
  	next_preset = next_program - (next_bank*10); // 0-9
  
 	for (step=0; step<10; step++)
   	{
   		if (editor)
   		{
  			((SilverBoxEditor*)editor)->setParameter(kBank00+step, (next_bank == step ? 1.0f : 0.0f));
  			((SilverBoxEditor*)editor)->setParameter(kPreset00+step,  (next_preset == step ? 1.0f : 0.0f));
   		}
   	}
}
#endif


void SilverBox::openlog()
{
	logfile = 0;
#ifdef _LOG_
#if WIN32
	logfile = fopen("c:\\cbox.txt", "w"); //text mode - write
#endif

#if MAC
	logfile = fopen("cbox.txt", "w"); //text mode - write
#endif

	time_t currtime;
	time(&currtime);
	tm * tm_struct = localtime(&currtime);

	writelog(" *** CREAKBOX LOG FILE OPEN %s *** \n\n", asctime(tm_struct));
	writelog(" getDirectory() returned: %s\n", (char *)getDirectory());
#endif
}

void SilverBox::closelog()
{
	if (logfile)
	{
		writelog(" getDirectory() returned: %s\n", (char *)getDirectory());
		writelog(" *** CREAKBOX LOG FILE CLOSED *** \n");
		fclose(logfile);
	}
}

void SilverBox::writelog(const char * fmt, ...)
{
	int r;

	if (logfile)
	{
		va_list ap; 
		va_start(ap,fmt); 
		r=vfprintf(logfile,fmt,ap); 
		va_end(ap); 
		//result = fprintf( logfile, const char *format [, argument ]...);
	}
}


/////////////////////////////////////////////////
// creakbox stuff
/////////////////////////////////////////////////

void SilverBox::CBConstruct()
{
//	inFile = 0;
	//memset(line, 100, sizeof(char));
//	currFrame = 0;
}

	
void SilverBox::CBDestroy()
{
//	clearAutomationSteps();
}

char * SilverBox::readline()
{
	if (fgets(line, 100, inFile) == 0)
	{
		printf("END OF INPUT FILE - EXITING!\n");
		exit(0);
	}
	
	return line;
}

bool SilverBox::CBgetnextslide(int currnote)
{
	bool result;

	result = (_currStep.bSlide) && ((_currStep.cPitch!=0) && (_nextStep.cPitch!=0));

	return result;
}

long double SilverBox::CBgetnextnote(int currnote)
{
	return cNoteFreq[_nextStep.cPitch];
}


void SilverBox::SetCurrentStep(int step)
{
	if (step == -1) return;
	int oldStep = iCurStep;
	if (step != iCurStep)
	{
		iCurStep = step;
		// update UI
		if (editor)
		{
			((SilverBoxEditor*)editor)->setParameter(kStep01 + oldStep, 0.0f); // turn off old step
			((SilverBoxEditor*)editor)->setParameter(kStep01 + iCurStep, 1.0f); // turn on new step
		}		
	}
}

void SilverBox::setCurrentPage(int page)
{
	int oldPage = currentPage;
	currentPage = page;
	if (editor) ((SilverBoxEditor*)editor)->setParameter(kPage00 + oldPage, 0.0); // the 0.0 and 1.0 values are used
	if (editor) ((SilverBoxEditor*)editor)->setParameter(kPage00 + currentPage, 1.0);
	refreshSequencer();
}

void SilverBox::setStepEditMode(int mode)
{	
	iStepEditMode = mode;
	refreshStepEdit();
}

void SilverBox::refreshStepEdit()
{
	if (editor)
	{
		if (iCurPitch == 0)
		{
			((SilverBoxEditor*)editor)->setParameter(kNoteOff, 1.0f);
			for (int note=0; note<12; note++)
				((SilverBoxEditor*)editor)->setParameter(kNote01 + note, 0.0f);
			for (int oct=0; oct<12; oct++)
				((SilverBoxEditor*)editor)->setParameter(kOctave01 + oct, 0.0f);
		}
		else
		{
			((SilverBoxEditor*)editor)->setParameter(kNoteOff, 0.0f);
			for (int note=0; note<12; note++)
			{
				if (note == iCurNote)
					((SilverBoxEditor*)editor)->setParameter(kNote01 + note, 1.0f);
				else
					((SilverBoxEditor*)editor)->setParameter(kNote01 + note, 0.0f);
			}
			for (int oct=0; oct<12; oct++)
			{
				if (oct == iCurOctave)
					((SilverBoxEditor*)editor)->setParameter(kOctave01 + oct, 1.0f);
				else
					((SilverBoxEditor*)editor)->setParameter(kOctave01 + oct, 0.0f);
			}
		}

		for (int step=0;step<16;step++)
			((SilverBoxEditor*)editor)->setParameter(kStepEdit01 + step, 0.0f);
	}
}

void SilverBox::setLastStep(int step)
{
	Bank.tPattern[(currentGroup*10) + currentPattern].LastStep = step;
	//patternLength[(currentGroup * 10) + currentPattern] = step;
	refreshStepEdit();
	/*
	if (editor)
	{
		for (int step=0;step<16;step++)
			((SilverBoxEditor*)editor)->setParameter(kStepEdit01 + step, 0.0f);
	}
	*/
}

void SilverBox::CBInit(char * inFilename)
{
}

void SilverBox::refreshSequencer()
{
	RefreshPatternDisplay();
	/*
	for (int a=0; a<16; a++)
	{
		if (editor) ((SilverBoxEditor*)editor)->setParameter(kAccent01 + a, 0.0);
		if (editor) ((SilverBoxEditor*)editor)->setParameter(kPitch01 + a, 0.0);
		if (editor) ((SilverBoxEditor*)editor)->setParameter(kSlide01 + a, 0.0);	
	}
	*/
}
void SilverBox::CBReset()
{
	//boxOctave = 2;
//	boxBPM = 125.f;
	// end freq stuff

	if (!_bPlayingFromStart) // only reset this stuff when playing start of pattern after pausing/stopping
	{
		currentPattern = buttonPattern;
		currentGroup = buttonGroup;	
		setCurrentGroup(currentGroup);
		setCurrentPattern(currentPattern);	
		currentStep = -1;
		// INIT PARAMS
		gateStat = 1;
		boxGate = 0;
	//	c_unpreciceknob = 0.5f;
	//	snd_in_dist = 0;

		// RESET FILTERS
	//	resetknobs(0);
		IIR_resetIIR(lp1_biquad);
		IIR_resetIIR(lp2_biquad);
		IIR_resetFIR(lp3_biquadFIR);
		IIR_resetIIR(hp1_biquad);
		IIR_resetIIR(hp2_biquad);

		// RESET DISTORTION BOX
		dist_initdist();

		// RESET VARIABLES
		bNoteSlide = 0;
		bSlideNext = 0;
	//	bufcount = 0;

		boxSlide = 1.f;           // change by wwelti: A value of 1 (instead of 0) indicates now a non-slide.
		boxDecay = 2.5f;
	//	knobVolume = 0;
		boxRezo = 0;
		boxAccent = false;
		boxOsc = 0;
		boxOscFrq = 0;
	//	boxMEG = 0;
		boxVolEnv = 1.f;
		boxAccentEnv = 0;
		temp_boxAccentFrq = 0;
		temp_boxAccentEnv = 0;
		bSlideNext = 0;


	//	tempAccentEnv = 0;

	//	snd_in = 0;
	//	snd_hiss = 0;

	//	lNON = 1;    // number of notes
	//	c_count = 0; // Start at sample position 0
	//	eSBN = sampleRate/(((boxBPM/4.f)*16.f)/60.f); // determine number of samples between two notes

		liStartPos = 1;
		if (liStartPos<1) liStartPos = 1;
		liEndPos = 1;
		boxSndType = 1; // 0 = saw, 1 = square
	}
	else
	{
		// RESET FILTERS
	//	resetknobs(0);
		IIR_resetIIR(lp1_biquad);
		IIR_resetIIR(lp2_biquad);
		IIR_resetFIR(lp3_biquadFIR);
		IIR_resetIIR(hp1_biquad);
		IIR_resetIIR(hp2_biquad);

		// RESET DISTORTION BOX
		dist_initdist();

		boxDecay = 2.5f;
		boxRezo = 0;
		boxAccent = false;
		boxOsc = 0;
		boxOscFrq = 0;
	//	boxMEG = 0;
		boxVolEnv = 1.f;
		boxAccentEnv = 0;
		temp_boxAccentFrq = 0;
		temp_boxAccentEnv = 0;
	}
#ifdef _CYCLE_

	if (bCycle) // cycle logic: loop the patterns from iCycleStart to iCycleFinish
	{
		int next_program, next_bank, next_preset;
		next_program = m_Part[0].cNextPattern;
		next_bank = next_program/10; // 0-9
		next_preset = next_program - (next_bank*10); // 0-9

		if ((!_bPlayingFromStart) || (m_Part[0].cNextPattern < iCycleStart) || (m_Part[0].cNextPattern > iCycleFinish))// we stopped so start from beginning
		{
			SetCurrentPattern(iCycleStart);
			currentPattern = iCycleStart;
		}
		else // we are already cycling.. set the next pattern
		{
			// version a
			/*
			SetCurrentPattern(m_Part[0].cNextPattern);
			m_Part[0].cPattern = m_Part[0].cNextPattern;
			*/
			// version b
			if (currentPattern < iCycleFinish)
				currentPattern++;
			else
				currentPattern = iCycleStart;
			SetCurrentPattern(currentPattern);
		}

		// set next pattern
		// version a
		/*
		if (m_Part[0].cPattern < iCycleFinish)
			m_Part[0].cNextPattern = m_Part[0].cPattern + 1;
		else
			m_Part[0].cNextPattern = iCycleStart;
		*/

		// version b
		m_Part[0].cNextPattern = currentPattern;

		SetNextPattern(m_Part[0].cNextPattern);

		// update ui

		int new_bank, new_preset, new_program;
		new_program = m_Part[0].cNextPattern;
		new_bank = new_program/10; // 0-9
		new_preset = new_program - (new_bank*10); // 0-9

		// program = (bank*8) + preset
//		writelog("new program %d bank %d preset %d\n", program, new_bank, new_preset);
	//	writelog("next program %d bank %d preset %d\n", next_program, next_bank, next_preset);

//		SetNextPattern(program);

		if (editor)
		{

			// turn off all buttons first
		//	for (int x=0; x<10; x++)
		//	{
		//		((SilverBoxEditor*)editor)->setParameter(kBank00 + next_bank, 0.0f); // turn off old bank
		//	}
#ifndef _LIMITED_
			((SilverBoxEditor*)editor)->setParameter(kBank00 + next_bank, 0.0f); // turn off old bank
			((SilverBoxEditor*)editor)->setParameter(kBank00 + new_bank, 1.0f); // turn on new bank

			((SilverBoxEditor*)editor)->setParameter(kPreset00 + next_preset, 0.0f); // turn off old preset
			((SilverBoxEditor*)editor)->setParameter(kPreset00 + new_preset, 1.0f); // turn on new preset
#endif _LIMITED_
		}
	}
	else // do non cycle logic
	{
#endif _CYCLE_
		// copy current pattern
		//SetCurrentPattern(m_Part[0].cNextPattern);
		currentPattern = nextPattern;
#ifdef _CYCLE_
	}
#endif _CYCLE_
	RefreshPatternDisplay();
	_bPlayingFromStart = true;
	iGlobalDebug1 = 0;
	iGlobalDebug2 = 0;

	iDebugPreviousStep = -1;
	iCurPosLast = 0;
#ifdef _PROCESS_DEBUG_
	writelog("SilverBox::CBReset() %d\n",iDebugPattern);
#endif _PROCESS_DEBUG_
}


void SilverBox::CBProcess(float * buffer, int sampleFrames)
{

	writelog("CBProcess() - sampleFrames: %d\n", sampleFrames);
	long double snd_in;
	int current_step;

	long double knobVolume; // Knobpositions
	long double knobCutoff;
	long double pre_knobRezo;
	long double knobRezo;
	long double knobEnvMod;
	long double knobAccent;
	long double knobTune;
	long double knobDecay;
	long double boxCutoff;
	long double tempcutoff; // used for calculating the cutoff
//	long double filterparam; // percentage from one note to another (ticks in trackers)
//	long double boxOsc = 0;

	/*
	oldppq = timecode->ppqPos;
		sampleRate = timecode->sampleRate;
	eSBN = timecode->sampleRate/(((timecode->tempo/4.f)*16.f)/60.f);
	boxBPM = timecode->tempo;
	float temp = (timecode->ppqPos - timecode->barStartPos)* 4.0f;  // this has the [step].[position in step]
	*/
	//_tInfo
	double temp, samples_per_beat, fSamplesPerStep;
	int eSBN; // samples between note
	int iCurPos; // positions into note in samples

	 if (m_fSync == 1.0) // syncing to host
	 {
		//temp = fmod(_tInfo.ppqPos, 1.0f) * 4.0f;
		//temp = (_tInfo.ppqPos - _tInfo.barStartPos)* 4.0f;  // this has the [step].[position in step]
		//temp = (_tInfo.ppqPos - _tInfo.barStartPos)* _tInfo.timeSigDenominator;
		//(4.0/timecode->timeSigDenominator))
		//temp = ((_tInfo.ppqPos * (4.0/_tInfo.timeSigNumerator)) - _tInfo.barStartPos)* _tInfo.timeSigNumerator;
		temp = ((_tInfo.ppqPos * (4.0/_tInfo.timeSigDenominator)) - _tInfo.barStartPos) * _tInfo.timeSigDenominator;
		//temp = (_tInfo.ppqPos - _tInfo.barStartPos)/(1.0/_tInfo.timeSigDenominator);
		//

		// problem is fruity treats 4/3 timing like 3/4 using the same ppqPos as 3/4 :(

		samples_per_beat=(_tInfo.sampleRate*60.0)/(_tInfo.tempo);     // samplesperbeat=(samples_per_minute / beats_per_minute) 
		//fSamplesPerStep = (_tInfo.sampleRate*15.0)/(float)(int)(_tInfo.tempo);
		fSamplesPerStep = (_tInfo.sampleRate*(60.0/_tInfo.timeSigDenominator))/(_tInfo.tempo);
		eSBN = (int)fSamplesPerStep;
		//currFrame = fSamplesPerStep * temp;
		/*
		3/4 is 3 quarter notes per measure.	1 2 3 4 1 2 3 4 1 2 3 4
		4/3 is 4 third per measure		1 2 3 1 2 3 1 2 3 1 2 3
		5/2 is 5 half notes per measure.
		6/8 is 6 eighth notes per measure. 

		*/


		iCurPos = fmod(temp, 1)*fSamplesPerStep;//(float)eSBN;
		current_step = temp;
	 }
	 else  if(m_fRunStop == 1.0) // running at internal clock
	 {
		temp = (_fInternalClock)* 4.0f;  // this has the [step].[position in step]

		samples_per_beat=(_tInfo.sampleRate*60.0)/(double)(_nTempo);     // samplesperbeat=(samples_per_minute / beats_per_minute) 
		fSamplesPerStep = (_tInfo.sampleRate*15.0)/(double)(_nTempo);
		eSBN = (int)fSamplesPerStep;

		//currFrame = fSamplesPerStep * temp;

		iCurPos = fmod(temp, 1)*(float)eSBN;
		current_step = temp;
	 }
	 writelog("starting iCurPos %d fSamplesPerStep %lf eSBN %d\n", iCurPos, fSamplesPerStep, eSBN);
	 if (iCurPos < 100)
	 {
		 dumpVstTimeInfo(_tInfo);
	 }
	 SetCurrentStep(currentStep);
	 fGlobalDebug1 = fSamplesPerStep;

	 // iGlobalDebug1 will hold the last big iCurPos;
	 // iGlobalDebug2 will hold the current big iCurPos;
	 //iGlobalDebug1 = 1;
	 iGlobalDebug2 = iCurPos;


//	 iGlobalDebug1 = bNoteSlide;
//	 iGlobalDebug2 = bSlideNext;

//	currFrame = (int)((float)temp*eSBN);
//	if (currFrame < 0)
//		currFrame == 0;
	bool bNewNote = false;
	bool bReset = false;
/*
	if (iDebugPattern == 1)
	{
		if (current_step == 8 || current_step == 9)
		{
			writelog("step %02d iCurPos %d\n", current_step, iCurPos);
		}
	}
*/
	// this will make sure a new note gets triggered
	if (iDebugPreviousStep == (current_step - 1))
	{
		bNewNote = true;
	}

	long eventFrame = -1; // -1 = no events
	long numEvents = 0;
	long currEvent = 0;

	if (m_pVstEvents != 0)
	{
		numEvents =  m_pVstEvents->numEvents;
	}


//	m_bProcessing = true;

	if (numEvents == 0)
	{
		eventFrame = -1;
		m_pVstEvents = 0;
	}
	else
	{
		// find first event midi event
		while(currEvent < numEvents)
		{
			if ((m_pVstEvents->events[currEvent])->type == kVstMidiType)
			{
				eventFrame = (m_pVstEvents->events[currEvent])->deltaFrames;
				break;
			}
			else
			{
				currEvent++;
			}
		}
	}
		

	for (int pos=0; pos<sampleFrames; pos++)
	{ 
		// handle vst midi events
		while (pos == eventFrame) // change this to while (eventFrame == frameCounter)
		{
			processMidiEvent(currEvent);
		//	m_fSampleRateDisplay = m_fPlaySampleRate;
			currEvent++; // check next event

			while (currEvent < numEvents)
			{
				if ((m_pVstEvents->events[currEvent])->type == kVstMidiType)
				{
					eventFrame = (m_pVstEvents->events[currEvent])->deltaFrames;
					break;
				}
				else
				{
					currEvent++;
				}
			}

			if (currEvent >= numEvents)
			{
				eventFrame = -1; // no more events
				m_pVstEvents = 0;
			}

		} 

		if (iCurPos < 0)
		{
			iCurPos++;
			continue;
		}
		/*
		if (iCurPos == 0)
		{
			bNewNote = true;
		}

		if ((current_step == 0) && (iCurPos == 0))
		{
			bReset = true;
		}
		*/

		if (iCurPos > eSBN)
		{
			writelog("iCurPos %d > eSBN %d - pos %d\n", iCurPos, eSBN, pos);
			iGlobalDebug1 = iGlobalDebug2;
			iCurPos = iCurPos - (eSBN+1);
			bNewNote = true;
			current_step++;
			if (current_step > 15)
			{
				current_step = 0;
				iDebugPattern++;
				bReset = true;
			}
			else
			{
		//		writelog("---step %02d - ", current_step);
			}
			//SetCurrentStep(currentStep);
		//	writelog("iCurPos %d\n", iCurPos);

		}
		
		if (iCurPos == 5000)
		{
			iGlobalDebug1 = 0;
		}

		if (iCurPos == 0 || bNewNote)
		{
			if (current_step == 0) // reset filter every 16 steps
			{
				CBReset();
				bReset = false;
#ifdef _CYCLE_
				if (bCycle) wwelti_algorithm = false;         // HACK by wwelti: switch algorithms always at pattern break, according to LOOP button.
				else        wwelti_algorithm = true;
#endif
				//bNextPatternLock = false;
			}
/*
			if (current_step == 8) // half way thru pattern so lets check if we are cycling
			{
#ifdef _CYCLE_
				if (bCycle)
				{
					int next_pattern;

					if (m_Part[0].cPattern < iCycleFinish)
						next_pattern = m_Part[0].cPattern + 1;
					else
						next_pattern = iCycleStart;

					setProgram(next_pattern);
				}
#endif
				//bNextPatternLock = true;
			}

			if (current_step == 15)
			{
				bNextPatternLock = true;
			}
*/
			if (iDebugPreviousStep != (current_step -1))
			{
#ifdef _PROCESS_DEBUG_
				writelog("*** missed step %02d\n", current_step -1);
#endif
			}
#ifdef _PROCESS_DEBUG_
			writelog("iCurPosLast %d _tInfo.ppqPos %f\n", iCurPosLast, _tInfo.ppqPos);
			writelog("---step %02d - ", current_step);
#endif
#ifndef _LIMITED_
			if (bNextPatternLock)
				RefreshPresetDisplay(); // push bad button back up
#endif

			iDebugPreviousStep = current_step;
		}

		if (!_bPlayingFromStart || current_step < 0)
		{
			iCurPos++;
			continue;
		}

//		if (currFrame == 0) 
//		{
//			CBReset();
//		}

//		current_step = currFrame/eSBN;
//		if (current_step > 15)
//		{
//			current_step = 0;
//		}

		// if start of new step then do new note process
		//if (current_step*eSBN == currFrame)
		if (iCurPos == 0 || bNewNote)
		{
#ifdef _PROCESS_DEBUG_
			writelog("init %d pos %d %d\n", current_step, pos, iCurPos);
#endif
			currentStep++;
			if (currentStep == Bank.tPattern[(currentGroup*10)+currentPattern].LastStep) // we are entering last step
			{				
				bLastStepLock = true;
				
				// set the next
				nextGroup = buttonGroup;
				nextPattern = buttonPattern;				
				nextStep = 0;
				//writelog("setting nextGroup to %d and nextPattern to %d\n", nextGroup, nextPattern);
			}
			else if (currentStep > Bank.tPattern[(currentGroup*10)+currentPattern].LastStep) // we are past the last step
			{
				//writelog("patternLength[%d] is %d\n", ((currentGroup * 10) + currentPattern), Bank.tPattern[(currentGroup*10)+currentPattern].LastStep);
				currentStep = 0;
				currentGroup = nextGroup;
				currentPattern = nextPattern;

				//writelog("setting currentGroup to %d and currentPattern to %d\n", currentGroup, currentPattern); 

				if (currentStep == (Bank.tPattern[(currentGroup*10)+currentPattern].LastStep - 1))
					nextStep = currentStep;
				else
					nextStep = currentStep + 1;


				//if (!((currentGroup == buttonGroup) && (currentPattern == buttonPattern)))
				//{
					setCurrentGroup(currentGroup);
					setCurrentPattern(currentPattern);
					refreshSequencer();
				//}
			}
			else
			{			
				nextPattern = currentPattern;
				nextGroup = currentGroup;
				nextStep = currentStep + 1;
				//writelog("ELSE setting nextGroup to %d and nextPattern to %d\n", nextGroup, nextPattern);
				bLastStepLock = false;
			}

			_currStep = Bank.tPattern[(currentGroup*10)+currentPattern].Step[currentStep];
			_nextStep = Bank.tPattern[(nextGroup*10)+nextPattern].Step[nextStep];
			SetCurrentStep(currentStep);
			dumpStep(currentStep, _currStep);
			dumpVstTimeInfo(_tInfo);

			if (!bNoteSlide)
			{
				// START A NEW NOTE...................................................
				//Determine the frequency of the note from pattern
				boxOscFrq = cNoteFreq[_currStep.cPitch];

				// Detune error in 303
				//if (boxOscFrq>0) boxOscFrq = pow(2,log(boxOscFrq +random*(boxOscFrq/11000) )); //HELP
				//if (boxOscFrq>0) boxOscFrq = pow(2.f,log2(boxOscFrq +((long double)rand()/(long double)RAND_MAX)*(boxOscFrq/11000.f) )); //HELP
				//if (boxOscFrq>0) boxOscFrq = pow(2.f,log2(boxOscFrq +(-8)*(boxOscFrq/11000.f) )); //HELP
			//	if (boxOscFrq>0) boxOscFrq = pow(2,log2(boxOscFrq +(boxOscFrq/11000) )); //HELP

				// reset
				boxOsc = 0;
				boxDecay = 4.f;
				boxMEG = 0;
				boxVolEnv = 1.f;
				boxLedError = 0; // (int)(((long double)rand()/(long double)RAND_MAX)*200.f); //random(200); // emulates the LED error which triggers the note unprecisely;

				if (boxOscFrq==0)
					gateStat=1;
				else
					gateStat=0; // open gate!
			}

			// Read slide and accent information from pattern
			boxAccent = _currStep.bAccent;
			if (boxAccent)
			{
				// If accent then introduce a new envelope and set the sweep to 2 octaves higher than the current sweep level!
				boxAccentEnv = 0.6f;
				boxAccentFrq = temp_boxAccentFrq+2.f;
			}

			// check previous step
		/*
			int prevstep;

			if (current_step == 0)
				if (currFrame == 0)
					prevstep = 0;
				else
					prevstep = 15;
			else
				prevstep = current_step - 1;
*/
			bSlideNext = CBgetnextslide(currentStep);
			writelog(" -- bNoteSlide: %d bSlideNext: %d\n", bNoteSlide, bSlideNext);

			//bSlideNext = getnextslide(current_step);
			bNewNote = false;
		}
//welti start
    int dspCycles = sampleFrames - pos;            // Wie oft kann die folgende DSP-Schleife durchlaufen werden, ohne 
                                                   // daß die Pattern-Logik bemüht werden muß?
	writelog("dspCycles %d sampleFrames %d pos %d iCurPos %d\n", dspCycles, sampleFrames, pos, iCurPos);
    if ((eventFrame>=0) && (eventFrame>pos))
    {
      if (eventFrame-pos < dspCycles)
        dspCycles = eventFrame-pos;
    }

    if ((iCurPos < eSBN) && (iCurPos > 0))
    {
       if (eSBN - iCurPos < dspCycles)
         dspCycles = eSBN - iCurPos;
    }
    else dspCycles = 1;

    int temp_iCurPos = iCurPos;              // iCurPos sichern
    int loopEndVal = pos + dspCycles;        // End-Vergleichswert für DSP-Schleife
	writelog("dspCycles %d loopEndVal %d pos %d\n", dspCycles, loopEndVal, pos);
//welti end
    
		// READ KNOBS
		//HELP - need to convert functions
#ifdef _STANDALONE_
		c_pattern = 0;
		liStartPos = 0;
		filterparam = ((long double)(currFrame) / (16.f*eSBN)); // -(c_pattern-liStartPos); // percentage into pattern
		c_pattern = 0;
		knobVolume  = 2.f*getVolume(c_pattern,(filterparam)*100.f)/75.f;
		if (boxSndType==0) knobVolume = knobVolume*1.5f;
		  pre_knobRezo    = getRezo(c_pattern,(filterparam)*100.f)/100.f;
		knobAccent  = getAccent(c_pattern,(filterparam)*100.f)/100.f;
		knobDecay   = getDecay(c_pattern,filterparam*100.f)/100.f;
		knobTune    = (getTune(c_pattern,(filterparam)*100.f)/100.f)*2.f-1.f;
		knobEnvMod  = getEnvMod(c_pattern,(filterparam)*100.f);
		knobCutoff   = getCutoff(c_pattern,(filterparam)*100.f)/100.f;
#else
//		c_pattern = 0;
//		liStartPos = 0;
//		filterparam = ((long double)(currFrame) / (16.f*eSBN)); // -(c_pattern-liStartPos); // percentage into pattern
//		c_pattern = 0;
//		knobRezo    = getRezo(c_pattern,(filterparam)*100.f)/100.f;
		// volume - 100 // tune - 50 //cutoff - 15 // rezo - 100
		// env mod - 10 // decay - 90 // accent - 100

		knobVolume  = 20.f*inputKnobVolume/75.f;
		if (boxSndType==0) knobVolume = knobVolume*1.5f;
		
		knobAccent  = inputKnobAccent*0.65f;
		knobDecay   = inputKnobDecay;
//		// tune .5 = 0.0

		knobTune    = inputKnobTune*2.f-1.f;
		knobEnvMod  = inputKnobEnvMod;

		knobCutoff   =  (powf(2.f,(inputKnobCutoff*0.5f))-1.f);
		knobCutoff = (powf(2.f,knobCutoff*0.80f)-0.987f); //*100.f;
		  pre_knobRezo    = (sin((inputKnobRezo)*halfpi) * 75.f + 25.f)/100.f;

/*
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
*/
#endif

		boxCutoff   = knobCutoff+(boxOscFrq/sampleRate)*0.33f;

		// first303
		// volume, tune,  1-50, 9999-50
		// cutoff 1-1, 9999-15
		// res, accent 1-100,9999-100
		// decay 1-10, 9990-10
		// envmod 1-90,9999-90
/*
		knobVolume  = .2;//2*(.1)/.75;
		if (boxSndType==0) knobVolume = knobVolume*1.5f;
		knobRezo    = 1.0;
		knobAccent  =.65;
		knobDecay   =.1;
		knobTune    =(.5)*2-1;
		knobEnvMod  =90;
		boxCutoff   =.15;
		boxCutoff   =boxCutoff+(boxOscFrq/44100)*0.33;
*/

    if (wwelti_algorithm)  // Invoke new DSP algorithm by wwelti
    {

      double boxMEG_c0;
      double boxMEG_c1;

      if (boxAccent)
      {
        boxMEG_c0 = -1.75f;  boxMEG_c1 = 0.00009f;  // One volume envelope for accented notes
      }
      else
      {
        boxMEG_c0 = -1.f;    boxMEG_c1 = 0.0000082f; // ...and another for NOT accented notes
      }

      double two_power_knobTune = powf(2.f,knobTune);

      double gateStat_c0;
      double gateStat_c1;

      if (gateStat==0)
      {
#ifdef _FRANK_TWEAK_
		  gateStat_c0 = 1.f; gateStat_c1 = 0.9f;
#else
        gateStat_c0 = 1.f; gateStat_c1 = 0.5f;
#endif
      }
      else
      {
#ifdef _FRANK_TWEAK_
		  gateStat_c0 = 0.f; gateStat_c1 = 0.004f;
#else
		gateStat_c0 = 0.f; gateStat_c1 = 0.01f;
#endif
      }

      //static double temp_ww0_min = 100;       // Nur als Debug-Ausgabe, um den Wertebereich zu überprüfen
      //static double temp_ww0_max = 0;

      double OscSwitch;
      if (boxOscFrq==0)
      {
        OscSwitch = 0.f;
      }
      else
      {
        OscSwitch = 1.f;
      }

      if (boxSndType == 0)  // separate loops for Saw and Square! Here comes the saw loop:
      {

        while (pos < loopEndVal)                 // DSP-Loop
        {
		      
          boxMEG += (boxMEG_c0 - boxMEG) * boxMEG_c1;                      // CALCULATE MAIN ENVELOPE		   
          boxGate = boxGate + (gateStat_c0 - boxGate) * gateStat_c1;       // CALCULATE GATE		    
		      boxVolEnv =boxVolEnv+ (0.f - boxVolEnv)*0.00001f;                // CALCULATE VOLUME ENVELOPE

		      // IF frequency of accent sweep >0 then keep subtracting
		      if (boxAccentFrq>-0)
		      {
			      boxAccentFrq =boxAccentFrq-(2.0f/4500.f);
		      }
		      else 
		      {
			      boxAccentFrq =-0;
		      }

		      if (boxAccentEnv>0)
		      {
			      boxAccentEnv =boxAccentEnv-0.00015f;
		      }
		      
		      temp_boxAccentFrq = temp_boxAccentFrq+(boxAccentFrq-temp_boxAccentFrq)*0.00144f;                // The accent sweep!		    
		      temp_boxAccentEnv = temp_boxAccentEnv+(boxAccentEnv-temp_boxAccentEnv)*0.03f;                   // the accent-envelope
		      boxDecay =boxDecay * ((knobDecay*0.0005f)+0.9995f);                                             // CALCULATE DECAY

          tempcutoff = boxCutoff * getTab2Interpolated(boxMEG+(knobEnvMod*-2.f)+(boxDecay*knobEnvMod)+temp_boxAccentFrq*knobAccent); // CutOff
                                                               		      
          boxOsc += twopi * (1.f/(88200*2.f)) * boxOscFrq * two_power_knobTune;     // CALCULATE FREQUENCY OF NOTE IF SOUND IS GENERATED!
		      if (boxOsc>twopi) boxOsc -= twopi;

		      // Shut gate or slide to next note! If we're about half through (a little longer for accented notes) then shut gate
		      //if (!bSlideNext && ((((current_step+1)*eSBN)-currFrame)<((eSBN/2.3f)*(1.f-(boxAccent*0.45f))) ))
#ifdef _FRANK_TWEAK_
			  if (!bSlideNext && (eSBN-iCurPos<((eSBN*(1.f/1.9f))*(1.f-(boxAccent*0.25f))) ))
#else
		      if (!bSlideNext && (eSBN-iCurPos<((eSBN*(1.f/2.3f))*(1.f-(boxAccent*0.45f))) ))
#endif
		      {
			      gateStat = 1; //shut gate!
            boxGate = boxGate + (gateStat_c0 - boxGate) * gateStat_c1; // set Gate Control-Values
		      }

		      snd_in = (( (twopi-boxOsc) * (1.f/twopi) ));//tonegen_sawWave(boxOsc);        // GENERATE SAW

          snd_in *= OscSwitch; // wwelti: My private 0-1 gate which helps avoiding if's.
		      
  	      snd_in = IIR_FIRlp(lp3_biquadFIR, 0.7f, snd_in);      // ...THEN LOWPASS A LITTLE BIT TO ROUND OFF SHARP EDGES		      
		      snd_in = IIR_doIIRhigh(hp1_biquad,0.0068027f,snd_in); // HIGHPASS FILTER THE SOUND TO BRIGHTEN
/*
#ifdef _FRANK_TWEAK_
#else			  		
*/
		      knobRezo = pre_knobRezo+(temp_boxAccentEnv*0.04f)*knobAccent; // IF NOTE IS ACCENTED, THE RESONANCE SHOULD BE INCREASED A BIT
//#endif
		      knobRezo *= 1.1f;       // INCREASE RESONANCE ON SAW

		      snd_in = snd_in*boxGate;       // APPLY GATE (NOTE ON/OFF)
		      
  	      snd_in = wwelti_IIR_IIRlp3pole(lp2_biquad, tempcutoff, knobRezo*0.9l, snd_in, boxOscFrq); // filter saw // LOWPASS FILTER

		      // AMP + AMPLITUDE ENVELOPE
		      snd_in *= ((1.f-knobRezo)*1.5f+1.f); // **** amplify again to correct distort-damping from lowpass!
#ifdef _FRANK_TWEAK_
			  snd_in *= boxVolEnv *(temp_boxAccentEnv*5.f+1.f)*knobVolume;
#else
		      snd_in *= boxVolEnv *(temp_boxAccentEnv*4.f+1.f)*knobVolume;
#endif
		      
		      snd_in = dist_dist(snd_in);             // DISTORT SOUND
		      
		      if (snd_in<-1.0f)  snd_in=-1.0f;        // STREAM TO BUFFER
		      else if (snd_in>1) snd_in=1.0f;
      #ifdef _STANDALONE_
		      buffer[pos]=snd_in*32767.f;//diskbuffer[bufcount]=round(snd_in*32767);
      #else
		      buffer[pos]=snd_in;
      #endif
		      
  	      boxOscFrq =boxOscFrq * boxSlide;        // RENDER SLIDE
	  
		      iCurPos++;
          pos++;             // Ende der DSP-Loop
        }

      }
      else                  // Now the different DSP-Loop for Square
      {
        while (pos < loopEndVal)                 // DSP-Loop
        {
		      
          boxMEG += (boxMEG_c0 - boxMEG) * boxMEG_c1;                      // CALCULATE MAIN ENVELOPE		   
          boxGate = boxGate + (gateStat_c0 - boxGate) * gateStat_c1;       // CALCULATE GATE		    
		      boxVolEnv =boxVolEnv+ (0.f - boxVolEnv)*0.00001f;                // CALCULATE VOLUME ENVELOPE

		      // IF frequency of accent sweep >0 then keep subtracting
		      if (boxAccentFrq>-0)
		      {
			      boxAccentFrq =boxAccentFrq-(2.0f/4500.f);
		      }
		      else 
		      {
			      boxAccentFrq =-0;
		      }

		      if (boxAccentEnv>0)
		      {
			      boxAccentEnv =boxAccentEnv-0.00015f;
		      }
		      
		      temp_boxAccentFrq = temp_boxAccentFrq+(boxAccentFrq-temp_boxAccentFrq)*0.00144f;                // The accent sweep!		    
		      temp_boxAccentEnv = temp_boxAccentEnv+(boxAccentEnv-temp_boxAccentEnv)*0.03f;                   // the accent-envelope
		      boxDecay =boxDecay * ((knobDecay*0.0005f)+0.9995f);                                             // CALCULATE DECAY

          tempcutoff = boxCutoff * getTab2Interpolated(boxMEG+(knobEnvMod*-2.f)+(boxDecay*knobEnvMod)+temp_boxAccentFrq*knobAccent); // CutOff
                                                               		      
          boxOsc += twopi * (1.f/(88200*2.f)) * boxOscFrq * two_power_knobTune;     // CALCULATE FREQUENCY OF NOTE IF SOUND IS GENERATED!
		      if (boxOsc>twopi) boxOsc -= twopi;

		      // Shut gate or slide to next note! If we're about half through (a little longer for accented notes) then shut gate
		      //if (!bSlideNext && ((((current_step+1)*eSBN)-currFrame)<((eSBN/2.3f)*(1.f-(boxAccent*0.45f))) ))
#ifdef _FRANK_TWEAK_
			  if (!bSlideNext && (eSBN-iCurPos<((eSBN*(1.f/1.9f))*(1.f-(boxAccent*0.25f))) ))
#else
		      if (!bSlideNext && (eSBN-iCurPos<((eSBN*(1.f/2.3f))*(1.f-(boxAccent*0.45f))) ))
#endif
		      {
			      gateStat = 1; //shut gate!
            boxGate = boxGate + (gateStat_c0 - boxGate) * gateStat_c1; // set Gate Control-Values
		      }
		      
		      if (boxOsc>pi) snd_in = 0.5f;  // GENERATE SQUARE
				  else		       snd_in = -0.5f;

          snd_in *= OscSwitch; // wwelti: My private 0-1 gate which helps avoiding if's.
		      
		      if (snd_in<0)  snd_in = IIR_FIRlp(lp3_biquadFIR, 0.7f, snd_in);       // ...THEN LOWPASS A LITTLE BIT TO ROUND OFF SHARP EDGES
			    else		       snd_in = IIR_FIRlp(lp3_biquadFIR, 0.125f, snd_in);
		      
		      snd_in = IIR_doIIRhigh(hp1_biquad,0.0068027f,snd_in);                // HIGHPASS FILTER THE SOUND TO BRIGHTEN
			  /*
#ifdef _FRANK_TWEAK_
#else		    
			  */
		      knobRezo = pre_knobRezo+(temp_boxAccentEnv*0.04f)*knobAccent;        // IF NOTE IS ACCENTED, THE RESONANCE SHOULD BE INCREASED A BIT
//#endif
		      
		      snd_in = snd_in*boxGate;            // APPLY GATE (NOTE ON/OFF)
		      
		      snd_in = wwelti_IIR_IIRlp3pole(lp2_biquad, tempcutoff, knobRezo*0.992l, snd_in, boxOscFrq); // filter square // LOWPASS FILTER

		      // AMP + AMPLITUDE ENVELOPE
		      snd_in *= ((1.f-knobRezo)*1.5f+1.f); // **** amplify again to correct distort-damping from lowpass!
#ifdef _FRANK_TWEAK_
			  snd_in *= boxVolEnv *(temp_boxAccentEnv*5.f+1.f)*knobVolume;
#else
		      snd_in *= boxVolEnv *(temp_boxAccentEnv*4.f+1.f)*knobVolume;
#endif
		      
		      snd_in = dist_dist(snd_in);             // DISTORT SOUND
		      
		      if (snd_in<-1.0f)  snd_in=-1.0f;        // STREAM TO BUFFER
		      else if (snd_in>1) snd_in=1.0f;
      #ifdef _STANDALONE_
		      buffer[pos]=snd_in*32767.f;//diskbuffer[bufcount]=round(snd_in*32767);
      #else
		      buffer[pos]=snd_in;
      #endif
		      
  	      boxOscFrq =boxOscFrq * boxSlide;        // RENDER SLIDE
	  
		      iCurPos++;
          pos++;             // Ende der DSP-Loop
        }

      }

      pos--;                                   // Letzten inkrement rückgängig machen, da wir uns in einer For-Schleife befinden
      iCurPos = temp_iCurPos + dspCycles;      // iCurPos wiederherstellen und nachführen
	  writelog("--iCurPos: %d eSBN: %d\n", iCurPos, eSBN);
		  if (iCurPos == eSBN+1)       // moved this back out of the dsp loop. Seems to werk this way as well as before.
		  {
  #ifdef _PROCESS_DEBUG_
			  writelog("             ");                                                    
			  writelog(" end %d pos %d %d %d\n", current_step, pos, iCurPos, eSBN);
  #endif

        //char szString[256];
			  //sprintf(szString, "Min. Value: %f, Max. Value : %f",temp_ww0_min, temp_ww0_max);
  			//((SilverBoxEditor*)editor)->SetDebugString(szString);

			  // IF SLIDE, THEN RENDER HOW MUCH TO ADD OR SUBTRACT FREQUENCY
			  writelog("[a] bNoteSlide(%d)=bSlideNext(%d) iCurPos %d eSBN %d\n", bNoteSlide, bSlideNext, iCurPos, eSBN);
			  bNoteSlide=bSlideNext;
			  if (bNoteSlide)
			  {
				  eNextNote=powf(2.f,log2(CBgetnextnote(current_step))+knobTune);
				  boxSlide= exp(ln(eNextNote/powf(2.0f,log2(boxOscFrq)+knobTune)) / (eSBN));  // boxSlide is the delta per sample
			  }
			  else
			  {
				  boxSlide = 1.f;
			  }
		  }

      iCurPosLast = iCurPos;

    }
    else       // Invoke old DSP algorithm by cybiEE!
    {

      while (pos < loopEndVal)                 // DSP-Schleife
      {

		// CALCULATE MAIN ENVELOPE
		if (boxAccent)	
		{
			boxMEG = boxMEG+ (-1.75f - boxMEG)*0.00009f; // One volume envelope for accented notes
		}
		else
		{
			boxMEG = boxMEG+ (-1.f - boxMEG)*0.0000082f; // ...and another for NOT accented notes
		}

		// CALCULATE GATE
		if (gateStat == 0)
		{
			if (boxLedError==0) 
			{
#ifdef _FRANK_TWEAK_
				boxGate =boxGate+(1.f-boxGate)*0.9f;
#else
				boxGate =boxGate+(1.f-boxGate)*0.5f;
#endif
			}
		}
		else if (gateStat == 1)
		{
#ifdef _FRANK_TWEAK_
			boxGate =boxGate+(0.f-boxGate)*0.004f;			
#else
			boxGate =boxGate+(0.f-boxGate)*0.01f;
#endif
		}

		// CALCULATE VOLUME ENVELOPE
		boxVolEnv =boxVolEnv+ (0.f - boxVolEnv)*0.00001f;

		// IF frequency of accent sweep >0 then keep subtracting
		if (boxAccentFrq>-0)
		{
			boxAccentFrq =boxAccentFrq-(2.0f/4500.f);
		}
		else 
		{
			boxAccentFrq =-0;
		}

		if (boxAccentEnv>0)
		{
			boxAccentEnv =boxAccentEnv-0.00015f;
		}

		// The accent sweep!
		temp_boxAccentFrq = temp_boxAccentFrq+(boxAccentFrq-temp_boxAccentFrq)*0.00144f;

		// the accent-envelope
		temp_boxAccentEnv = temp_boxAccentEnv+(boxAccentEnv-temp_boxAccentEnv)*0.03f;

		// CALCULATE DECAY
		boxDecay =boxDecay * ((knobDecay*0.0005f)+0.9995f);

		// CALCULATE CUTOFF
		    
        //tempcutoff =pow(2.f, log2(boxCutoff) +boxMEG );                 // Changed by WW
//		tempcutoff =pow(2.f, log2(tempcutoff)+(knobEnvMod*-2.f) );
//		tempcutoff =pow(2.f, log2(tempcutoff)+(boxDecay*knobEnvMod) );
//		tempcutoff =pow(2.f, log2(tempcutoff)+temp_boxAccentFrq*knobAccent );

		    tempcutoff =powf(2.f, log2(boxCutoff) +boxMEG+(knobEnvMod*-2.f)+(boxDecay*knobEnvMod)+temp_boxAccentFrq*knobAccent );   // Changed by WW

    

		// CALCULATE FREQUENCY OF NOTE IF SOUND IS GENERATED!
		boxLedError = 0;
		if (boxLedError==0) // Don't play until ledError delay is over!
		{
			if (boxOscFrq != 0)
			{
				boxOsc = boxOsc+twopi/( (88200 * 2.f) / powf(2.f,log2(boxOscFrq) +knobTune ));
			//	boxOsc = boxOsc+twopi/( (sampleRate*4.f) / pow(2.f,log2(boxOscFrq) +knobTune ));
			}
			else
				boxOsc =0.f;
		}
		else
			boxLedError--;

		if (boxOsc>twopi) boxOsc=boxOsc-twopi;
		// Shut gate or slide to next note! If we're about half through (a little longer for accented notes) then shut gate
		//if (!bSlideNext && ((((current_step+1)*eSBN)-currFrame)<((eSBN/2.3f)*(1.f-(boxAccent*0.45f))) ))
#ifdef _FRANK_TWEAK_
		if (!bSlideNext && (eSBN-iCurPos<((eSBN/1.9f)*(1.f-(boxAccent*0.25f))) ))
#else
		if (!bSlideNext && (eSBN-iCurPos<((eSBN/2.3f)*(1.f-(boxAccent*0.45f))) ))
#endif
		{
			gateStat = 1; //shut gate!
		}

		// GENERATE SAW OR SQUARE
		if (boxOsc!=0)
		{
			if (boxSndType == 0)
				snd_in = (( (twopi-boxOsc) / twopi ));//tonegen_sawWave(boxOsc);
			else if (boxSndType == 1)
			{
				//snd_in = tonegen_sqrWave(boxOsc);
				if (boxOsc>pi)
					snd_in = 0.5f;
				else
					snd_in = -0.5f;
			}
		
			/* // this is for mixing saw/square - will be added back in later in 1.2 or whatever
			//m_Part[_nCurrPart].fWaveform == 0.0f boxsndtype = 0
			float fSquare, fSaw;
			float fPercent = m_Part[_nCurrPart].fWaveform;
			if (boxOsc>pi)
			{
				fSquare = 0.5f * fPercent;
			}
			else
			{
				fSquare = -0.5f * fPercent;
			}

			fSaw = (( (twopi-boxOsc) / twopi )) * (1.0f- fPercent);
			snd_in = fSquare + fSaw;
			*/
		
		}
		else
			snd_in = 0;

		// ...THEN LOWPASS A LITTLE BIT TO ROUND OFF SHARP EDGES
		if (boxSndType==0)
			snd_in = IIR_FIRlp(lp3_biquadFIR, 0.7f, snd_in);
		else
		{
			if (snd_in<0)
				snd_in = IIR_FIRlp(lp3_biquadFIR, 0.7f, snd_in);
			else
				snd_in = IIR_FIRlp(lp3_biquadFIR, 0.125f, snd_in);
		}

		// HIGHPASS FILTER THE SOUND TO BRIGHTEN
		snd_in = IIR_doIIRhigh(hp1_biquad,0.0068027f,snd_in);

		// IF NOTE IS ACCENTED, THE RESONANCE SHOULD BE INCREASED A BIT
		/*
#ifdef _FRANK_TWEAK_
		// nothing
#else
		*/
		    knobRezo = pre_knobRezo+(temp_boxAccentEnv*0.04f)*knobAccent;
//#endif

		// INCREASE RESONANCE ON SAW
		if (boxSndType==0) knobRezo =knobRezo*1.1f;

		// APPLY GATE (NOTE ON/OFF)
		snd_in = snd_in*boxGate;

		// LOWPASS FILTER
		if (boxSndType == 0)
		{
			snd_in = IIR_IIRlp3pole(lp2_biquad, tempcutoff, knobRezo*0.9l, snd_in, boxOscFrq); // filter saw
		}
		else if (boxSndType == 1)
		{
			snd_in = IIR_IIRlp3pole(lp2_biquad, tempcutoff, knobRezo*0.992l, snd_in, boxOscFrq); // filter square
		}
		


		// AMP + AMPLITUDE ENVELOPE
		/*
		snd_in:=snd_in* ((1-knobRezo)*1.5+1); // **** amplify again to correct distort-damping from lowpass!
      snd_in:=snd_in* boxVolEnv *(temp_boxAccentEnv*4+1)*knobvolume;
		*/
		snd_in = snd_in* ((1.f-knobRezo)*1.5f+1.f); // **** amplify again to correct distort-damping from lowpass!
#ifdef _FRANK_TWEAK_
		snd_in = snd_in* boxVolEnv *(temp_boxAccentEnv*5.f+1.f)*knobVolume;
#else
		snd_in = snd_in* boxVolEnv *(temp_boxAccentEnv*4.f+1.f)*knobVolume;
#endif

		// DISTORT SOUND
		snd_in = dist_dist(snd_in);

		// STREAM TO BUFFER
		if (snd_in<-1.0f) 
			snd_in=-1.0f;
		else if (snd_in>1) 
			snd_in=1.0f;
#ifdef _STANDALONE_
		buffer[pos]=snd_in*32767.f;//diskbuffer[bufcount]=round(snd_in*32767);
#else
		buffer[pos]=snd_in;
#endif

		// RENDER SLIDE
		    if (boxSlide!=1.f) 
		{
			boxOscFrq =boxOscFrq * boxSlide; //beregner slide
		//	boxOscFrq += boxSlide; //beregner slide
		}



		// only process on a next note
//		if ((current_step*5292 == currFrame) // end a new note
		//if ((current_step+1 == (currFrame+1)/eSBN)) // only go in here on an end of a note
		if (iCurPos == eSBN) // cybiEEE's code - never called from wwelti's algo
		{
#ifdef _PROCESS_DEBUG_
			writelog("             ");
			writelog(" end %d pos %d %d %d\n", current_step, pos, iCurPos, eSBN);
#endif
			// IF SLIDE, THEN RENDER HOW MUCH TO ADD OR SUBTRACT FREQUENCY
			writelog("[b] bNoteSlide(%d)=bSlideNext(%d)\n", bNoteSlide, bSlideNext);
			bNoteSlide=bSlideNext;
			if (bNoteSlide)
			{
			//	long double eNextNote;
				eNextNote=powf(2.f,log2(CBgetnextnote(current_step))+knobTune);
				boxSlide= exp(ln(eNextNote/powf(2.0f,log2(boxOscFrq)+knobTune)) / (eSBN));  // boxSlide is the delta per sample
				//eNextNote = CBgetnextnote(current_step);
				//boxSlide = (eNextNote -boxOscFrq )/eSBN;
			}
			else
			{
				    boxSlide = 1.f;
			}
		}

		// end process	
		iCurPos++;
		    iGlobalDebug2 = iCurPos;                     // Der globalDebug-Kram kann doch rausfliegen...
		if (iGlobalDebug1<iGlobalDebug2)
		{
			iGlobalDebug1 = iGlobalDebug2;
		}

	//	currFrame++; // do the increment here because below we check if the next time we go thru this loop if it is a new note

	//	if (currFrame == eSBN*16)
	//	{
	//		currFrame = 0;
	//	}

//welti
        pos++;             // Ende der DSP-Loop
      }
      pos--;                                   // Letzten inkrement rückgängig machen, da wir uns in einer For-Schleife befinden
      iCurPos = temp_iCurPos + dspCycles;      // iCurPos wiederherstellen und nachführen

//welti
		iCurPosLast = iCurPos;

    } // End: Selection of DSP-Algorithm (either wwelti or cybiEE's)

	}

	// post process :)    |<-- for fruity -----|
	if ((iCurPos == eSBN) || (iCurPos == eSBN-1))// we reach the end of the note
	{
		// post
#ifdef _PROCESS_DEBUG_
		writelog("post process ");
		writelog(" end %d pos %d %d %d\n", current_step, pos, iCurPos, eSBN);
#endif
		// IF SLIDE, THEN RENDER HOW MUCH TO ADD OR SUBTRACT FREQUENCY
		writelog("[c] bNoteSlide(%d)=bSlideNext(%d)\n", bNoteSlide, bSlideNext);
		bNoteSlide=bSlideNext;
		if (bNoteSlide)
		{
		//	long double eNextNote;
			eNextNote=powf(2.f,log2(CBgetnextnote(current_step))+knobTune);
			boxSlide= exp(ln(eNextNote/powf(2.0f,log2(boxOscFrq)+knobTune)) / (eSBN));  // boxSlide is the delta per sample
			//eNextNote = CBgetnextnote(current_step);
			//boxSlide = (eNextNote -boxOscFrq )/eSBN;
		}
		else
		{
			boxSlide = 1.f;
		}

		//init next note
		iGlobalDebug1 = iGlobalDebug2;
		iCurPos = iCurPos - eSBN;
		bNewNote = true;
		current_step++;
		if (current_step > 15)
		{
			current_step = 0;
			bReset = true;
		}
		else
		{
	//		writelog("---step %02d - ", current_step);
		}
		SetCurrentStep(currentStep);
	}

	if (m_fRunStop == 1.0) // update internal clock
	{
		double fppqPlayed = (double)sampleFrames/(double)(eSBN*4);
		_fInternalClock += fppqPlayed;
		if (_fInternalClock > 4.0f)
		{
			_fInternalClock -= 4.0f;
		}
		// 0 1 2 3
		// 1/4 note (1 beat) is 1.0
		// eSBN * 4 = 1.0
		// samplesFrames = amount of samples processed
		//_fInternalClock
		
		//timecode->sampleRate/(((timecode->tempo/4.f)*16.f)/60.f);
	}
}


//
// _FILE_CHUNK
//

#ifdef _FILE_CHUNK_
long SilverBox::getChunk(void** data, bool isPreset)
{
	writelog("getChunk isPreset: %d\n", isPreset);
	if (isPreset)
	{
		//ignore ispreset in 1.0
		//set up structure
		Preset.cId[0] = 'C';
		Preset.cId[1] = 'B';
		Preset.cId[2] = 'C';
#ifdef _LIMITED_
		Preset.cId[3] = 'L';
#else
		Preset.cId[3] = '0';
#endif
		Preset.iVersion[0] = 1;
		Preset.iVersion[1] = 0;
		// copy single pattern into preset
		memcpy(&(Preset.tPattern), &(Bank.tPattern[currentPattern]), sizeof(t_CB_Pattern));
		// fill data
		*data = &Preset;
		writelog("sizeof(Preset) is %d\n", sizeof(Preset));
		return sizeof (Preset);
	}
	else
	{
		Bank.cId[0] = 'C';
		Bank.cId[1] = 'B';
		Bank.cId[2] = 'C';
#ifdef _LIMITED_
		Bank.cId[3] = 'L';
#else
		Bank.cId[3] = '0';
#endif
		Bank.iVersion[0] = 1;
		Bank.iVersion[1] = 0;
		//memcpy(&(Bank.tPattern), &(Bank.tPattern), sizeof(t_CB_Pattern)*100);
		// fill data
		*data = &Bank;
		writelog("sizeof(Bank) is %d\n", sizeof(Bank));
		return sizeof(Bank);
	}
}	// returns byteSize


long SilverBox::setChunk(void* data, long byteSize, bool isPreset) 
{
	writelog("setChunk byteSize %d isPreset: %d\n", byteSize, isPreset);
	if (isPreset)
	{
		t_CBX_Preset *myPreset = (t_CBX_Preset*)data;
		//ignore ispreset
		// verify structure
		if (byteSize != sizeof(t_CBX_Preset))
		{
			writelog("size does not match\n");
			return 0;
		}

		// read data
		if ((myPreset->cId[0] == 'C') &&
			(myPreset->cId[1] == 'B') &&
			(myPreset->cId[2] == 'C') &&
#ifdef _LIMITED_
			(myPreset->cId[3] == 'L') &&
#else
			(myPreset->cId[3] == '0') &&
#endif
			(myPreset->iVersion[0] == 1) &&
			(myPreset->iVersion[1] == 0))
		{
			// copy pattern data from single preset(loaded) pattern
			memcpy(&(Bank.tPattern[currentPattern]), &(myPreset->tPattern), sizeof(t_CB_Pattern));
			RefreshPatternDisplay();
		}
		else
		{
			writelog("header data does not match up\n");
			return 0;
		}

		return 1;
	}
	else
	{
		t_CBX_Bank *myBank = (t_CBX_Bank*)data;
		//ignore ispreset
		// verify structure
		if (byteSize == sizeof(t_CBX_Bank_Old))
		{
			writelog("loading old format size %d", byteSize);
			// read data
			if ((myBank->cId[0] == 'C') &&
				(myBank->cId[1] == 'B') &&
				(myBank->cId[2] == 'C') &&
#ifdef _LIMITED_
				(myBank->cId[3] == 'L') &&
#else
				(myBank->cId[3] == '0') &&
#endif
				(myBank->iVersion[0] == 1) &&
				(myBank->iVersion[1] == 0))
			{
				memcpy(&(Bank.tPattern), &(myBank->tPattern), sizeof(t_CB_Pattern)*100);
				RefreshPatternDisplay();
			}
			else
			{
				writelog("header data does not match up\n");
				return 0;
			}
		}
		else if (byteSize == sizeof(t_CBX_Bank))
		{
			writelog("loading new format size %d", byteSize);
			// read data
			if ((myBank->cId[0] == 'C') &&
				(myBank->cId[1] == 'B') &&
				(myBank->cId[2] == 'C') &&
#ifdef _LIMITED_
				(myBank->cId[3] == 'L') &&
#else
				(myBank->cId[3] == '0') &&
#endif
				(myBank->iVersion[0] == 1) &&
				(myBank->iVersion[1] == 0))
			{
				memcpy((void *)&Bank, myBank, sizeof(t_CBX_Bank));
				RefreshPatternDisplay();
			}
			else
			{
				writelog("header data does not match up\n");
				return 0;
			}
		}
		else
		{
			writelog("invalid bank size: %d", byteSize);
			return 0;
		}


		return 1;
	}
}

#endif

/*
I think: 
inline DWORD Flip32(DWORD a) { return ((a&0x000000FF)<<24)|((a&0x0000FF00)<<8)|((a&0x00FF0000)>>8)|((a&0xFF000000)>>24); }
*/
void SilverBox::SwapBytes(long &l)
{
unsigned char *b = (unsigned char *)&l;
long intermediate =  ((long)b[0] << 24) |
                     ((long)b[1] << 16) |
                     ((long)b[2] << 8) |
                     (long)b[3];
l = intermediate;
}

void SilverBox::SwapBytes(float &f)
{
long *pl = (long *)&f;
SwapBytes(*pl);
}

//inline DWORD Flip32(DWORD a) { return ((a&0x000000FF)<<24)|((a&0x0000FF00)<<8)|((a&0x00FF0000)>>8)|((a&0xFF000000)>>24); };

long LoadFileIntoBuffer(char * file, unsigned char * buffer);

void SilverBox::LoadChunk(char * filename, bool isPreset)
{
	bool bNeedSwap = false;
	char szChnk[] = "CcnK";          /* set up swapping flag              */
	long lChnk = 'CcnK';
	bNeedSwap = !!memcmp(szChnk, &lChnk, 4);

	
	//
	unsigned char * buffer = 0;

	FILE * pFILE;
	pFILE = fopen(filename, "rb"); // open for reading in binary

	if (pFILE == 0) return;

	// move to end of file
	int seek = fseek(pFILE, 0, SEEK_END );


	if (seek != 0) { fclose(pFILE); return; }

	// get length
	long len = ftell(pFILE);

	if (len == 0) { fclose(pFILE); return; }

	buffer = new unsigned char[len];

	//fseek(pFILE, 0, SEEK_SET );
	rewind(pFILE);

	int read  = fread(buffer, sizeof(char), len, pFILE );

	fclose(pFILE);

	int size = len;

	writelog("size of buffer %d\n", size);

	if (buffer != 0)
	{
		for (int ctr=0; ctr<size; ctr++)
		{
			if (buffer[ctr]>=32 && buffer[ctr]<126)
			{
				writelog("%c",(char)buffer[ctr]);
			}
			else
			{
				writelog(".");
			}
		}
		writelog("\n");
	}


	if (isPreset)
	{
		fxProgram *pProgram = (fxProgram *)buffer;
		//bNeedSwap = true;
		if (bNeedSwap)
		{
			writelog("bNeedSwap is true\n");
			SwapBytes(pProgram->chunkMagic);
			SwapBytes(pProgram->byteSize);
			SwapBytes(pProgram->fxMagic);
			SwapBytes(pProgram->version);
			SwapBytes(pProgram->fxID);
			SwapBytes(pProgram->fxVersion);
			SwapBytes(pProgram->numParams);
		}


		 writelog("LoadChunk\n");
		 char * chunkMagic = (char *)&pProgram->chunkMagic;
		 writelog("chunkMagic %c%c%c%c\n", chunkMagic[0], chunkMagic[1], chunkMagic[2], chunkMagic[3]);
		 writelog("byteSize: %d\n", pProgram->byteSize);

		 char * fxMagic = (char *)&pProgram->fxMagic;
		 writelog("fxMagic %c%c%c%c\n", fxMagic[0], fxMagic[1], fxMagic[2], fxMagic[3]);
		 writelog("version: %d\n", pProgram->version);

		 char * fxID = (char *)&pProgram->fxID;
		 writelog("fxID %c%c%c%c\n", fxID[0], fxID[1], fxID[2], fxID[3]);
		 writelog("fxVersion: %d\n", pProgram->fxVersion);

		 writelog("numParams: %d\n", pProgram->numParams);
		 writelog("prgName: %s\n", pProgram->prgName);
		 writelog("ok! sizeof(fxProgram) is %d\n", sizeof(fxProgram));

		 writelog("pProgram->chunkMagic %d cMagic %d\n", pProgram->chunkMagic, cMagic);
		 writelog("pProgram->fxMagic %d chunkPresetMagic %d\n", pProgram->fxMagic, chunkPresetMagic);
		 if (pProgram->chunkMagic != cMagic) return;
		 if (pProgram->fxMagic != chunkPresetMagic) return;

		long newSize = size - sizeof(fxProgram);
		writelog("newSize %d\n", newSize);
		void * chunkData = (void *)&pProgram->params[1]; //move one over
		setChunk(chunkData, newSize, isPreset);
	}
	else
	{
		fxChunkSet *pChunkSet = (fxChunkSet *)buffer;
		//bNeedSwap = true;
		if (bNeedSwap)
		{
			writelog("bNeedSwap is true\n");
			SwapBytes(pChunkSet->chunkMagic);
			SwapBytes(pChunkSet->byteSize);
			SwapBytes(pChunkSet->fxMagic);
			SwapBytes(pChunkSet->version);
			SwapBytes(pChunkSet->fxID);
			SwapBytes(pChunkSet->fxVersion);
			SwapBytes(pChunkSet->numPrograms);
			SwapBytes(pChunkSet->chunkSize);
		}


		 writelog("LoadChunk\n");
		 char * chunkMagic = (char *)&pChunkSet->chunkMagic;
		 writelog("chunkMagic %c%c%c%c\n", chunkMagic[0], chunkMagic[1], chunkMagic[2], chunkMagic[3]);
		 writelog("byteSize: %d\n", pChunkSet->byteSize);

		 char * fxMagic = (char *)&pChunkSet->fxMagic;
		 writelog("fxMagic %c%c%c%c\n", fxMagic[0], fxMagic[1], fxMagic[2], fxMagic[3]);
		 writelog("version: %d\n", pChunkSet->version);

		 char * fxID = (char *)&pChunkSet->fxID;
		 writelog("fxID %c%c%c%c\n", fxID[0], fxID[1], fxID[2], fxID[3]);
		 writelog("fxVersion: %d\n", pChunkSet->fxVersion);

		 writelog("numPrograms: %d\n", pChunkSet->numPrograms);
		 writelog("chunkSize: %d\n", pChunkSet->chunkSize);
		 writelog("ok! sizeof(pChunkSet) is %d\n", sizeof(pChunkSet));

		 writelog("pChunkSet->chunkMagic %d cMagic %d\n", pChunkSet->chunkMagic, cMagic);
		 writelog("pChunkSet->fxMagic %d chunkBankMagic %d\n", pChunkSet->fxMagic, chunkBankMagic);
		 if (pChunkSet->chunkMagic != cMagic) return;
		 if (pChunkSet->fxMagic != chunkBankMagic) return;

		long newSize = size - sizeof(fxChunkSet) + 8;
		writelog("newSize %d\n", newSize);
		void * chunkData = (void *)&pChunkSet->chunk[0]; //move none
		writelog("size of Bank is %d\n", sizeof(t_CBX_Bank));
		setChunk(chunkData, newSize, isPreset);
	}
	if (buffer) delete [] buffer;
}

void SilverBox::SaveChunk(char * filename, bool isPreset)
{
	bool bNeedSwap = false;
	char szChnk[] = "CcnK";          /* set up swapping flag              */
	long lChnk = 'CcnK';
	bNeedSwap = !!memcmp(szChnk, &lChnk, 4);
#ifdef _LIMITED_
	long lCBox = 'cblx';
#else
	long lCBox = 'cbox';
#endif

	unsigned char * buffer = 0;
	unsigned char * saveData = 0;
	int dataSize = 0;
	int newSize;

	dataSize = getChunk((void **)&saveData, isPreset);

	if (isPreset)
	{
		// handle preset
		newSize = dataSize + sizeof(fxProgram);
		buffer = new unsigned char[newSize];

		fxProgram *pProgram = (fxProgram *)buffer;
		
		// set values

		pProgram->chunkMagic = cMagic;
		pProgram->byteSize = newSize - 8;

		pProgram->fxMagic = chunkPresetMagic;
		pProgram->version = 1;
		pProgram->fxID = lCBox;
		pProgram->fxVersion = 1;

		pProgram->numParams = 1;
		strcpy(pProgram->prgName, Bank.Name[currentPattern]);
		pProgram->params[0] = (float)dataSize;

		// do swap
		if (bNeedSwap)
		{
			writelog("bNeedSwap is true\n");
			SwapBytes(pProgram->chunkMagic);
			SwapBytes(pProgram->byteSize);
			SwapBytes(pProgram->fxMagic);
			SwapBytes(pProgram->version);
			SwapBytes(pProgram->fxID);
			SwapBytes(pProgram->fxVersion);
			SwapBytes(pProgram->numParams);
			SwapBytes(pProgram->params[0]);
		}
		// copy preset data
		memcpy(&pProgram->params[1], saveData, dataSize);
	}
	else
	{
		// handle bank
		newSize = dataSize + sizeof(fxChunkSet) - 8;
		buffer = new unsigned char[newSize];

		fxChunkSet *pChunkSet = (fxChunkSet *)buffer;

		pChunkSet->chunkMagic = cMagic;
		pChunkSet->byteSize = newSize - sizeof(fxChunkSet) - 8;

		pChunkSet->fxMagic = chunkBankMagic;
		pChunkSet->version = 1;
		pChunkSet->fxID = lCBox;
		pChunkSet->fxVersion = 1;

		pChunkSet->numPrograms = 100;
		memset(pChunkSet->future, 0, 128);

		pChunkSet->chunkSize = dataSize;

		if (bNeedSwap)
		{
			writelog("bNeedSwap is true\n");
			SwapBytes(pChunkSet->chunkMagic);
			SwapBytes(pChunkSet->byteSize);
			SwapBytes(pChunkSet->fxMagic);
			SwapBytes(pChunkSet->version);
			SwapBytes(pChunkSet->fxID);
			SwapBytes(pChunkSet->fxVersion);
			SwapBytes(pChunkSet->numPrograms);
			SwapBytes(pChunkSet->chunkSize);
		}
		// copy bank data
		memcpy(&pChunkSet->chunk[0], saveData, dataSize);
	}

	// save file
	FILE * pFILE;
	pFILE = fopen(filename, "wb"); // open for reading in binary

	if (pFILE == 0) return;

	int written = fwrite(buffer, 1, newSize, pFILE );

	if (written != newSize) { fclose(pFILE); return; }

	fclose(pFILE);

	// clear out buffer
	if (buffer) delete [] buffer;
}


// load file into buffer
// save file from buffer
//

// return size of buffer
long LoadFileIntoBuffer(char * file, unsigned char * buffer)
{

	FILE * pFILE;
	pFILE = fopen(file, "rb"); // open for reading in binary

	if (pFILE == 0) return 0;

	// move to end of file
	int seek = fseek(pFILE, 0, SEEK_END );


	if (seek != 0) { fclose(pFILE); return 0; }

	// get length
	long len = ftell(pFILE);

	if (len == 0) { fclose(pFILE); return 0; }

	buffer = new unsigned char[len];

	fseek(pFILE, 0, SEEK_SET );

	int read  = fread(&buffer, sizeof(char), len, pFILE );

	fclose(pFILE);

	return len;
}

int SaveFileFromBuffer(char * file, char * buffer, long len)
{
	return 0;
}

void SilverBox::dumpVstTimeInfo(VstTimeInfo timeinfo)
{
	writelog("VstTimeInfo dump:\n");
	writelog("samplePos: %f\n", timeinfo.samplePos);
	writelog("sampleRate: %f\n", timeinfo.sampleRate);
	writelog("nanoSeconds: %f\n", timeinfo.nanoSeconds);
	writelog("ppqPos: %f\n", timeinfo.ppqPos);
	writelog("tempo: %f\n", timeinfo.tempo);
	writelog("barStartPos: %f\n", timeinfo.barStartPos);
	writelog("cycleStartPos: %f\n", timeinfo.cycleStartPos);
	writelog("cycleEndPos: %f\n", timeinfo.cycleEndPos);

	writelog("timeSigNumerator: %d\n", timeinfo.timeSigNumerator);
	writelog("timeSigDenominator: %d\n", timeinfo.timeSigDenominator);
	writelog("smpteOffset: %d\n", timeinfo.smpteOffset);
	writelog("smpteFrameRate: %d\n", timeinfo.smpteFrameRate);
	writelog("samplesToNextClock: %d\n", timeinfo.samplesToNextClock);
	writelog("flags: %d\n", timeinfo.flags);
}

void SilverBox::dumpStep(int step, t_CB_Step & myStep)
{
	writelog("dumpStep step:%02d -- ", step);
	writelog("pitch: %03d slide: %d accent: %d", myStep.cPitch, myStep.bSlide, myStep.bAccent);
}
/*
struct VstTimeInfo
{
	double samplePos;			// current location
	double sampleRate;
	double nanoSeconds;			// system time
	double ppqPos;				// 1 ppq
	double tempo;				// in bpm
	double barStartPos;			// last bar start, in 1 ppq
	double cycleStartPos;		// 1 ppq
	double cycleEndPos;			// 1 ppq
	long timeSigNumerator;		// time signature
	long timeSigDenominator;
	long smpteOffset;
	long smpteFrameRate;		// 0:24, 1:25, 2:29.97, 3:30, 4:29.97 df, 5:30 df
	long samplesToNextClock;	// midi clock resolution (24 ppq), can be negative
	long flags;					// see below
};
*/