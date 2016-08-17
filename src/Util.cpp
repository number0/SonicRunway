//
//  Util.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 4/6/16.
//
//

#include "Util.hpp"
#include "Debug.hpp"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

using namespace std;

static std::string
_GetResourcePath()
{
    // from http://stackoverflow.com/questions/516200/relative-paths-not-working-in-xcode-c
    // This makes relative paths work in C++ in Xcode by changing directory to the Resources folder inside the .app bundle
#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);

    return std::string(path);
#endif
    
    SrError("_GetResourcePath only understood for __APPLE__\n");
    return std::string("");
}

void
SrUtil_ChangeToResourceDirectory()
{
    std::string path = _GetResourcePath();
    chdir(path.c_str());
}

std::string
SrUtil_GetAbsolutePathForResource(const std::string & localPath)
{
    std::string resourcePath = _GetResourcePath();
    return resourcePath + '/' + localPath;
}

/*
std::string
SrUtil_GetFullPath(const ofAbstractParameter & param)
{
    std::vector<std::string> pathVec = param.getGroupHierarchyNames();
    std::string ret;
    for(auto iter = pathVec.begin(); iter != pathVec.end(); iter++) {
        ret += "/" + (*iter);
    }
   
    return ret;
}
 */

// From stack overflow:
// http://stackoverflow.com/questions/236129/split-a-string-in-c
static vector<string> &
_SplitString(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string>
SrUtil_SplitString(const string &s, char delim) {
    vector<string> elems;
    _SplitString(s, delim, elems);
    return elems;
}

static ofAbstractParameter *
_FindParameterInGroup(ofParameterGroup & parameterGroup,
                      const std::string & name)
{
    for(auto iter = parameterGroup.begin();
        iter != parameterGroup.end();
        iter++) {
        SrDebug("      checking %s\n", (*iter)->getName().c_str());
        if ((*iter)->getName() == name) {
            return iter->get();
        }
    }
    
    return NULL;
}

static ofAbstractParameter *
_FindParameterRecurse(ofParameterGroup & parameterGroup,
                      const vector<string> & pathVec,
                      size_t pathIdx)
{
    if (pathIdx > pathVec.size()) {
        SrError("Index error looking for parameter\n");
        return NULL;
    }
    
    ofAbstractParameter * param =
        _FindParameterInGroup(parameterGroup, pathVec[pathIdx]);
    
    if (not param) {
        return NULL;
    }
    
    if (pathIdx == pathVec.size() - 1) {
        return param;
    }
    
    ofParameterGroup * groupPtr =
        dynamic_cast<ofParameterGroup *>(param);
    if (not groupPtr) {
        SrError("%s is not a parameter group\n", param->getName().c_str());
        return NULL;
    }
    
    return _FindParameterRecurse(*groupPtr, pathVec, pathIdx + 1);
}

ofAbstractParameter *
SrUtil_FindParameter(ofParameterGroup & parameterGroup,
                     const std::string & path)
{
    vector<string> pathVec = SrUtil_SplitString(path, '/');
    
    // Start at path index 2.  index 0 is the empty place to the left
    // of the first /.  Index 1 is the first token, which should match
    // the parameter group.  XXX would be nice to verify this...
    return _FindParameterRecurse(parameterGroup, pathVec, 2);
}

bool
SrUtil_IsPathToEnabledPattern(const std::string & path,
                              ofParameterGroup & parameterGroup)
{
    std::string enabledPath = path + "/Enabled";
    ofAbstractParameter * param =
        SrUtil_FindParameter(parameterGroup, enabledPath);
    if (not param) {
        return false;
    }
    
    ofParameter<bool> * boolParam = dynamic_cast<ofParameter<bool> * >(param);
    if (not boolParam) {
        SrError("Enabled param is not a bool\n");
    }
   
    return (bool) (*boolParam);
}

float
SrUtil_ClampCycle(float min, float max, float value)
{
    float length = max - min;
    if (value > max) {
        float remainder = value - max;
        return min + fmod(remainder, length);
    }
    
    if (value < min) {
        float remainder = min - value;
        return max - fmod(remainder, length);
    }
    
    return value;
}

ofFloatColor
SrUtil_LerpColors(const ofFloatColor & c0, const ofFloatColor & c1, float t)
{
    // Color space nerds:  feel free to improve this :)
    return c0 * (1.0 - t) + c1 * t;
}

float
SrUtil_Lerp(float a, float b, float t)
{
    return a * (1.0 - t) + b * t;
}

