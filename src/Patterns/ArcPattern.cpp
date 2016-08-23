//
//  ArcPattern.cpp
//  SonicRunway
//
//  Created by Paul Roales on 7/14/16.
//
//

#include "ArcPattern.hpp"
#include "Audio.hpp"
#include "GlobalParameters.hpp"
#include "Util.hpp"

SrArcPattern::SrArcPattern(const std::string & name,
                             SrModel * model, SrAudio * audio,
                           SrGlobalParameters * globalParameters) :
SrScrollingPattern(name, model, audio, globalParameters),
_hueParam(0.2),
_saturationParam(0.8),
_brightnessParam(0.8),
_thresholdParam(0.05)
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
    
    _thresholdParam.setName("Threshold");
    _thresholdParam.setMin(0.0);
    _thresholdParam.setMax(1.0);
    _AddUIParameter(_thresholdParam);
}

SrArcPattern::~SrArcPattern()
{
    // Uses GlobalParams like so:
    // Dial1 - Hue
    // Dial2 - Threshold
    // Slider1 - Saturation
    // Slider2 - Brightness
}

void
SrArcPattern::_Update()
{
    SrScrollingPattern::_Update();
    
    float threshold;
   
    SrGlobalParameters * globals = GetGlobalParameters();
    
    if (globals->WasRecentManualInput()) {
        threshold = GetGlobalParameters()->GetDial2();
    } else {
        threshold = _thresholdParam;
    }
    
    vector<float> fftValues = GetAudio()->GetCurrentRawFftValues();
    int fftSize = fftValues.size();

    // Protection
    if (fftSize < segments) {
        return;
    }
    int fftValuesPerSegment = fftSize / segments;
    
    // Set sums back to zero
    for (int segmentI = 0; segmentI < segments; ++segmentI) {
        _fftSumBySegment[segmentI] = 0;
    }
    for (int segmentI = 0; segmentI < segments; ++segmentI) {
        for (int fftI = 0; fftI < fftValuesPerSegment; ++fftI) {
            // Accumulate fft values by segment
            int fftIndex = fftI + (fftValuesPerSegment * segmentI);
            _fftSumBySegment[segmentI] += fftValues.at(fftIndex);
        }
    }
    
    // Do some slow auto calibration of each segments threashold
    for (int segmentI = 0; segmentI < segments; ++segmentI) {
        if (_fftSumBySegment[segmentI] > _segmentTreshold[segmentI] - threshold) {
            _segmentTreshold[segmentI] = _segmentTreshold[segmentI] + 0.01;
        } else {
            _segmentTreshold[segmentI] = _segmentTreshold[segmentI] - 0.01;
        }
    }
}

void
SrArcPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    float hue;
    float saturation;
    float brightness;
   
    SrGlobalParameters * globals = GetGlobalParameters();
    
    if (globals->WasRecentManualInput()) {
        hue = globals->GetDial1();
        saturation = globals->GetSlider1();
        brightness = globals->GetSlider2();
    } else {
        hue = _hueParam + globals->GetSlowCycle();
        saturation = _saturationParam;
        brightness = _brightnessParam;
    }
    
    hue = SrUtil_ClampCycle(0, 1, hue);
    
    int pixels = buffer->size();
    if (segments > pixels) {
        return;
    }
    int pixelsPerSegment = pixels / segments;
    
    for (int segmentI = 0; segmentI < segments; ++segmentI) {
        // If the segment meets the treshhold draw the segment
        if (_fftSumBySegment[segmentI] > _segmentTreshold[segmentI]) {
            ofFloatColor color;
            color.setHsb(hue, saturation, brightness);
            for (int pixelI = 0; pixelI < pixelsPerSegment; ++pixelI) {
                int pixel = pixelI + (pixelsPerSegment * segmentI);
                // Clamp to buffer size just to be safe
                int index = std::min(pixel, (int) buffer->size() - 1);
                (*buffer)[index] = color;
            }
        }
    }
    
}