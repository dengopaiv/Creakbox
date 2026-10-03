//
// freq.h
//

#ifndef _FREQ_H_
#define _FREQ_H_

#include "core.h"

struct tNote
{
	char note; //1..72=note; 0=note off!
    bool slide;
	bool accent;
};

//int freq_sign(long double nr);

//long double tonegen_sawWave(long double position);
//long double tonegen_sqrWave(long double position);

#endif _FREQ_H