//
//  StarPattern.cpp
//  SonicRunway
//
//  Created by Paul Roales on 7/14/16.
//
//

#include "StarPattern.hpp"
#include "Audio.hpp"
#include "GlobalParameters.hpp"

SrStarPattern::SrStarPattern(const std::string & name,
                             SrModel * model, SrAudio * audio,
                             SrGlobalParameters * globalParameters) :
SrScrollingPattern(name, model, audio, globalParameters),
_hueParam(1.0),
_numberParam(30.0),
_thresholdParam(0.4),
_saturationParam(1.0),
_brightnessParam(1.0)
{
    _hueParam.setName("Hue");
    _hueParam.setMin(0.0);
    _hueParam.setMax(1.0);
    _AddUIParameter(_hueParam);
    
    _numberParam.setName("Number");
    _numberParam.setMin(0.0);
    _numberParam.setMax(1.0);
    _AddUIParameter(_numberParam);
    
    _thresholdParam.setName("Threshold");
    _thresholdParam.setMin(0.0);
    _thresholdParam.setMax(1.0);
    _AddUIParameter(_thresholdParam);
    
    _saturationParam.setName("Saturation");
    _saturationParam.setMin(0.0);
    _saturationParam.setMax(1.0);
    _AddUIParameter(_saturationParam);
    
    _brightnessParam.setName("Brightness");
    _brightnessParam.setMin(0.0);
    _brightnessParam.setMax(1.0);
    _AddUIParameter(_brightnessParam);
}

SrStarPattern::~SrStarPattern()
{
    // Uses GlobalParams like so:
    // Dial1 - Hue 1
    // Dial2 - Number (Dial2 * 100)
    // Dial3 - Threshold
    // Slider1 - Saturation;
    // Slider2 - Brightness;
}

void
SrStarPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    float fftSum = GetAudio()->GetCalibratedFftSum();
    
    float hue;
    float saturation;
    float brightness;
    float number;
    float threshold;
    if (GetGlobalParameters()->UseLocalParams()) {
        hue = _hueParam;
        saturation = _saturationParam;
        brightness = _brightnessParam;
        number = _numberParam;
        threshold = _thresholdParam;
    } else if (GetGlobalParameters()->GetCycleAutomatically()) {
        hue = 1.0;
        saturation = 0.0;
        brightness = 0.8;
        number = 12;
        threshold = 0.4;
    } else {
        hue = GetGlobalParameters()->GetDial1();
        number = GetGlobalParameters()->GetDial2() * 100;
        threshold = GetGlobalParameters()->GetDial3();
        saturation = GetGlobalParameters()->GetSlider1();
        brightness = GetGlobalParameters()->GetSlider2();
    }
    
    if (fftSum < threshold) {
        return;
    }
    
    for (int i = 0; i < number; ++i) {
        int index = rand() % buffer->size();
        
        // Clamp to buffer size just to be safe
        index = std::min(index, (int) buffer->size() - 1);
        
        ofFloatColor c;
        c.setHsb(hue, saturation, brightness);
        
        (*buffer)[index] = c;
    }
}