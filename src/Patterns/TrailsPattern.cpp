//
//  TrailsPattern.cpp
//  SonicRunway
//
//  Created by Paul Roales on 7/17/16.
//
//

#include "TrailsPattern.hpp"
#include "Audio.hpp"

SrTrailsPattern::SrTrailsPattern(const std::string & name,
                                 SrModel * model, SrAudio * audio,
                                 SrGlobalParameters * globalParameters) :
SrScrollingPattern(name, model, audio, globalParameters),
_hueParam(0.75),
_jitterParam(20.0)
{
    _hueParam.setName("Hue");
    _hueParam.setMin(0.0);
    _hueParam.setMax(1.0);
    _AddUIParameter(_hueParam);
    
    _jitterParam.setName("Jitter");
    _jitterParam.setMin(0.0);
    _jitterParam.setMax(100.0);
    _AddUIParameter(_jitterParam);
}

SrTrailsPattern::~SrTrailsPattern()
{
    
}

void
SrTrailsPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    float hueParam = (float) _hueParam;
    float jitterParam = (float) _jitterParam;
    int pixels = buffer->size();
    vector<float> fftValues = GetAudio()->GetCurrentRawFftValues();
    
    // Protection
    if (segments > pixels) {
        return;
    }
    int pixelsPerSegment = pixels / segments + 1;
    
    for (int i = 0; i < segments; ++i) {
        ofFloatColor color;
        color.setHsb(hueParam, 0.7, 0.8);
        
        int pixel = pixelsPerSegment * i;
        int pixelJitter = pixel + fftValues.at(i) * jitterParam;
        
        // Clamp to buffer size just to be safe
        int index = std::min(pixelJitter, (int) buffer->size() - 1);
        (*buffer)[index] = color;

    }
    
}