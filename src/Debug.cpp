//
//  Debug.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/16/16.
//
//

#include "Debug.hpp"
#include "stdarg.h"

// Define this const to enable debugging output.
// XXX we could be fancier and do this all with a macro so
// you could enable/disable per file..
// #define SR_DEBUG

void
SrError(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf(fmt, args);
}

void
SrWarn(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf(fmt, args);
}

void
SrDebug(const char *fmt, ...)
{
#ifdef SR_DEBUG
    va_list args;
    va_start(args, fmt);
    printf(fmt, args);
#endif
}
