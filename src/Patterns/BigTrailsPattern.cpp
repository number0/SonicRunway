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
#include "Util.hpp"

SrBigTrailsPattern::SrBigTrailsPattern(const std::string & name,
                                 SrModel * model, SrAudio * audio,
                                       SrGlobalParameters * globalParameters) :
    SrScrollingPattern(name, model, audio, globalParameters),
    _average(0.0),
    _hueParam(0.2),
    _saturationParam(0.8),
    _brightnessParam(0.8),
    _jitterParam(30.0),
    _rotationParam(12.0)
{
    _hueParam.setName("Hue");
    _hueParam.setMin(0.0);
    _hueParam.setMax(1.0);
    _AddUIParameter(_hueParam);
    
    _saturationParam.setName("Saturation");
    _saturationParam.setMin(0.0);
    _saturationParam.setMax(1.0);
    _AddUIParameter(_saturationParam);
    
    _brightnessParam.setName("Brightness");
    _brightnessParam.setMin(0.0);
    _brightnessParam.setMax(1.0);
    _AddUIParameter(_brightnessParam);
    
    _jitterParam.setName("Jitter");
    _jitterParam.setMin(0.0);
    _jitterParam.setMax(1.0);
    _AddUIParameter(_jitterParam);
    
    _rotationParam.setName("Rotation");
    _rotationParam.setMin(0.0);
    _rotationParam.setMax(1.0);
    _AddUIParameter(_rotationParam);
}

SrBigTrailsPattern::~SrBigTrailsPattern()
{
    // Uses GlobalParams like so:
    // Dial1 - Hue 1
    // Dial2 - Jitter (Dial2 * 100)
    // Dial3 - Rotation (Dial3 * 100 - 50)
    // Slider1 - Saturation
    // Slider2 - Brightness
}

void
SrBigTrailsPattern::_Update()
{
    SrScrollingPattern::_Update();
    
    float low = GetAudio()->GetLows()[0];
    float mid = GetAudio()->GetMids()[0];
    float high = GetAudio()->GetHighs()[0];
    
    float jitter;
    
    SrGlobalParameters * globals = GetGlobalParameters();
    
    if (globals->WasRecentManualInput()) {
        jitter = globals->GetDial2() * 100;
    } else {
        jitter = _jitterParam;
    }
    
    low *= jitter;
    mid *= jitter;
    high *= jitter;
    
    // Rolling average replacing 5% each time.
    _average = (_average * .95) + (.05 * (low + mid + high));
}

void
SrBigTrailsPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    int bufferSize = buffer->size();

    float hue;
    float saturation;
    float brigthness;
    float rotation;
    float jitter;
    
    SrGlobalParameters * globals = GetGlobalParameters();
    
    if (GetGlobalParameters()->WasRecentManualInput()) {
        hue = globals->GetDial1();
        jitter = globals->GetDial2() * 100;
        rotation = globals->GetDial3() * 100 - 50;
        saturation = globals->GetSlider1();
        brigthness = globals->GetSlider2();
    } else {
        hue = _hueParam + globals->GetVerySlowCycle();
        saturation = _saturationParam;
        brigthness = _brightnessParam;
        rotation = _rotationParam;
        jitter = _jitterParam;
    }
    
    hue = SrUtil_ClampCycle(0, 1, hue);
    
    float low = GetAudio()->GetLows()[0];
    float mid = GetAudio()->GetMids()[0];
    float high = GetAudio()->GetHighs()[0];
    
    // Square the values to accentuate the peaks
    low *= jitter;
    mid *= jitter;
    high *= jitter;
    
    int manualAdjustment = rotation;
    int index1 = bufferSize * 0.19 + low - _average + manualAdjustment;
    int index2 = bufferSize * 0.5 + mid - _average + manualAdjustment;
    int index3 = bufferSize * 0.81 + high - _average + manualAdjustment;
    
    ofFloatColor color;
    color.setHsb(hue, saturation, brigthness);
    
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