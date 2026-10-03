// simple test editor using the library vstgui.


#ifndef __SILVERBOX_EDITOR__
#include "SilverBoxEditor.h"
#endif

#ifndef __SILVERBOX__
#include "SilverBox.h"
#endif

#include <stdio.h>

// skin manager required includes
#if WIN32
#include <tchar.h>
#endif
#include <math.h>

// position struct
struct MyRect
{
	int X, Y, W, H;
};

enum
{
	kButtonWidth = 32,
	kButtonHeight = 32,
	kKnobWidth = 32,
	kKnobHeight = 32,
	kKnobFrames = 61,
	kFaderWidth = 22,
	kFaderHeight = 199,
	kSModeWidth = 31,
	kSModeHeight = 15,
	
	kSliderWidth = 31,
	kSliderHeight = 76,

	kStepButtonWidth = 28,
	kStepButtonHeight = 32,
	kStepLEDWidth = 32,
	kStepLEDHeight = 15,
	kPresetHeight = 21,
	kPresetWidth = 16,

	kThinButtonWidth = 16,
	kThinButtonHeight = 32,

	kPitchDisplayHeight = 12
};

// positions
#ifdef _MIDI_LEARN_
MyRect CtrlPos[kNumParams+159] = 
#else
MyRect CtrlPos[kNumParams+156] = 
#endif
{
	{20, 18, 65, 65}, //kVolume

	{87, 52, 37, 38}, //kWaveform

	{132, 46, 49, 58}, //kTuning
	{184, 46, 48, 58}, //kCutOffFreq
	{235, 46, 48, 58}, //kResonance
	{285, 46, 48, 58}, //kEnvMod
	{338, 46, 48, 58}, //kDecay
	{389, 46, 48, 58}, //kAccent
	
	{0, 0, 0, 0},

#ifdef _MIDI_LEARN_
	{32, 128, kSModeWidth, kSModeHeight},  //kMidiLearn
#endif
	{436, 19, 63, 66}, //kTempo
//	{416, 64, kKnobWidth, kKnobHeight}, //kNextPattern
#ifndef _LIMITED_
	{63,  184, 47, 27}, //kClear
	{63,  121, 47, 27}, //kCopy
	{63,  152, 47, 27}, //kPaste
#ifdef _RANDOM_PATTERNS_
	{119, 121, 47, 27}, //kRandom1
	{119, 152, 47, 27}, //kRandom2
#endif
#endif
	{499, 51, 47, 27}, //kRunStop
	{499, 22, 47, 27}, //kSync

	//250
	{39,  250, 31, 16}, //kAccent01
	{70,  250, 31, 16}, //kAccent02
	{101, 250, 31, 16}, //kAccent03
	{132, 250, 31, 16}, //kAccent04
	{163, 250, 31, 16}, //kAccent05
	{194, 250, 31, 16}, //kAccent06
	{225, 250, 31, 16}, //kAccent07
	{256, 250, 31, 16}, //kAccent08
	{287, 250, 31, 16}, //kAccent09
	{318, 250, 31, 16}, //kAccent10
	{349, 250, 31, 16}, //kAccent11
	{380, 250, 31, 16}, //kAccent12
	{411, 250, 31, 16}, //kAccent13
	{442, 250, 31, 16}, //kAccent14
	{473, 250, 31, 16}, //kAccent15
	{504, 250, 31, 16}, //kAccent16

	{39,  296, kSliderWidth, kSliderHeight}, //kPitch01
	{70,  296, kSliderWidth, kSliderHeight}, //kPitch02
	{101, 296, kSliderWidth, kSliderHeight}, //kPitch03
	{132, 296, kSliderWidth, kSliderHeight}, //kPitch04
	{163, 296, kSliderWidth, kSliderHeight}, //kPitch05
	{194, 296, kSliderWidth, kSliderHeight}, //kPitch06
	{225, 296, kSliderWidth, kSliderHeight}, //kPitch07
	{256, 296, kSliderWidth, kSliderHeight}, //kPitch08
	{287, 296, kSliderWidth, kSliderHeight}, //kPitch09
	{318, 296, kSliderWidth, kSliderHeight}, //kPitch10
	{349, 296, kSliderWidth, kSliderHeight}, //kPitch11
	{380, 296, kSliderWidth, kSliderHeight}, //kPitch12
	{411, 296, kSliderWidth, kSliderHeight}, //kPitch13
	{442, 296, kSliderWidth, kSliderHeight}, //kPitch14
	{473, 296, kSliderWidth, kSliderHeight}, //kPitch15
	{504, 296, kSliderWidth, kSliderHeight}, //kPitch16

	{39,  266, 31, 15}, //kSlide01
	{70,  266, 31, 15}, //kSlide02
	{101, 266, 31, 15}, //kSlide03
	{132, 266, 31, 15}, //kSlide04
	{163, 266, 31, 15}, //kSlide05
	{194, 266, 31, 15}, //kSlide06
	{225, 266, 31, 15}, //kSlide07
	{256, 266, 31, 15}, //kSlide08
	{287, 266, 31, 15}, //kSlide09
	{318, 266, 31, 15}, //kSlide10
	{349, 266, 31, 15}, //kSlide11
	{380, 266, 31, 15}, //kSlide12
	{411, 266, 31, 15}, //kSlide13
	{442, 266, 31, 15}, //kSlide14
	{473, 266, 31, 15}, //kSlide15
	{504, 266, 31, 15}, //kSlide16

	{42,  283, 25, 10}, //kPitchDisplay01
	{73,  283, 25, 10}, //kPitchDisplay02
	{104, 283, 25, 10}, //kPitchDisplay03
	{135, 283, 25, 10}, //kPitchDisplay04
	{166, 283, 25, 10}, //kPitchDisplay05
	{197, 283, 25, 10}, //kPitchDisplay06
	{228, 283, 25, 10}, //kPitchDisplay07
	{259, 283, 25, 10}, //kPitchDisplay08
	{290, 283, 25, 10}, //kPitchDisplay09
	{321, 283, 25, 10}, //kPitchDisplay10
	{352, 283, 25, 10}, //kPitchDisplay11
	{383, 283, 25, 10}, //kPitchDisplay12
	{414, 283, 25, 10}, //kPitchDisplay13
	{445, 283, 25, 10}, //kPitchDisplay14
	{476, 283, 25, 10}, //kPitchDisplay15
	{507, 283, 25, 10},  //kPitchDisplay16

	{450, 90, 30, 9},  //kTempoDisplay
#ifndef _LIMITED_
	{227, 127, 32, 15},  //kCurrentPatternDisplay
	{322, 127, 32, 15},  //kNextPatternDisplay
#endif
	{39, 238, kSModeWidth, 12},  //kStep01,
	{70, 238, kSModeWidth, 12},  //kStep02,
	{101, 238, kSModeWidth, 12},  //kStep03,
	{132, 238, kSModeWidth, 12},  //kStep04,
	{163, 238, kSModeWidth, 12},  //kStep05,
	{194, 238, kSModeWidth, 12},  //kStep06,
	{225, 238, kSModeWidth, 12},  //kStep07,
	{256, 238, kSModeWidth, 12},  //kStep08,
	{287, 238, kSModeWidth, 12},  //kStep09,
	{318, 238, kSModeWidth, 12},  //kStep10,
	{349, 238, kSModeWidth, 12},  //kStep11,
	{380, 238, kSModeWidth, 12},  //kStep12,
	{411, 238, kSModeWidth, 12},  //kStep13,
	{442, 238, kSModeWidth, 12},  //kStep14,
	{473, 238, kSModeWidth, 12},  //kStep15,
	{504, 238, kSModeWidth, 12},  //kStep16
#ifndef _LIMITED_
	{209, 158, kPresetWidth, kPresetHeight},	//	kBank00
	{225, 158, kPresetWidth, kPresetHeight},	//	kBank01
	{241, 158, kPresetWidth, kPresetHeight},	//	kBank02
	{257, 158, kPresetWidth, kPresetHeight},	//	kBank03
	{273, 158, kPresetWidth, kPresetHeight},	//	kBank04
	{289, 158, kPresetWidth, kPresetHeight},	//	kBank05
	{305, 158, kPresetWidth, kPresetHeight},	//	kBank06
	{321, 158, kPresetWidth, kPresetHeight},	//	kBank07
	{337, 158, kPresetWidth, kPresetHeight},	//	kBank08
	{353, 158, kPresetWidth, kPresetHeight},	//	kBank09

	{209, 179, kPresetWidth, kPresetHeight},	//	kPreset00
	{225, 179, kPresetWidth, kPresetHeight},	//	kPreset01
	{241, 179, kPresetWidth, kPresetHeight},	//	kPreset02
	{257, 179, kPresetWidth, kPresetHeight},	//	kPreset03
	{273, 179, kPresetWidth, kPresetHeight},	//	kPreset04
	{289, 179, kPresetWidth, kPresetHeight},	//	kPreset05
	{305, 179, kPresetWidth, kPresetHeight},	//	kPreset06
	{321, 179, kPresetWidth, kPresetHeight},	//	kPreset07
	{337, 179, kPresetWidth, kPresetHeight},	//	kPreset08
	{353, 179, kPresetWidth, kPresetHeight},	//	kPreset09
#endif
	{133, 402, 23, 47},	//	kNoteOff

	{172, 407, 14, 38},	//	kNote01
	{186, 407, 14, 38},	//	kNote02
	{200, 407, 14, 38},	//	kNote03
	{214, 407, 14, 38},	//	kNote04
	{228, 407, 14, 38},	//	kNote05
	{242, 407, 14, 38},	//	kNote06
	{256, 407, 14, 38},	//	kNote07
	{270, 407, 14, 38},	//	kNote08
	{284, 407, 14, 38},	//	kNote09
	{298, 407, 14, 38},	//	kNote10
	{312, 407, 14, 38},	//	kNote11
	{326, 407, 14, 38},	//	kNote12

	{358, 407, 14, 38},	//	kOctave01
	{372, 407, 14, 38},	//	kOctave02
	{386, 407, 14, 38},	//	kOctave03
	{400, 407, 14, 38},	//	kOctave04
	{414, 407, 14, 38},	//	kOctave05

	{39, 372, 31, 18},  //kStepEdit01,
	{70, 372, 31, 18},  //kStepEdit02,
	{101, 372, 31, 18},  //kStepEdit03,
	{132, 372, 31, 18},  //kStepEdit04,
	{163, 372, 31, 18},  //kStepEdit05,
	{194, 372, 31, 18},  //kStepEdit06,
	{225, 372, 31, 18},  //kStepEdit07,
	{256, 372, 31, 18},  //kStepEdit08,
	{287, 372, 31, 18},  //kStepEdit09,
	{319, 372, 31, 18},  //kStepEdit10,
	{349, 372, 31, 18},  //kStepEdit11,
	{380, 372, 31, 18},  //kStepEdit12,
	{411, 372, 31, 18},  //kStepEdit13,
	{442, 372, 31, 18},  //kStepEdit14,
	{473, 372, 31, 18},  //kStepEdit15,
	{504, 372, 31, 18},  //kStepEdit16
#ifndef _LIMITED_

#ifdef _BANKS_
	{592,  64, kSModeWidth, kSModeHeight},  //kLoadFXP,
	{544,  64, kSModeWidth, kSModeHeight},  //kLoadFXB,
	{592,  96, kSModeWidth, kSModeHeight},  //kSaveFXP,
	{544,  96, kSModeWidth, kSModeHeight},  //kSaveFXB,
#endif
#ifdef _MIDI_LEARN_
	{ 80, 128, kSModeWidth, kSModeHeight},  //kLoadMidiMap,
	{128, 128, kSModeWidth, kSModeHeight},  //kSaveMidiMap,
#endif
//	{576, 160, kSModeWidth, kSModeHeight},  //kPModePattern
	{415, 121, 47, 27},  //kCycle,
	{415, 152, 47, 27},  //kSetCycleStart,
	{415, 184, 47, 27},  //kSetCycleFinish,
	{479, 152, 32, 15},  //kStartDisplay,
	{479, 190, 32, 15}   //kFinishDisplay
#endif
};

//-----------------------------------------------------------------------------
// resource id's
enum {
	// bitmaps
	kBackgroundId = 128,
	kSplashScreenId,
	kVolKnob = 130,
	kWaveformId,
	kTuneId,
	kCutoffId,
	kResoId,
	kEnvId,
	kDecayId,
	kAccId,
	kTempoId,
	kSyncId,
	kRunId = 140,
	kCopyId,
	kPasteId,
	kClearId,
#ifdef _RANDOM_PATTERNS_
	kRandom1Id,
	kRandom2Id,
#endif
	kLoopId,
	kLoopStartId,
	kLoopEndId,

	kTempoDigitsId = 149,

	kGroup0Id = 150,
	kGroup1Id = 151,
	kGroup2Id = 152,
	kGroup3Id = 153,
	kGroup4Id = 154,
	kGroup5Id = 155,
	kGroup6Id = 156,
	kGroup7Id = 157,
	kGroup8Id = 158,
	kGroup9Id = 159,

	kPreset0Id = 160,
	kPreset1Id = 161,
	kPreset2Id = 162,
	kPreset3Id = 163,
	kPreset4Id = 164,
	kPreset5Id = 165,
	kPreset6Id = 166,
	kPreset7Id = 167,
	kPreset8Id = 168,
	kPreset9Id = 169,

	kLED01Id = 170,
	kAccentId = 171,
	kSlideId = 172,
	kPitchDisplayId= 173,
	kSliderBackId = 174,
	kSliderThumbId = 175,

	kOffId = 176,
	kWhiteId = 177,
	kBlackId = 178,
	kOctave0 = 179,
	kOctave1 = 180,
	kOctave2 = 181,
	kOctave3 = 182,
	kOctave4 = 183,

	kLED02Id = 184,
	kSliderBack2Id = 185,
	kStepEditId = 186,
	kSliderThumb2Id = 187,

	kNumberDigitsId = 188,


	kDisplayX = 31,
	kDisplayY = 459,
	kDisplayWidth = 122,  //30,
	kDisplayHeight = 14

};

#if MOTIF
// resource for MOTIF (format XPM)
#include "bmp00128.xpm"
#include "bmp00129.xpm"
#include "bmp00130.xpm"

CResTable xpmResources = {
  {kBackgroundId , bmp00128},
  {kFaderBodyId  , bmp00129},
  {kFaderHandleId, bmp00130},
  {0, 0}
};
#endif

//-----------------------------------------------------------------------------
// prototype string convert float -> percent

char gszFileName[1024];

void FileNameStringConvert(float value, char * text)
{
//	float2StringConvert(value, text);
	strcpy(text, gszFileName);
}

char * cNoteDesc2[61] = { "OFF",
	"C-0","C#0","D-0","D#0","E-0","F-0","F#0","G-0","G#0","A-0","A#0","B-0", //1
	"C-1","C#1","D-1","D#1","E-1","F-1","F#1","G-1","G#1","A-1","A#1","B-1", //13
	"C-2","C#2","D-2","D#2","E-2","F-2","F#2","G-2","G#2","A-2","A#2","B-2", //25
	"C-3","C#3","D-3","D#3","E-3","F-3","F#3","G-3","G#3","A-3","A#3","B-3",
	"C-4","C#4","D-4","D#4","E-4","F-4","F#4","G-4","G#4","A-4","A#4","B-4"
	//"C-5","C#5","D-5","D#5","E-5","F-5","F#5","G-5","G#5","A-5","A#5","B-5"
};

//-----------------------------------------------------------------------------
// ADEditor class implementation
//-----------------------------------------------------------------------------
SilverBoxEditor::SilverBoxEditor (AudioEffect *effect)
 : AEffGUIEditor (effect) 
{
	int a;

	bmpOnOffButton = 0;
	bmpSplashScreen = 0;


	bmpSliderBack = 0;
	bmpSliderThumb = 0;
	bmpSliderThumb2 = 0;

	// new bitmaps
	for (a=0; a<10; a++)
	{
		bmpGroupBtn[a] = 0;
		bmpPresetBtn[a] = 0;
	}
	bmpOffButton = 0;
	bmpKeyWhite = 0;
	bmpKeyBlack = 0;;
	for (a=0; a<5; a++)
	{
		bmpOctave[a] = 0;
	}
	bmpAccent = 0;
	bmpSlide = 0;

	bmpRun = 0;
	bmpSync = 0;

	bmpLoop = 0;
	bmpLoopStart = 0;
	bmpLoopEnd = 0;

	bmpCopy = 0;
	bmpPaste = 0;
	bmpClear = 0;
#ifdef _RANDOM_PATTERNS_
	bmpRnd1 = 0;
	bmpRnd2 = 0;
#endif

	bmpVolume = 0;
	bmpTempo = 0;

	bmpTune = 0;
	bmpCut =0;
	bmpReso = 0;
	bmpEnv = 0;
	bmpDecay = 0;
	bmpAcc = 0;
	
	bmpLED01 = 0;
	bmpLED02 = 0;
	bmpSliderBack2 = 0;
	bmpStepEdit = 0;

	bmpPitchDisplay = 0;
	
	bmpTempoDigits = 0;
	bmpNumberDigits = 0;

#ifdef _DEBUG_STRING_
	pdFileName = 0;
#endif

	teDisplay = 0;

	// splash screen
	ssAbout = 0;


#ifdef ___WAVEFORM_KNOB__
	kbWaveform = 0;
#else
	btnWaveform = 0;
#endif

	for (a=0; a<10; a++)
	{
		btnPreset[a]=0;
		btnBank[a]=0;
	}

	btnNoteOff = 0;
	for (int note=0;note<12;note++) btnNote[note] = 0;
	for (int octave=0;octave<5;octave++) btnOctave[octave] = 0;

	// pattern management
	kbClear = 0;
	kbCopy = 0;
	kbPaste = 0;
	kbRandom1 = 0;
	kbRandom2 = 0;

	// play/sync
	btnRunStop = 0;
	btnSync = 0;	// to sync to host

	for (a=0; a<16; a++)
	{
		btnAccent[a] = 0;
		btnSlide[a] = 0;
		vsPitch[a] = 0;
		mbPitch[a] = 0;
		mbStep[a]=0;
		btnStepEdit[a]=0;
	}

	for (a=0;a<4;a++)
	{
		btnPage[a] = 0;
	}

#ifdef _BANKS_
	kbLoadFXP = 0;
	kbSaveFXP = 0;
	kbLoadFXB = 0;
	kbSaveFXB = 0;
#endif
	kbLoadMidiMap = 0;
	kbSaveMidiMap = 0;

	btnCycle = 0;
	kbSetCycleStart = 0;
	kbSetCycleFinish = 0;
	
	pdStartPattern = 0;
	pdFinishPattern = 0;

	btnStepEditMode = 0;

	// load the background bitmap
	// we don't need to load all bitmaps, this could be done when open is called
	hBackground  = new CBitmap (kBackgroundId); 

	// init the size of the plugin
	rect.left   = 0;
	rect.top    = 0;
	rect.right  = (short)hBackground->getWidth ();
	rect.bottom = (short)hBackground->getHeight ();

	vsKey = 0;

	teVersion = 0;

	memset(gszFileName, 0, 1024);
	strcpy(gszFileName, "????");
	memset(_szLastPath, 0, 1024);
	strcpy(_szLastPath, "C:\\");
}

//-----------------------------------------------------------------------------
SilverBoxEditor::~SilverBoxEditor ()
{
	// free background bitmap
	if (hBackground)
		hBackground->forget ();
	hBackground = 0;
}

void SilverBoxEditor::SetControlSize(int control_id, CRect & size)
{
	size (CtrlPos[control_id].X, CtrlPos[control_id].Y, CtrlPos[control_id].X + CtrlPos[control_id].W, CtrlPos[control_id].Y + CtrlPos[control_id].H);
}

void SilverBoxEditor::SetDisplaySize(int control_id, CRect & size)
{
	SetControlSize(control_id, size);
	size.left = (size.left - ((kDisplayWidth - CtrlPos[control_id].W)/2));
	size.top = (size.bottom + 4); 
	size.right = (size.left + kDisplayWidth);
	size.bottom = (size.top + kDisplayHeight);
}

//-----------------------------------------------------------------------------
long SilverBoxEditor::open (void *ptr)
{
	int a;
	// !!! always call this !!!
	AEffGUIEditor::open (ptr);

	// load some bitmaps
	if (!bmpOnOffButton)
		bmpOnOffButton = new CBitmap(kWaveformId);

	if (!bmpSplashScreen)
		bmpSplashScreen = new CBitmap(kSplashScreenId);

	if (!bmpSliderBack)
		bmpSliderBack = new CBitmap(kSliderBackId);

	if (!bmpSliderThumb)
		bmpSliderThumb = new CBitmap(kSliderThumbId);

	if (!bmpSliderThumb2)
		bmpSliderThumb2 = new CBitmap(kSliderThumb2Id);

	if (!bmpVolume)
		bmpVolume = new CBitmap(kVolKnob);

	if (!bmpTune) bmpTune = new CBitmap(kTuneId);
	if (!bmpCut) bmpCut = new CBitmap(kCutoffId);
	if (!bmpReso) bmpReso = new CBitmap(kResoId);
	if (!bmpEnv) bmpEnv = new CBitmap(kEnvId);
	if (!bmpDecay) bmpDecay = new CBitmap(kDecayId);
	if (!bmpAcc) bmpAcc = new CBitmap(kAccId);

	if (!bmpTempo) bmpTempo = new CBitmap(kTempoId);

	if (!bmpSync) bmpSync = new CBitmap(kSyncId);
	if (!bmpRun) bmpRun = new CBitmap(kRunId);
	if (!bmpCopy) bmpCopy = new CBitmap(kCopyId);
	if (!bmpPaste) bmpPaste = new CBitmap(kPasteId);
	if (!bmpClear) bmpClear = new CBitmap(kClearId);

#ifdef _RANDOM_PATTERNS_
	if (!bmpRnd1) bmpRnd1 = new CBitmap(kRandom1Id);
	if (!bmpRnd2) bmpRnd2 = new CBitmap(kRandom2Id);
#endif

	if (!bmpLoop) bmpLoop = new CBitmap(kLoopId);
	if (!bmpLoopStart) bmpLoopStart = new CBitmap(kLoopStartId);
	if (!bmpLoopEnd) bmpLoopEnd = new CBitmap(kLoopEndId);

	if (!bmpAccent) bmpAccent = new CBitmap(kAccentId);
	if (!bmpSlide) bmpSlide = new CBitmap(kSlideId);

	if (!bmpOffButton) bmpOffButton = new CBitmap(kOffId);
	if (!bmpKeyWhite) bmpKeyWhite = new CBitmap(kWhiteId);
	if (!bmpKeyBlack) bmpKeyBlack = new CBitmap(kBlackId);

	if (!bmpOctave[0]) bmpOctave[0] = new CBitmap(kOctave0);
	if (!bmpOctave[1]) bmpOctave[1] = new CBitmap(kOctave1);
	if (!bmpOctave[2]) bmpOctave[2] = new CBitmap(kOctave2);
	if (!bmpOctave[3]) bmpOctave[3] = new CBitmap(kOctave3);
	if (!bmpOctave[4]) bmpOctave[4] = new CBitmap(kOctave4);

	for (a=0; a<10; a++)
	{
		if (!bmpGroupBtn[a]) bmpGroupBtn[a] = new CBitmap(kGroup0Id + a);
		if (!bmpPresetBtn[a]) bmpPresetBtn[a] = new CBitmap(kPreset0Id + a);
	}

	
	if (!bmpLED01) bmpLED01 = new CBitmap(kLED01Id);
	if (!bmpLED02) bmpLED02 = new CBitmap(kLED02Id);
	if (!bmpSliderBack2) bmpSliderBack2 = new CBitmap(kSliderBack2Id);
	if (!bmpStepEdit) bmpStepEdit = new CBitmap(kStepEditId);

	if (!bmpPitchDisplay) bmpPitchDisplay = new CBitmap(kPitchDisplayId);

	if (!bmpTempoDigits) bmpTempoDigits = new CBitmap(kTempoDigitsId);
	if (!bmpNumberDigits) bmpNumberDigits = new CBitmap(kNumberDigitsId);

	//--init background frame-----------------------------------------------
	CRect size (0, 0, hBackground->getWidth (), hBackground->getHeight ());
	frame = new CFrame (size, ptr, this);
	frame->setBackground (hBackground);

	CPoint point (0, 0);
	CPoint offset (0, 0);

	SetControlSize(kVolume, size);
	akVolume = new CAnimKnob (size, this, kVolume, 60, 65, bmpVolume, offset);
	akVolume->setValue (effect->getParameter (kVolume));
	frame->addView (akVolume);

#ifdef ___WAVEFORM_KNOB__
	SetControlSize(kWaveform, size);
	kbWaveform = new CKnob(size, this, kWaveform, bmpSmallKnobBack, bmpSmallKnobKnob, offset);
	kbWaveform->setValue(effect->getParameter(kWaveform));
	frame->addView(kbWaveform);
#else
	SetControlSize(kWaveform, size);
	btnWaveform = new COnOffButton (size, this, kWaveform, bmpOnOffButton);
	btnWaveform->setValue (effect->getParameter (kWaveform));
	frame->addView (btnWaveform);
#endif

	SetControlSize(kTuning, size);
	akTuning = new CAnimKnob (size, this, kTuning, 60, 58, bmpTune, offset);
	akTuning->setValue (effect->getParameter (kTuning));
	frame->addView (akTuning);

	SetControlSize(kCutOffFreq, size);
	akCutOffFreq = new CAnimKnob (size, this, kCutOffFreq, 60, 58, bmpCut, offset);
	akCutOffFreq->setValue (effect->getParameter (kCutOffFreq));
	frame->addView (akCutOffFreq);

	SetControlSize(kResonance, size);
	akResonance = new CAnimKnob (size, this, kResonance, 60, 58, bmpReso, offset);
	akResonance->setValue (effect->getParameter (kResonance));
	frame->addView (akResonance);

	SetControlSize(kEnvMod, size);
	akEnvMod = new CAnimKnob (size, this, kEnvMod, 60, 58, bmpEnv, offset);
	akEnvMod->setValue (effect->getParameter (kEnvMod));
	frame->addView (akEnvMod);

	SetControlSize(kDecay, size);
	akDecay = new CAnimKnob (size, this, kDecay, 60, 58, bmpDecay, offset);
	akDecay->setValue (effect->getParameter (kDecay));
	frame->addView (akDecay);

	SetControlSize(kAccent, size);
	akAccent = new CAnimKnob (size, this, kAccent, 60, 58, bmpAcc, offset);
	akAccent->setValue (effect->getParameter (kAccent));
	frame->addView (akAccent);

	SetControlSize(kTempo, size);
	akTempo = new CAnimKnob (size, this, kTempo, 60, 66, bmpTempo, offset);
	akTempo->setValue (effect->getParameter (kTempo));
	frame->addView (akTempo);

#ifdef _MIDI_LEARN_
	// midi map
	SetControlSize(kLoadMidiMap, size);
	kbLoadMidiMap = new CKickButton (size, this, kLoadMidiMap, (bmpScratchMode->getHeight()/2), bmpScratchMode, point);
	kbLoadMidiMap->setValue (effect->getParameter (kLoadMidiMap));
	frame->addView (kbLoadMidiMap);

	SetControlSize(kSaveMidiMap, size);
	kbSaveMidiMap = new CKickButton (size, this, kSaveMidiMap, (bmpScratchMode->getHeight()/2), bmpScratchMode, point);
	kbSaveMidiMap->setValue (effect->getParameter (kSaveMidiMap));
	frame->addView (kbSaveMidiMap);
#endif

#ifdef _BANKS_
	// bank/preset
	SetControlSize(kLoadFXP, size);
	kbLoadFXP = new CKickButton (size, this, kLoadFXP, (bmpScratchMode->getHeight()/2), bmpScratchMode, point);
	kbLoadFXP->setValue (effect->getParameter (kLoadFXP));
	frame->addView (kbLoadFXP);

	SetControlSize(kLoadFXB, size);
	kbLoadFXB = new CKickButton (size, this, kLoadFXB, (bmpScratchMode->getHeight()/2), bmpScratchMode, point);
	kbLoadFXB->setValue (effect->getParameter (kLoadFXB));
	frame->addView (kbLoadFXB);

	SetControlSize(kSaveFXP, size);
	kbSaveFXP = new CKickButton (size, this, kSaveFXP, (bmpScratchMode->getHeight()/2), bmpScratchMode, point);
	kbSaveFXP->setValue (effect->getParameter (kSaveFXP));
	frame->addView (kbSaveFXP);

	SetControlSize(kSaveFXB, size);
	kbSaveFXB = new CKickButton (size, this, kSaveFXB, (bmpScratchMode->getHeight()/2), bmpScratchMode, point);
	kbSaveFXB->setValue (effect->getParameter (kSaveFXB));
	frame->addView (kbSaveFXB);
#endif
	//

#ifndef _LIMITED_
	SetControlSize(kClear, size);
	kbClear = new CKickButton (size, this, kClear, (bmpClear->getHeight()/2), bmpClear, point);
	kbClear->setValue (effect->getParameter (kClear));
	frame->addView (kbClear);

	SetControlSize(kCopy, size);
	kbCopy = new CKickButton (size, this, kCopy, (bmpCopy->getHeight()/2), bmpCopy, point);
	kbCopy->setValue (effect->getParameter (kCopy));
	frame->addView (kbCopy);

	SetControlSize(kPaste, size);
	kbPaste = new CKickButton (size, this, kPaste, (bmpPaste->getHeight()/2), bmpPaste, point);
	kbPaste->setValue (effect->getParameter (kPaste));
	frame->addView (kbPaste);

#ifdef _RANDOM_PATTERNS_
	SetControlSize(kRandom1, size);
	kbRandom1 = new CKickButton (size, this, kRandom1, (bmpRnd1->getHeight()/2), bmpRnd1, point);
	kbRandom1->setValue (effect->getParameter (kRandom1));
	frame->addView (kbRandom1);

	SetControlSize(kRandom2, size);
	kbRandom2 = new CKickButton (size, this, kRandom2, (bmpRnd2->getHeight()/2), bmpRnd2, point);
	kbRandom2->setValue (effect->getParameter (kRandom2));
	frame->addView (kbRandom2);

#endif
#endif
	long xpos2[7] = { 450, 460, 470, 90, 40, 50 ,60 };
	long ypos2[7] = {  90,  90,  90, 90, 90, 90 ,90 };

	SetControlSize(kTempoDisplay, size);
	sdTempo = new CSpecialDigit (size, this, kTempoDisplay, 120, 3, (long *)&xpos2, (long *)&ypos2, 10, 9, bmpTempoDigits);
	sdTempo->setValue(effect->getParameter(kTempoDisplay));
	frame->addView (sdTempo);
#ifndef _LIMITED_
	// PATTERNS
	long xpos3[7] = { 227, 243, 0, 0, 0, 0 ,0 };
	long ypos3[7] = { 127, 127, 0, 0, 0, 0 ,0 };

	SetControlSize(kCurrentPatternDisplay, size);
	sdCurrPattern = new CSpecialDigit (size, this, kCurrentPatternDisplay,0, 2, (long *)&xpos3, (long *)&ypos3, 16, 15, bmpNumberDigits);
	sdCurrPattern->setValue(effect->getParameter(kCurrentPatternDisplay));
	frame->addView(sdCurrPattern);

  	long xpos4[7] = { 322, 338, 0, 0, 0, 0 ,0 };;

	SetControlSize(kNextPatternDisplay, size);
	sdNextPattern = new CSpecialDigit (size, this, kNextPatternDisplay,0, 2, (long *)&xpos4, (long *)&ypos3, 16, 15, bmpNumberDigits);
	sdNextPattern->setValue(effect->getParameter(kNextPatternDisplay));
	frame->addView(sdNextPattern);

	//CYCLE
#ifdef _CYCLE_
	long xpos5[7] = { 479, 495, 0, 0, 0, 0 ,0 };
	long ypos5[7] = { 158, 158, 0, 0, 0, 0 ,0 };

	SetControlSize(kStartDisplay, size);
	sdStartCycle = new CSpecialDigit (size, this, kStartDisplay,0, 2, (long *)&xpos5, (long *)&ypos5, 16, 15, bmpNumberDigits);
	sdStartCycle->setValue(effect->getParameter(kStartDisplay));
	frame->addView(sdStartCycle);

  	long ypos6[7] = { 190, 190, 0, 0, 0, 0 ,0 };;

	SetControlSize(kFinishDisplay, size);
	sdFinishCycle = new CSpecialDigit (size, this, kFinishDisplay,0, 2, (long *)&xpos5, (long *)&ypos6, 16, 15, bmpNumberDigits);
	sdFinishCycle->setValue(effect->getParameter(kFinishDisplay));
	frame->addView(sdFinishCycle);
#endif
#endif
	SetControlSize(kRunStop, size);
	btnRunStop = new COnOffButton (size, this, kRunStop, bmpRun);
	btnRunStop->setValue (effect->getParameter (kRunStop));
	frame->addView (btnRunStop);

	SetControlSize(kSync, size);
	btnSync = new COnOffButton (size, this, kSync, bmpSync);
	btnSync->setValue (effect->getParameter (kSync));
	frame->addView (btnSync);

#ifdef _MIDI_LEARN_
	SetControlSize(kMidiLearn, size);
	btnMidiLearn = new COnOffButton (size, this, kMidiLearn, bmpScratchMode);
	btnMidiLearn->setValue (effect->getParameter (kMidiLearn));
	frame->addView (btnMidiLearn);
#endif
#ifndef _LIMITED_
#ifdef _CYCLE_
	// cycle
	SetControlSize(kCycle, size);
	btnCycle = new COnOffButton(size, this, kCycle, bmpLoop);
	btnCycle->setValue(effect->getParameter(kCycle));
	frame->addView(btnCycle);

	SetControlSize(kSetCycleStart, size);
	kbSetCycleStart = new CKickButton(size, this, kSetCycleStart, (bmpLoopStart->getHeight()/2), bmpLoopStart, point);
	kbSetCycleStart->setValue(effect->getParameter(kSetCycleStart));
	frame->addView(kbSetCycleStart);

	SetControlSize(kSetCycleFinish, size);
	kbSetCycleFinish = new CKickButton(size, this, kSetCycleFinish, (bmpLoopEnd->getHeight()/2), bmpLoopEnd, point);
	kbSetCycleFinish->setValue(effect->getParameter(kSetCycleFinish));
	frame->addView(kbSetCycleFinish);
#endif

	//bank/preset
	for (int preset=0; preset<10; preset++)
	{
		SetControlSize(kBank00 + preset, size);
		//btnBank[preset] = new COnOffButton (size, this, kBank00 + preset, bmpPreset);
		btnBank[preset] = new CMovieButton (size, this, kBank00 + preset, kPresetHeight, bmpGroupBtn[preset], offset); 
		btnBank[preset]->setValue (effect->getParameter(kBank00 + preset));
		frame->addView(btnBank[preset]);

		SetControlSize(kPreset00 + preset, size);
		//btnPreset[preset] = new COnOffButton (size, this, kPreset00 + preset, bmpPreset);
		btnPreset[preset] = new CMovieButton (size, this, kPreset00 + preset, kPresetHeight, bmpPresetBtn[preset], offset); 
		btnPreset[preset]->setValue (effect->getParameter(kPreset00 + preset));
		frame->addView(btnPreset[preset]);
	}
#endif
	SetControlSize(kNoteOff, size);
	btnNoteOff = new CMovieButton (size, this, kNoteOff, (bmpOffButton->getHeight()/2), bmpOffButton, offset); 
	btnNoteOff->setValue (effect->getParameter(kNoteOff));
	frame->addView(btnNoteOff);

	CBitmap * bmpTemp;
	for (int note=0; note<12; note++)
	{
		switch(note)
		{
		case 1:
		case 3:
		case 6:
		case 8:
		case 10:
			bmpTemp = bmpKeyBlack;
			break;
		default:
			bmpTemp = bmpKeyWhite;
		}
		SetControlSize(kNote01 + note, size);
		btnNote[note] = new CMovieButton (size, this, kNote01 + note, (bmpTemp->getHeight()/2), bmpTemp, offset); 
		btnNote[note]->setValue (effect->getParameter(kNote01 + note));
		frame->addView(btnNote[note]);
	}

	for (int octave=0; octave<5; octave++)
	{
		SetControlSize(kOctave01 + octave, size);
		btnOctave[octave] = new CMovieButton (size, this, kOctave01 + octave, (bmpOctave[octave]->getHeight()/2), bmpOctave[octave], offset); 
		btnOctave[octave]->setValue (effect->getParameter(kOctave01 + octave));
		frame->addView(btnOctave[octave]);
	}

	// 16 step LED
	for (int step=0; step<16; step++)
	{
		switch(step)
		{
		case 0:
		case 4:
		case 8:
		case 12:
			bmpTemp = bmpLED02;
			break;
		default:
			bmpTemp = bmpLED01;
		}
		SetControlSize(kStep01 + step, size);
		mbStep[step] = new CMovieBitmap (size, this, 0, 2, (bmpTemp->getHeight()/2), bmpTemp, offset);
		mbStep[step]->setValue (effect->getParameter(kStep01 + step));
		frame->addView (mbStep[step]);
	}

	for (int stepedit=0; stepedit<16; stepedit++)
	{
		// step edit
		SetControlSize(kStepEdit01 + stepedit, size);
		btnStepEdit[stepedit] = new CMovieButton (size, this, kStepEdit01 + stepedit, (bmpStepEdit->getHeight()/2), bmpStepEdit, offset);
		btnStepEdit[stepedit]->setValue(effect->getParameter(kStepEdit01 + stepedit));
		frame->addView(btnStepEdit[stepedit]);
	}

	// step edit
	//{504, 372, 31, 18},  //kStepEdit16
	
	size(543, 372, 543+31, 372+18);
	btnStepEditMode = new CMovieButton(size, this, kStepEditMode, (bmpStepEdit->getHeight()/2), bmpStepEdit, offset);
	btnStepEditMode->setValue(effect->getParameter(kStepEditMode));
	frame->addView(btnStepEditMode);

	for (int page=0; page<4; page++)
	{
		size(543, 240+(page*18), 543+31, 240+18+(page*18));
		btnPage[page] = new CMovieButton(size, this, kPage00 + page, (bmpStepEdit->getHeight()/2), bmpStepEdit, offset);
		btnPage[page]->setValue(effect->getParameter(kPage00 + page));
		frame->addView(btnPage[page]);
	}

	// 16 step pattern
	for (int acc=0; acc<16; acc++)
	{
		SetControlSize(kAccent01 + acc, size);
		btnAccent[acc] = new COnOffButton (size, this, kAccent01 + acc, bmpAccent);
		btnAccent[acc]->setValue (effect->getParameter (kAccent01 + acc));
		frame->addView(btnAccent[acc]);
	}

	for (int slide=0; slide<16; slide++)
	{
		SetControlSize(kSlide01 + slide, size);
		btnSlide[slide] = new COnOffButton (size, this, kSlide01 + slide, bmpSlide);
		btnSlide[slide]->setValue (effect->getParameter (kSlide01 + slide));
		frame->addView(btnSlide[slide]);
	}

	for (int pitch=0; pitch<16; pitch++)
	{
		CBitmap * bmpBack;
		CBitmap * bmpThumb;
		switch(pitch)
		{
		case 0:
		case 4:
		case 8:
		case 12:
			bmpBack = bmpSliderBack2;
			bmpThumb = bmpSliderThumb;
			break;
		default:
			bmpBack = bmpSliderBack;
			bmpThumb = bmpSliderThumb2;
		}


		CPoint point (0, 0);
		CPoint offset (0, 0);
		CPoint hOffset(2,1);

		SetControlSize(kPitch01 + pitch, size);
		/*
		vsPitch[pitch] = new CVerticalSlider(size, this, kPitch01 + pitch, 0, 76, bmpSliderThumb, bmpSliderBack, offset, kBottom);
		vsPitch[pitch]->setValue (effect->getParameter (kPitch01 + pitch));
		//vsPitch[pitch]->setPos((int)(60.f * vsPitch[pitch]->getValue()));
		//vsPitch[pitch]->setDrawTransparentHandle(true); 
		frame->addView(vsPitch[pitch]);
		*/
		//vsPitch[pitch] = new CPitchSlider (size, this, kPitch01 + pitch, 0, 76, bmpThumb, bmpBack, offset, kBottom);
		
		vsPitch[pitch] = new CPitchSlider (size,this, kPitch01 + pitch,  hOffset, 77, bmpThumb, bmpBack, offset, kBottom|kVertical);
	
		//vsPitch[pitch] = new CVerticalSlider(size, this, kPitch01 + pitch, 0, 76, bmpSliderThumb, bmpSliderBack, offset, kBottom);
		//vsPitch[pitch]->setOffsetHandle (hOffset);
		vsPitch[pitch]->setValue (effect->getParameter (kPitch01 + pitch));
		vsPitch[pitch]->setPos((int)(60.f * vsPitch[pitch]->getValue()));
		vsPitch[pitch]->setDrawTransparentHandle(false); 
		frame->addView(vsPitch[pitch]);


		SetControlSize(kPitchDisplay01 + pitch, size);
		mbPitch[pitch] = new CMovieBitmap(size, this, kPitchDisplay01 + pitch, 61, (bmpPitchDisplay->getHeight()/61), bmpPitchDisplay, offset);
		mbPitch[pitch]->setValue(effect->getParameter(kPitchDisplay01 + pitch));
		frame->addView(mbPitch[pitch]);
	}

	// pd filename aka debug display - only for debugging
#ifdef _DEBUG_STRING_
	size (0, 0, 550, 16);
	pdFileName = new CParamDisplay (size, 0, kCenterText); // bmpDisplayBackground //hBackground
	pdFileName->setFont (kNormalFontSmall);
	pdFileName->setFontColor (kWhiteCColor);
	pdFileName->setBackColor (kBlackCColor);
	pdFileName->setFrameColor (kBlueCColor);
	pdFileName->setValue (0.0f);
	pdFileName->setStringConvert (FileNameStringConvert);
//	pdFileName->setTextTransparency(true); 
	frame->addView (pdFileName);
#endif

#ifdef _LIMITED_
	// teDisplay
	size (220, 192, 320, 212);
	teVersion = new CTextEdit(size, this, kCenterText, 0, 0, 0); // kDoubleClickStyle); 
	teVersion->setFont (kNormalFontSmall);
	teVersion->setFontColor (kBlackCColor);
	teVersion->setBackColor (kWhiteCColor);
//	teVersion->setFrameColor (kBlueCColor);
	teVersion->setValue (0.0f);
	teVersion->setText("Demo 1.01");
	teVersion->setTextTransparency(true); 
	frame->addView (teVersion);
#endif

	// splash screen
	size (170, 8, 398, 39); // area of creakbox logo
	CRect toDisplay(0, 0, bmpSplashScreen->getWidth(), bmpSplashScreen->getHeight());
	ssAbout = new CSplashScreen(size, this, kAbout, bmpSplashScreen, toDisplay, point); 
	frame->addView(ssAbout);

	return true;
}

//-----------------------------------------------------------------------------
void SilverBoxEditor::close()
{
	delete frame;
	frame = 0;

}

//-----------------------------------------------------------------------------
void SilverBoxEditor::setParameter(long index, float value)
{
	if (!frame)
		return;

	switch(index)
	{
		case kVolume:		if (akVolume) akVolume->setValue(value); break;
#ifdef ___WAVEFORM_KNOB__
		case kWaveform:		if (kbWaveform) kbWaveform->setValue(value); break;
#else
		case kWaveform:		if (btnWaveform) btnWaveform->setValue(value); break;
#endif
		case kTuning:		if (akTuning) akTuning->setValue(value); break;
		case kCutOffFreq:	if (akCutOffFreq) akCutOffFreq->setValue(value); break;
		case kResonance:	if (akResonance) akResonance->setValue(value); break;
		case kEnvMod:		if (akEnvMod) akEnvMod->setValue(value); break;
		case kDecay:		if (akDecay) akDecay->setValue(value); break;
		case kAccent:		if (akAccent) akAccent->setValue(value); break;

		case kTempo:	
			{
				if (akTempo) akTempo->setValue(value); 
				if (sdTempo) sdTempo->setValue((value * 260.0)+ 40.0); break;
			}
			break;
#ifndef _LIMITED_
		case kCurrentPatternDisplay:
			if (sdCurrPattern) sdCurrPattern->setValue(value); break;

		case kNextPatternDisplay:
			if (sdNextPattern) sdNextPattern->setValue(value); break;

#ifdef _CYCLE_
		case kStartDisplay:
			if (sdStartCycle) sdStartCycle->setValue(value); break;

		case kFinishDisplay:
			if (sdFinishCycle) sdFinishCycle->setValue(value); break;
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
				int step = index-kAccent01;
				if (btnAccent[step]) btnAccent[step]->setValue(value);
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
				int step = index-kPitch01;
				int iPos = (int)(60.f * value);

				if (vsPitch[step]) vsPitch[step]->setPos(iPos);
				if (vsPitch[step]) vsPitch[step]->setValue(value);
				if (mbPitch[step]) mbPitch[step]->setValue(value);

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
				int step = index - kPitchDisplay01;
				if (mbPitch[step]) mbPitch[step]->setValue(effect->getParameter(index));
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
				int step = index-kSlide01;
				if (btnSlide[step]) btnSlide[step]->setValue(value);
			}
			break;

		case kSync:	if (btnSync) btnSync->setValue(value); 	break;
		case kRunStop:	if (btnRunStop) btnRunStop->setValue(value); 	break;

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
				//if (mbStep[step]) mbStep[step]->setValue(value);
				if (mbStep[step]) mbStep[step]->setValue(effect->getParameter(index));
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
//				if (btnStepEdit[step]) btnStepEdit[step]->setValue(value);
				if (btnStepEdit[step]) btnStepEdit[step]->setValue(effect->getParameter(index));
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
				if (btnBank[bank]) btnBank[bank]->setValue(effect->getParameter(index));
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
				if (btnPreset[preset]) btnPreset[preset]->setValue(effect->getParameter(index));
			}
			break;
#endif
		case kNoteOff:
			{
				if (btnNoteOff) btnNoteOff->setValue(value);
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
				if (btnNote[note]) btnNote[note]->setValue(value);
			}
			break;
		case kOctave01:
		case kOctave02:
		case kOctave03:
		case kOctave04:
		case kOctave05:
			{
				int octave = index - kOctave01;
				if (btnOctave[octave]) btnOctave[octave]->setValue(value);
			}
			break;

		case kPage00:
		case kPage01:
		case kPage02:
		case kPage03:
			{
				int page = index-kPage00;
				if (btnPage[page]) btnPage[page]->setValue(value);
			}
			break;

		case kStepEditMode:
			{
				if (btnStepEditMode) btnStepEditMode->setValue(value);
			}
			break;


	}


	char display[256];
	if (teDisplay)
	{
		effect->getParameterDisplay(index, display);
		teDisplay->setText(display);
		teDisplay->setDirty();
	}

//	postUpdate (); // do no post update.. have the synth do it
}


//-----------------------------------------------------------------------------
// this function is called when the user changes a parameter in the editor;
void SilverBoxEditor::valueChanged(CDrawContext* context, CControl* control)
{
//	int counter;

	long tag = control->getTag ();
	switch(tag)
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
			int step = tag-kPitch01;
			float value = control->getValue();
			//effect->setParameterAutomated (tag, value);
			// update pitch display
			float fPos = vsPitch[step]->getPos() / 60.f;

			effect->setParameter(kPitchDisplay01+step, fPos);
			((SilverBox*)effect)->setParameterInt(tag, vsPitch[step]->getPos());
			// update param display
			if (mbPitch[step]) mbPitch[step]->setValue(fPos);
			control->update(context);
		}
		break;

	
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
			effect->setParameter(tag, control->getValue());
			control->update (context);
		}
		break;

	case kTempo:
		{
			float value = control->getValue();
			effect->setParameter(tag, value);
			effect->setParameter(kTempoDisplay, (value * 260.0)+ 40.0);
			control->update (context);
		}
		break;

	case kVolume:
	case kWaveform:
	case kTuning:
	case kCutOffFreq:
	case kResonance:
	case kEnvMod:
	case kDecay:
	case kAccent:
		effect->setParameterAutomated (tag, control->getValue ());
		control->update (context);
		break;
#ifndef _LIMITED_
	case kClear:
	case kCopy:
	case kPaste:
		if (control->getValue() == 1.0f)
		{
			effect->setParameter(tag, control->getValue ());
			control->update (context);
		}
		break;

#ifdef _RANDOM_PATTERNS_
	case kRandom1:
	case kRandom2:
		if (control->getValue() == 1.0f)
		{
			effect->setParameter(tag, control->getValue ());
			control->update (context);
		}
		break;
#endif
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
			float value = control->getValue();
			if (value == 1.0)
			{
		//		if (!(((SilverBox*) (effect))->GetNextPatternLock()))
		//		{
				effect->setParameter(tag, value);
				control->update (context);
		//		}
			}
			else // do not press up a button
				control->setValue(1.0f);

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
			float value = control->getValue();
			if (value == 1.0)
			{
		//		if (!(((SilverBox*) (effect))->GetNextPatternLock()))
		//		{
				effect->setParameter(tag, value);
				control->update (context);
		//		}
			}
			else // do not press up a button
				control->setValue(1.0f);
		}
		break;
#endif
	case kNoteOff:
		{
			float value = control->getValue();
			if (value == 1.0)
			{
				effect->setParameter(tag, value);
				control->update (context);
			}
			else // do not press up a button
				control->setValue(1.0f);
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
			float value = control->getValue();
			if (value == 1.0)
			{
				effect->setParameter(tag, value);
				control->update (context);
			}
			else // do not press up a button
				control->setValue(1.0f);
		}
		break;
	case kOctave01:
	case kOctave02:
	case kOctave03:
	case kOctave04:
	case kOctave05:
		{
			float value = control->getValue();
			if (value == 1.0)
			{
				effect->setParameter(tag, value);
				control->update (context);
			}
			else // do not press up a button
				control->setValue(1.0f);
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
			float value = control->getValue();
			if (value == 1.0)
			{
				effect->setParameter(tag, value);
				control->update (context);
			}
			else // do not press up a button
				control->setValue(1.0f);
		}
		break;
#ifdef _MIDI_LEARN_
	case kLoadMidiMap:if (control->getValue()) LoadMidiMap(); break;
	case kSaveMidiMap:if (control->getValue()) SaveMidiMap(); break;
#endif
#ifndef _LIMITED_
#ifdef _CYCLE_
	case kSetCycleStart:
	case kSetCycleFinish:
		{
			float value = control->getValue();
			if (value == 1.0)
			{
				effect->setParameter(tag, value);
				control->update (context);
			}
		}
		break;
#endif
#ifdef _BANKS_
	case kLoadFXP:	if (control->getValue()) LoadChunk(true); break;
	case kSaveFXP:	if (control->getValue()) SaveChunk(true); break;
	case kLoadFXB:	if (control->getValue()) LoadChunk(false); break;
	case kSaveFXB:	if (control->getValue()) SaveChunk(false); break;
#endif
#endif
	default:
		effect->setParameter(tag, control->getValue());
		control->update(context);
	}

}

#ifdef _DEBUG_STRING_
void SilverBoxEditor::SetDebugString(char * string)
{
	if (!frame)
		return;

	strcpy(gszFileName, string);

	if (pdFileName)
	{
		float temp = pdFileName->getValue();
		if (temp == 1.0f)
			pdFileName->setValue(0.0f);
		else
			pdFileName->setValue(1.0f);
	}
}
#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//if (editor) ((AEffGUIEditor*)editor)->setParameter(9999, (float) curProgram / 639);
//if (editor)	editor->postUpdate ();

void SilverBoxEditor::LoadMidiMap()
{
}

void SilverBoxEditor::SaveMidiMap()
{
}


void SilverBoxEditor::SaveChunk(bool isPreset)
{
		((SilverBox*) (effect))->writelog("SaveChunk(%d)\n",isPreset);

		CFileSelector fsFileSelector((AudioEffectX *)effect);
#if WIN32
			//strcpy(gszFileName, "blah0");
 			struct VstFileSelect filestruct;
			struct VstFileType bankType ("Bank", ".FXB", "fxb", "fxb", "", "");
			struct VstFileType presetType ("Preset", "FXP", "fxp", "fxp", "", "");
 			memset (&filestruct, 0, sizeof (VstFileSelect));
 			filestruct.command = kVstFileSave ;
 			filestruct.macCreator = 0;
 			filestruct.type = kVstFileType;
 			filestruct.initialPath = "C:\\";
 			//strcpy(filestruct.initialPath, "c:\\");
 			strcpy(filestruct.title,"save bank/preset file");
 			filestruct.nbFileTypes = 1;
			if (isPreset)
				filestruct.fileTypes = &presetType;
			else
				filestruct.fileTypes = &bankType;

 			filestruct.returnPath = new char[1024];
//			filestruct.sizeReturnPath = 1024;
			memset(filestruct.returnPath, 0, 1024);
			long blah = fsFileSelector.run(&filestruct);

			((SilverBox*) (effect))->writelog("nbReturnPath: %d  sizeReturnPath: %d\n",filestruct.nbReturnPath,filestruct.sizeReturnPath);

			//strcpy(gszFileName, filestruct.returnPath);
			//strcpy(gszFileName, "blah1");
			
			if (filestruct.nbReturnPath > 0)
			{
				((SilverBox*) (effect))->writelog("filepath: %s sizeReturnPath: %d\n",filestruct.returnPath, filestruct.sizeReturnPath);

				char szFileName[256];
				memset(szFileName, 0, 256);
				int iLen = strlen(filestruct.returnPath);
				int x = iLen;
				while (x>0)
				{
					if (filestruct.returnPath[x-1] == '\\')
					{
						break;
					}
					x--;
				}

				((SilverBox*) (effect))->writelog("found \\ at position %d\n",(x-1));
				strncpy(szFileName, &filestruct.returnPath[x], iLen-x);
 
				((SilverBox*) (effect))->writelog("szFileName: %s\n",szFileName);
				
				strcpy(gszFileName, szFileName);
				((SilverBox*) (effect))->SaveChunk(filestruct.returnPath, isPreset);
			}
			delete [] filestruct.returnPath;
#endif
}

void SilverBoxEditor::LoadChunk(bool isPreset)
{
	((SilverBox*) (effect))->writelog("LoadChunk(%d)\n",isPreset);

		CFileSelector fsFileSelector((AudioEffectX *)effect);
#if WIN32
			//strcpy(gszFileName, "blah0");
 			struct VstFileSelect filestruct;
			struct VstFileType bankType ("Bank", ".FXB", "fxb", "fxb", "", "");
			struct VstFileType presetType ("Preset", "FXP", "fxp", "fxp", "", "");
 			memset (&filestruct, 0, sizeof (VstFileSelect));
 			filestruct.command = kVstFileLoad ;
 			filestruct.macCreator = 0;
 			filestruct.type = kVstFileType;
 			filestruct.initialPath = "C:\\";
 			//strcpy(filestruct.initialPath, "c:\\");
 			strcpy(filestruct.title,"load bank/preset file");
 			filestruct.nbFileTypes = 1;
			if (isPreset)
				filestruct.fileTypes = &presetType;
			else
				filestruct.fileTypes = &bankType;

 			filestruct.returnPath = new char[1024];
			//memset(filestruct.returnPath, 0, 1024);
			long blah = fsFileSelector.run(&filestruct);

			((SilverBox*) (effect))->writelog("nbReturnPath: %d  sizeReturnPath: %d\n",filestruct.nbReturnPath,filestruct.sizeReturnPath);

			//strcpy(gszFileName, filestruct.returnPath);
			//strcpy(gszFileName, "blah1");
			
			if (filestruct.nbReturnPath > 0)
			{
				((SilverBox*) (effect))->writelog("filepath: %s sizeReturnPath: %d\n",filestruct.returnPath, filestruct.sizeReturnPath);

				char szFileName[256];
				memset(szFileName, 0, 256);
				int iLen = strlen(filestruct.returnPath);
				int x = iLen;
				while (x>0)
				{
					if (filestruct.returnPath[x-1] == '\\')
					{
						break;
					}
					x--;
				}

				((SilverBox*) (effect))->writelog("found \\ at position %d\n",(x-1));
				strncpy(szFileName, &filestruct.returnPath[x], iLen-x);
 
				((SilverBox*) (effect))->writelog("szFileName: %s\n",szFileName);
				
				strcpy(gszFileName, szFileName);
				((SilverBox*) (effect))->LoadChunk(filestruct.returnPath, isPreset);
			}
			delete [] filestruct.returnPath;
#endif
}
