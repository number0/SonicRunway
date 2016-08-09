//
//  ArcPattern.cpp
//  SonicRunway
//
//  Created by Paul Roales on 7/14/16.
//
//

#include "ArcPattern.hpp"
#include "Audio.hpp"

SrArcPattern::SrArcPattern(const std::string & name,
                             SrModel * model, SrAudio * audio,
                           SrGlobalParameters * globalParameters) :
SrScrollingPattern(name, model, audio, globalParameters),
_hueParam(0.2),
_thresholdParam(0.5)
{
    _hueParam.setName("Hue");
    _hueParam.setMin(0.0);
    _hueParam.setMax(1.0);
    _AddUIParameter(_hueParam);
    
    _thresholdParam.setName("Threshold");
    _thresholdParam.setMin(0.0);
    _thresholdParam.setMax(1.0);
    _AddUIParameter(_thresholdParam);
    
}

SrArcPattern::~SrArcPattern()
{
    
}

void
SrArcPattern::_Update()
{
    SrScrollingPattern::_Update();
    
    vector<float> fftValues = GetAudio()->GetCurrentFftValues();
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
        if (_fftSumBySegment[segmentI] > _segmentTreshold[segmentI] - _thresholdParam) {
            _segmentTreshold[segmentI] = _segmentTreshold[segmentI] + 0.01;
        } else {
            _segmentTreshold[segmentI] = _segmentTreshold[segmentI] - 0.01;
        }
    }
}

void
SrArcPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    float hue = (float) _hueParam;
    
    int pixels = buffer->size();
    if (segments > pixels) {
        return;
    }
    int pixelsPerSegment = pixels / segments;
    
    for (int segmentI = 0; segmentI < segments; ++segmentI) {
        // If the segment meets the treshhold draw the segment
        if (_fftSumBySegment[segmentI] > _segmentTreshold[segmentI]) {
            ofFloatColor color;
            color.setHsb(hue, 0.8, 0.8);
            for (int pixelI = 0; pixelI < pixelsPerSegment; ++pixelI) {
                int pixel = pixelI + (pixelsPerSegment * segmentI);
                // Clamp to buffer size just to be safe
                int index = std::min(pixel, (int) buffer->size() - 1);
                (*buffer)[index] = color;
            }
        }
    }
    
}