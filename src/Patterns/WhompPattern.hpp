//
//  WhompPattern.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 8/16/16.
//
//

#ifndef SR_WHOMP_PATTERN_HPP
#define SR_WHOMP_PATTERN_HPP

#include "ScrollingPattern.hpp"

//
// Renders low-pass results to alternating gates
//
class SrWhompPattern : public SrScrollingPattern {
public:
    SrWhompPattern(const std::string & name,
                 SrModel * model, SrAudio * audio,
                 SrGlobalParameters * globalParameters);
    virtual ~SrWhompPattern();
    
protected:
    virtual void _DrawCurrentGate(std::vector<ofColor> * buffer) const;
    
    virtual void _Update();
    
    virtual bool _FlipLeftRight() const;
    
private:
    
private:
    bool _flipLeftRight;
    
    ofParameter<float> _exponent;
    ofParameter<float> _multiplier;
    ofParameter<float> _scale;
    ofParameter<float> _hueOffset;
    ofParameter<float> _hueShift;
    ofParameter<float> _minSaturation;
    ofParameter<float> _maxSaturation;
    ofParameter<float> _cutoffValue;
    ofParameter<bool> _vibrate;
    ofParameter<bool> _cycleHues;
};

#endif
