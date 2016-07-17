//
//  StarPattern.cpp
//  SonicRunway
//
//  Created by Paul Roales on 7/14/16.
//
//

#include "StarPattern.hpp"
#include "Audio.hpp"

SrStarPattern::SrStarPattern(const std::string & name,
                             SrModel * model, SrAudio * audio) :
SrScrollingPattern(name, model, audio),
_numberParam(12.0),
_thresholdParam(0.4)
{
    _numberParam.setName("Number");
    _numberParam.setMin(0.0);
    _numberParam.setMax(30.0);
    _AddUIParameter(_numberParam);
    
    _thresholdParam.setName("Threshold");
    _thresholdParam.setMin(0.0);
    _thresholdParam.setMax(1.0);
    _AddUIParameter(_thresholdParam);
}

SrStarPattern::~SrStarPattern()
{
    
}

void
SrStarPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    int number = static_cast<int>(_numberParam);
    float threshold = _thresholdParam;
    float fftSum = GetAudio()->GetCalibratedFftSum();
    
    if (fftSum < threshold) {
        return;
    }
    
    for (int i = 0; i < number; ++i) {
        int index = rand() % buffer->size();
        
        // Clamp to buffer size just to be safe
        index = std::min(index, (int) buffer->size() - 1);
        
        ofFloatColor c;
        c.setHsb(number, 0.0, 0.8);
        
        (*buffer)[index] = c;
    }
}