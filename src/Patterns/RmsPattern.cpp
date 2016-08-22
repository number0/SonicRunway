//
//  RmsPattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/14/16.
//
//

#include "RmsPattern.hpp"
#include "Audio.hpp"
#include "GlobalParameters.hpp"
#include "Util.hpp"

SrRmsPattern::SrRmsPattern(const std::string & name,
                           SrModel * model, SrAudio * audio,
                           SrGlobalParameters * globalParameters) :
    SrScrollingPattern(name, model, audio, globalParameters),
    _hue1Param(0.2),
    _hue2Param(0.5),
    _hue3Param(0.8),
    _saturationParam(0.8),
    _brightnessParam(0.8)
{
    _hue1Param.setName("Hue 1");
    _hue1Param.setMin(0.0);
    _hue1Param.setMax(1.0);
    _AddUIParameter(_hue1Param);
    
    _hue2Param.setName("Hue 2");
    _hue2Param.setMin(0.0);
    _hue2Param.setMax(1.0);
    _AddUIParameter(_hue2Param);
    
    _hue3Param.setName("Hue 3");
    _hue3Param.setMin(0.0);
    _hue3Param.setMax(1.0);
    _AddUIParameter(_hue3Param);
    
    _saturationParam.setName("Saturation");
    _saturationParam.setMin(0.0);
    _saturationParam.setMax(1.0);
    _AddUIParameter(_saturationParam);
    
    _brightnessParam.setName("Brightness");
    _brightnessParam.setMin(0.0);
    _brightnessParam.setMax(1.0);
    _AddUIParameter(_brightnessParam);
}

SrRmsPattern::~SrRmsPattern()
{
    // Uses GlobalParams like so:
    // Dial1 - Hue 1
    // Dial2 - Hue 2
    // Dial3 - Hue 3
    // Slider1 - Saturation
    // Slider2 - Brightness
}

void
SrRmsPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    float low = GetAudio()->GetLows()[0];
    float mid = GetAudio()->GetMids()[0];
    float high = GetAudio()->GetHighs()[0];
    
    ofFloatColor lowColor;
    ofFloatColor midColor;
    ofFloatColor highColor;
    
    SrGlobalParameters * globals = GetGlobalParameters();
    
    float lowHue = _hue1Param + globals->GetPhraseCycle();
    float midHue = _hue2Param + globals->GetPhraseCycle();
    float highHue = _hue3Param + globals->GetPhraseCycle();
    float saturation = _saturationParam;
    float brightness = _brightnessParam;
    
    lowHue = SrUtil_ClampCycle(0, 1, lowHue);
    midHue = SrUtil_ClampCycle(0, 1, midHue);
    highHue = SrUtil_ClampCycle(0, 1, highHue);
    
    if (GetGlobalParameters()->WasRecentManualInput()) {
        lowHue = globals->GetDial1();
        midHue = globals->GetDial2();
        highHue = globals->GetDial3();
        
        saturation = globals->GetSlider1();
        brightness = globals->GetSlider2();
    }
    
    lowColor.setHsb(lowHue, saturation, brightness);
    midColor.setHsb(midHue, saturation, brightness);
    highColor.setHsb(highHue, saturation, brightness);
    
    // Square the values to accentuate the peaks
    low *= low;
    mid *= mid;
    high *= high;
    
    int numSegments = 3;
    size_t segment0Start = 0;
    size_t segment1Start = buffer->size() / numSegments;
    size_t segment2Start = buffer->size() / numSegments * 2;
    
    size_t numLowLeds = low * buffer->size() / numSegments;
    size_t numMidLeds = mid * buffer->size() / numSegments;
    size_t numHighLeds = high * buffer->size() / numSegments;
    
    size_t fullCircleClamp = 4.0 / 3.0 * buffer->size();
    
    for(size_t i=0; i < numLowLeds; i++) {
        size_t idx = i + segment0Start;
        idx %= buffer->size();
        (*buffer)[idx] = lowColor;
    }
    
    for(size_t i=0; i < numMidLeds; i++) {
        int idx = i + segment1Start;
        idx %= buffer->size();
        (*buffer)[idx] = midColor;
    }
    
    for(size_t i=0; i < numHighLeds; i++) {
        int idx = i + segment2Start;
        idx %= buffer->size();
        (*buffer)[idx] = highColor;
    }
    
}