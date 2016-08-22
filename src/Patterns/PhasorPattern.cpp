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
#include "Util.hpp"

SrPhasorPattern::SrPhasorPattern(const std::string & name,
                           SrModel * model, SrAudio * audio,
                                 SrGlobalParameters * globalParameters) :
SrScrollingPattern(name, model, audio, globalParameters),
_hueParam(0.75),
_saturationParam(0.8),
_trailParam(90.0),
_thresholdParam(0.04)
{
    _hueParam.setName("Hue");
    _hueParam.setMin(0.0);
    _hueParam.setMax(1.0);
    _AddUIParameter(_hueParam);
    
    _saturationParam.setName("Saturation");
    _saturationParam.setMin(0.0);
    _saturationParam.setMax(1.0);
    _AddUIParameter(_saturationParam);
    
    _trailParam.setName("Trail");
    _trailParam.setMin(0.0);
    _trailParam.setMax(1.0);
    _AddUIParameter(_trailParam);
    
    _thresholdParam.setName("Threshold");
    _thresholdParam.setMin(0.0);
    _thresholdParam.setMax(1.0);
    _AddUIParameter(_thresholdParam);
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
   
    SrGlobalParameters * globals = GetGlobalParameters();
    
    if (globals->WasRecentManualInput()) {
        threshold = globals->GetDial2();
    } else {
        threshold = _thresholdParam;
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
    
    SrGlobalParameters * globals = GetGlobalParameters();
    
    if (globals->WasRecentManualInput()) {
        hue = GetGlobalParameters()->GetDial1();
        saturation = GetGlobalParameters()->GetSlider1();
        trail = GetGlobalParameters()->GetDial2() * 100;
    } else {
        hue = _hueParam + globals->GetSlowCycle();
        saturation = _saturationParam;
        trail = _trailParam;
    }
    
    hue = SrUtil_ClampCycle(0, 1, hue);
    
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