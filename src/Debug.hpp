//
//  Debug.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/16/16.
//
//

#ifndef SR_DEBUG_HPP
#define SR_DEBUG_HPP

#include <stdio.h>

// Print as an error.
void
SrError(const char * fmt, ...);

// Print as a warning
void
SrWarn(const char * fmt, ...);

// Print if debugging is on (see const in Debug.cpp)
void
SrDebug(const char * fmt, ...);

#endif
