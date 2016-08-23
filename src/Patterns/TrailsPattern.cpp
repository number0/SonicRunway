//
//  TrailsPattern.cpp
//  SonicRunway
//
//  Created by Paul Roales on 7/17/16.
//
//

#include "TrailsPattern.hpp"
#include "Audio.hpp"
#include "GlobalParameters.hpp"
#include "Util.hpp"

SrTrailsPattern::SrTrailsPattern(const std::string & name,
                                 SrModel * model, SrAudio * audio,
                                 SrGlobalParameters * globalParameters) :
SrScrollingPattern(name, model, audio, globalParameters),
_hueParam(0.75),
_jitterParam(0.5),
_saturationParam(0.8),
_brightnessParam(0.8)
{
    _hueParam.setName("Hue");
    _hueParam.setMin(0.0);
    _hueParam.setMax(1.0);
    _AddUIParameter(_hueParam);
    
    _jitterParam.setName("Jitter");
    _jitterParam.setMin(0.0);
    _jitterParam.setMax(1.0);
    _AddUIParameter(_jitterParam);
    
    _saturationParam.setName("Saturation");
    _saturationParam.setMin(0.0);
    _saturationParam.setMax(1.0);
    _AddUIParameter(_saturationParam);
    
    _brightnessParam.setName("Brightness");
    _brightnessParam.setMin(0.0);
    _brightnessParam.setMax(1.0);
    _AddUIParameter(_brightnessParam);
}

SrTrailsPattern::~SrTrailsPattern()
{
    // Uses GlobalParams like so:
    // Dial1 - Hue
    // Dial2 - Jitter (Dial2 * 100)
    // Slider1 - Saturation
    // Slider2 - Brightness
}

void
SrTrailsPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    int pixels = buffer->size();
    vector<float> fftValues = GetAudio()->GetCurrentRawFftValues();
    
    float hue;
    float jitter;
    float saturation;
    float brightness;
    
    SrGlobalParameters * globals = GetGlobalParameters();
    
    if (globals->WasRecentManualInput()) {
        hue = GetGlobalParameters()->GetDial1();
        jitter = GetGlobalParameters()->GetDial2() * 100;
        saturation = GetGlobalParameters()->GetSlider1();
        brightness = GetGlobalParameters()->GetSlider2();
    } else {
        hue = _hueParam + globals->GetSlowCycle();
        jitter = _jitterParam * 100;
        saturation = _saturationParam;
        brightness = _brightnessParam;
    }
    
    hue = SrUtil_ClampCycle(0, 1, hue);
    
    // Protection
    if (segments > pixels) {
        return;
    }
    int pixelsPerSegment = pixels / segments + 1;
    
    for (int i = 0; i < segments; ++i) {
        ofFloatColor color;
        color.setHsb(hue, saturation, brightness);
        
        int pixel = pixelsPerSegment * i;
        int pixelJitter = pixel + fftValues.at(i) * jitter;
        
        // Clamp to buffer size just to be safe
        int index = std::min(pixelJitter, (int) buffer->size() - 1);
        (*buffer)[index] = color;

    }
    
}