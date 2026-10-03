
#ifndef __SCRATCHA_EDITOR__
#define __SCRATCHA_EDITOR__

// define below to have knob, else ull have a button
//#define ___WAVEFORM_KNOB__

// include VSTGUI
#ifndef __vstgui__
#include "vstgui.h"
#endif

#ifndef __SILVERBOX__
#include "SilverBox.h"
#endif


#include "CPitchSlider.h"

//-----------------------------------------------------------------------------
class SilverBoxEditor : public AEffGUIEditor, public CControlListener
{
public:
	SilverBoxEditor (AudioEffect *effect);
	virtual ~SilverBoxEditor ();
	virtual void setParameter (long index, float value);

#ifdef _DEBUG_STRING_
	void SetDebugString(char * string);
#endif

protected:
	virtual long open (void *ptr);
	virtual void close ();
	virtual void valueChanged (CDrawContext* context, CControl* control);

private:
	char _szFileName[1024];
	char _szLastPath[1024];
	

	void LoadChunk(bool isPreset);
	void SaveChunk(bool isPreset);

	void SaveMidiMap();
	void LoadMidiMap();

	// CB303 controls
	CAnimKnob *akVolume;

	// sound editing
	CAnimKnob *akTuning;
	CAnimKnob *akCutOffFreq;
	CAnimKnob *akResonance;
	CAnimKnob *akEnvMod;
	CAnimKnob *akDecay;
	CAnimKnob *akAccent;

	CAnimKnob *akTempo;

#ifdef ___WAVEFORM_KNOB__
	CKnob *kbWaveform;
#else
	COnOffButton * btnWaveform;
#endif

	// pattern management
	CKickButton * kbClear;
	CKickButton * kbCopy;
	CKickButton * kbPaste;
	CKickButton * kbRandom1;
	CKickButton * kbRandom2;
#ifdef _BANKS_
	CKickButton * kbLoadFXP;
	CKickButton * kbSaveFXP;
	CKickButton * kbLoadFXB;
	CKickButton * kbSaveFXB;
#endif
	CKickButton * kbLoadMidiMap;
	CKickButton * kbSaveMidiMap;

	CKickButton * kbSetCycleStart;
	CKickButton * kbSetCycleFinish;
	COnOffButton * btnCycle;

	CParamDisplay * pdStartPattern;
	CParamDisplay * pdFinishPattern;

	// pattern/tempo
	CSpecialDigit * sdTempo;

	CSpecialDigit * sdCurrPattern;
	CSpecialDigit * sdNextPattern;

	CSpecialDigit * sdStartCycle;
	CSpecialDigit * sdFinishCycle;

	// play/sync
	COnOffButton * btnRunStop;
	COnOffButton * btnSync;	// to sync to host

	CMovieBitmap * mbStep[16];
	CMovieButton * btnStepEdit[16];

	CMovieButton * btnBank[10];
	CMovieButton * btnPreset[10];

	CMovieButton  * btnNoteOff;
	CMovieButton  * btnNote[12];
	CMovieButton  * btnOctave[5];

	// pattern editing
	COnOffButton *	btnAccent[16];
	COnOffButton *	btnSlide[16];
	CPitchSlider * vsPitch[16];
	//CVerticalSlider * vsPitch[16];
	CMovieButton * btnStepEditMode;
	CMovieButton * btnPage[4];

	CMovieBitmap * mbPitch[16];

//	COnOffButton * btnMidiLearn;

	// Param Displays
#ifdef _DEBUG_STRING_
	CParamDisplay *pdFileName;
#endif

	CTextEdit *teDisplay;

	// Bitmaps
	CBitmap *hBackground;
	CBitmap *bmpOnOffButton;
	CBitmap *bmpSplashScreen;

	CBitmap *bmpSliderBack;
	CBitmap *bmpSliderThumb;
	//
	CBitmap *bmpGroupBtn[10];
	CBitmap *bmpPresetBtn[10];
	CBitmap *bmpOffButton;
	CBitmap *bmpKeyWhite;
	CBitmap *bmpKeyBlack;
	CBitmap *bmpOctave[5];
	CBitmap *bmpAccent;
	CBitmap *bmpSlide;

	CBitmap *bmpRun;
	CBitmap *bmpSync;

	CBitmap *bmpLoop;
	CBitmap *bmpLoopStart;
	CBitmap *bmpLoopEnd;

	CBitmap *bmpCopy;
	CBitmap *bmpPaste;
	CBitmap *bmpClear;
#ifdef _RANDOM_PATTERNS_
	CBitmap *bmpRnd1;
	CBitmap *bmpRnd2;
#endif

	CBitmap *bmpVolume;
	CBitmap *bmpTempo;

	CBitmap *bmpTune;
	CBitmap *bmpCut;
	CBitmap *bmpReso;
	CBitmap *bmpEnv;
	CBitmap *bmpDecay;
	CBitmap *bmpAcc;

	CBitmap *bmpLED01;
	CBitmap *bmpLED02;
	CBitmap *bmpSliderBack2;
	CBitmap *bmpStepEdit;
	CBitmap *bmpSliderThumb2;

	CBitmap *bmpPitchDisplay;

	CBitmap *bmpTempoDigits;
	CBitmap *bmpNumberDigits;

	CVerticalSwitch * vsKey;

	CTextEdit * teVersion;

	// SplashScreen
	CSplashScreen *ssAbout;

	void SetControlSize(int control_id, CRect & size);
	void SetDisplaySize(int control_id, CRect & size);
};

#endif
