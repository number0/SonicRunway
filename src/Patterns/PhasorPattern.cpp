//
//  PhasorPattern.cpp
//  SonicRunway
//
//  Created by Paul Roales on 7/14/16.
//
//

#include "PhasorPattern.hpp"
#include "Audio.hpp"
#include "GlobalParameters.hpp"

SrPhasorPattern::SrPhasorPattern(const std::string & name,
                           SrModel * model, SrAudio * audio,
                                 SrGlobalParameters * globalParameters) :
SrScrollingPattern(name, model, audio, globalParameters)
{
    
}

SrPhasorPattern::~SrPhasorPattern()
{
    // Uses GlobalParams like so:
    // Dial1 - Hue
    // Dial2 - Threshold
    // Dial3 - Trail (Dial3 * 100)
    // Slider1 - Saturation
}

void
SrPhasorPattern::_Update()
{
    SrScrollingPattern::_Update();
    
    float threshold;
    if (GetGlobalParameters()->GetCycleAutomatically()) {
        threshold = 0.05;
    } else {
        threshold = GetGlobalParameters()->GetDial2();
    }

    for (int i = 0; i < segments; ++i) {
        if (_segmentCountdown[i] > 0) {
            _segmentCountdown[i] = _segmentCountdown[i] - 2;
        }
    }
    
    vector<float> fftValues = GetAudio()->GetCurrentRawFftValues();
    for (int i = 0; i < segments; ++i) {
        if (fftValues.at(i) > threshold) {
            _segmentCountdown[i] = 100;
            _segmentTreshold[i] = _segmentTreshold[i] - 0.01;
        } else {
            if (_segmentTreshold[i] > _segmentTreshold[i] - threshold) {
                _segmentTreshold[i] = _segmentTreshold[i] + 0.01;
            }
        }
    }
}

void
SrPhasorPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    int pixels = buffer->size();
    
    float hue;
    float saturation;
    float trail;
    if (GetGlobalParameters()->GetCycleAutomatically()) {
        hue = GetGlobalParameters()->GetSlowCycle();
        saturation = 0.7;
        trail = 75.0;
    } else {
        hue = GetGlobalParameters()->GetDial1();
        saturation = GetGlobalParameters()->GetSlider1();
        trail = GetGlobalParameters()->GetDial2() * 100;
    }
    
    // Protection
    if (segments > pixels) {
        return;
    }
    int pixelsPerSegment = pixels / segments;
    
    for (int segmentI = 0; segmentI < segments; ++segmentI) {
        // If the segment meets the treshhold draw the segment
        if (_segmentCountdown[segmentI] > 0) {
            int rotatedSegment = (segmentI + 18) % segments;
            ofFloatColor color;
            float trail = (float) _segmentCountdown[segmentI] / trail;
            color.setHsb(hue, saturation, trail);
            for (int pixelI = 0; pixelI < pixelsPerSegment; ++pixelI) {
                int pixel = pixelI + (pixelsPerSegment * rotatedSegment);
                // Clamp to buffer size just to be safe
                int index = std::min(pixel, (int) buffer->size() - 1);
                (*buffer)[index] = color;
            }
        }
    }
    
}