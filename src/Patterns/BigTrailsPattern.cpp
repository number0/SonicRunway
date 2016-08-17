//
//  BigTrailsPattern.cpp
//  SonicRunway
//
//  Created by Paul Roales on 7/17/16.
//
//

#include "BigTrailsPattern.hpp"
#include "Audio.hpp"
#include "GlobalParameters.hpp"

SrBigTrailsPattern::SrBigTrailsPattern(const std::string & name,
                                 SrModel * model, SrAudio * audio,
                                       SrGlobalParameters * globalParameters) :
    SrScrollingPattern(name, model, audio, globalParameters),
    _jitterParam(20.0),
    _rotationParam(12.0),
    _average(0.0)
{
    _jitterParam.setName("Jitter");
    _jitterParam.setMin(0.0);
    _jitterParam.setMax(100.0);
    _AddUIParameter(_jitterParam);
    
    _rotationParam.setName("Rotation");
    _rotationParam.setMin(-50.0);
    _rotationParam.setMax(50.0);
    _AddUIParameter(_rotationParam);
}

SrBigTrailsPattern::~SrBigTrailsPattern()
{
    
}

void
SrBigTrailsPattern::_Update()
{
    SrScrollingPattern::_Update();
    
    float jitterParam = (float) _jitterParam;
    
    float low = GetAudio()->GetLows()[0];
    float mid = GetAudio()->GetMids()[0];
    float high = GetAudio()->GetHighs()[0];
    
    low *= jitterParam;
    mid *= jitterParam;
    high *= jitterParam;
    
    // Rolling average replacing 5% each time.
    _average = (_average * .95) + (.05 * (low + mid + high));
}

void
SrBigTrailsPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    int bufferSize = buffer->size();
    float jitterParam = (float) _jitterParam;
    float rotationParam = (float) _rotationParam;
    
    float low = GetAudio()->GetLows()[0];
    float mid = GetAudio()->GetMids()[0];
    float high = GetAudio()->GetHighs()[0];
    
    // Square the values to accentuate the peaks
    low *= jitterParam;
    mid *= jitterParam;
    high *= jitterParam;
    
    int manualAdjustment = rotationParam;
    int index1 = bufferSize * 0.19 + low - _average + manualAdjustment;
    int index2 = bufferSize * 0.5 + mid - _average + manualAdjustment;
    int index3 = bufferSize * 0.81 + high - _average + manualAdjustment;
    
    float hue;
    if (GetGlobalParameters()->GetCycleAutomatically()) {
        hue = GetGlobalParameters()->GetVerySlowCycle();
    } else {
        GetGlobalParameters()->GetSlider1();
    }
        
    ofFloatColor color;
    color.setHsb(hue, 0.9, 0.7);
    
    int width = bufferSize * .05;
    for (int i = 0; i < width; ++i) {
        int index1Minus = std::min(index1 + i, bufferSize - 1);
        int index2Minus = std::min(index2 + i, bufferSize - 1);
        int index3Minus = std::min(index3 + i, bufferSize - 1);
        
        int index1Plus = std::max(index1 - i, 1);
        int index2Plus = std::max(index2 - i, 1);
        int index3Plus = std::max(index3 - i, 1);
        
        (*buffer)[index1Minus] = color;
        (*buffer)[index2Minus] = color;
        (*buffer)[index3Minus] = color;
        (*buffer)[index1Plus] = color;
        (*buffer)[index2Plus] = color;
        (*buffer)[index3Plus] = color;
    }
    
}