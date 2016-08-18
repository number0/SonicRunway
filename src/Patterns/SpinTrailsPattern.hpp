//
//  SpinTrailsPattern.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 8/18/16.
//
//

#ifndef SR_SQUARES_PATTERN_HPP
#define SR_SQUARES_PATTERN_HPP

#include "ScrollingPattern.hpp"

//
// Renders low-pass results as trails
//
class SrSpinTrailsPattern : public SrScrollingPattern {
public:
    SrSpinTrailsPattern(const std::string & name,
                   SrModel * model, SrAudio * audio,
                   SrGlobalParameters * globalParameters);
    virtual ~SrSpinTrailsPattern();
    
protected:
    virtual void _DrawCurrentGate(std::vector<ofColor> * buffer) const;
    
    virtual void _Update();
    
private:
    
private:
    std::vector<int> _offsets;
    std::vector<int> _ages;
    std::vector<int> _velocities;
    std::vector<bool> _flipped;
    std::vector<float> _hues;
    
    ofParameter<int> _numTrails;
    ofParameter<int> _duration;
    ofParameter<int> _wander;
    ofParameter<int> _spin;
    ofParameter<int> _width;
    
    ofParameter<float> _exponent;
    ofParameter<float> _multiplier;
    ofParameter<float> _scale;
    ofParameter<float> _hueOffset;
    ofParameter<float> _hueShift;
    ofParameter<float> _hueVariation;
    ofParameter<float> _minSaturation;
    ofParameter<float> _maxSaturation;
    ofParameter<float> _cutoffValue;
    ofParameter<bool> _cycleHues;
};



#endif
