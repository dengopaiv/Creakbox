//
// core
//

#if MACX
#include <math.h>
#endif

#ifndef _CORE_H_
#define _CORE_H_

#if MACX
extern const double_t pi;
#else
const long double pi = 3.141592653l;
#endif
const long double twopi = pi*2.0l;
const long double halfpi = pi*0.5l;

#endif