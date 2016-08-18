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

SrTrailsPattern::SrTrailsPattern(const std::string & name,
                                 SrModel * model, SrAudio * audio,
                                 SrGlobalParameters * globalParameters) :
SrScrollingPattern(name, model, audio, globalParameters)
{

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
    if (GetGlobalParameters()->GetCycleAutomatically()) {
        hue = GetGlobalParameters()->GetSlowCycle();
        jitter = 50.0;
        saturation = 0.7;
        brightness = 0.8;
    } else {
        hue = GetGlobalParameters()->GetDial1();
        jitter = GetGlobalParameters()->GetDial2() * 100;
        saturation = GetGlobalParameters()->GetSlider1();
        brightness = GetGlobalParameters()->GetSlider2();
    }
    
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