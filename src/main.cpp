
#include <stddef.h>
#include "SilverBoxGUI.h"
#include <time.h>

static AudioEffect *effect = 0;
bool oome = false;

//#define _DATETIME_PROTECTION_
#ifdef _DATETIME_PROTECTION_

#define PROT_MONTH 03
#define PROT_YEAR 2003

#endif

#if MAC
#pragma export on
#endif

//------------------------------------------------------------------------
// prototype of the export function main
#if BEOS
#define main main_plugin
extern "C" __declspec(dllexport) AEffect *main_plugin (audioMasterCallback audioMaster);

#elif MACX
#define main main_macho
extern "C" AEffect *main_macho (audioMasterCallback audioMaster);

#else
AEffect *main (audioMasterCallback audioMaster);
#endif

AEffect *main (audioMasterCallback audioMaster)
{
#ifdef _DATETIME_PROTECTION_
	time_t currtime;
	time(&currtime);
	tm * tm_struct = localtime(&currtime);
	
	//printf("tm_mon: %d tm_year: %d\n", tm_struct->tm_mday, tm_struct->tm_year);
	if( !(((tm_struct->tm_mon+1) == PROT_MONTH) && ((tm_struct->tm_year + 1900) == PROT_YEAR)) )
	{
		return 0;
	}

#endif

// get vst version
	if(!audioMaster (0, audioMasterVersion, 0, 0, 0, 0))
		return 0;  // old version

	effect = new SilverBoxGUI (audioMaster);
	if (!effect)
		return 0;
	if (oome)
	{
		delete effect;
		return 0;
	}
	return effect->getAeffect ();
}

#if MAC
#pragma export off
#endif

#if WIN32
#include <windows.h>
void* hInstance;
BOOL WINAPI DllMain (HINSTANCE hInst, DWORD dwReason, LPVOID lpvReserved)
{
#ifdef _DATETIME_PROTECTION_
	time_t currtime;
	time(&currtime);
	tm * tm_struct = localtime(&currtime);
	
	//printf("tm_mon: %d tm_year: %d\n", tm_struct->tm_mday, tm_struct->tm_year);
	if( !(((tm_struct->tm_mon+1) == PROT_MONTH) && ((tm_struct->tm_year + 1900) == PROT_YEAR)) )
	{
		return 0;
	}
#endif

	hInstance = hInst;
	return 1;
}
#endif
