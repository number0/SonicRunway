//
//  FftPattern.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 3/4/16.
//
//

#ifndef SR_FFT_PATTERN_HPP
#define SR_FFT_PATTERN_HPP

#include <stdio.h>

#include "ScrollingPattern.hpp"

//
// A Pattern that renders the fftBuffer to the light array
//
class SrFftPattern : public SrScrollingPattern {
public:
    SrFftPattern(const std::string & name,
                 SrModel * model, SrAudio * audio,
                 SrGlobalParameters * globalParameters);
    virtual ~SrFftPattern();
    
protected:
    virtual void _DrawCurrentGate(std::vector<ofColor> * buffer) const;
    
private:
    float _ComputeValue(float fftValue) const;
    ofFloatColor _ComputeColor(float fftValue, float hueOffset) const;
    
private:
    ofParameter<float> _exponent;
    ofParameter<float> _spreadExponent;
    ofParameter<float> _multiplier;
    ofParameter<float> _hueOffset;
    ofParameter<float> _hueShift;
    ofParameter<bool> _mirror;
    ofParameter<float> _scale;
    ofParameter<bool> _scaleRegions;
    ofParameter<bool> _spinSlowly;
    ofParameter<bool> _cycleHues;
};

#endif
