//
//  Util.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 4/6/16.
//
//

#ifndef SR_UTIL_HPP
#define SR_UTIL_HPP

#include <stdio.h>
#include <string>
#include "ofxGui.h"

// Set the current working directory to the location of resources
// so we can load text files, images, etc.
void
SrUtil_ChangeToResourceDirectory();

// Get a resolved file path for the given resource
std::string
SrUtil_GetAbsolutePathForResource(const std::string & localPath);

// Split the string with the given delimeter character
std::vector<std::string>
SrUtil_SplitString(const std::string & s, char delim);

// Find the parameter at the given path, or return NULL.
ofAbstractParameter *
SrUtil_FindParameter(ofParameterGroup & parameterGroup,
                     const std::string & path);

// Return true if the given path represents a pattern that is
// enabled.
bool
SrUtil_IsPathToEnabledPattern(const std::string & path,
                              ofParameterGroup & parameterGroup);

float
SrUtil_ClampCycle(float min, float max, float value);

#endif /* Util_hpp */
