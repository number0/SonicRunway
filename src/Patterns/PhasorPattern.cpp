//
//  PhasorPattern.cpp
//  SonicRunway
//
//  Created by Paul Roales on 7/14/16.
//
//

#include "PhasorPattern.hpp"
#include "Audio.hpp"

SrPhasorPattern::SrPhasorPattern(const std::string & name,
                           SrModel * model, SrAudio * audio,
                                 SrGlobalParameters * globalParameters) :
SrScrollingPattern(name, model, audio, globalParameters),
_hueParam(0.75),
_thresholdParam(0.22),
_trailParam(90)
{
    _hueParam.setName("Hue");
    _hueParam.setMin(0.0);
    _hueParam.setMax(1.0);
    _AddUIParameter(_hueParam);
    
    _thresholdParam.setName("Threshold");
    _thresholdParam.setMin(0.0);
    _thresholdParam.setMax(0.5);
    _AddUIParameter(_thresholdParam);
    
    _trailParam.setName("Trail");
    _trailParam.setMin(0.0);
    _trailParam.setMax(100.0);
    _AddUIParameter(_trailParam);
    
}

SrPhasorPattern::~SrPhasorPattern()
{
    
}

void
SrPhasorPattern::_Update()
{
    SrScrollingPattern::_Update();
    
    float threshold = (float) _thresholdParam;

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
            if (_segmentTreshold[i] > _segmentTreshold[i] - _thresholdParam) {
                _segmentTreshold[i] = _segmentTreshold[i] + 0.01;
            }
        }
    }
}

void
SrPhasorPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    float hueParam = (float) _hueParam;
    float trailParam = (float) _trailParam;
    int pixels = buffer->size();
    
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
            float trail = (float) _segmentCountdown[segmentI] / trailParam;
            color.setHsb(hueParam, 0.7, trail);
            for (int pixelI = 0; pixelI < pixelsPerSegment; ++pixelI) {
                int pixel = pixelI + (pixelsPerSegment * rotatedSegment);
                // Clamp to buffer size just to be safe
                int index = std::min(pixel, (int) buffer->size() - 1);
                (*buffer)[index] = color;
            }
        }
    }
    
}