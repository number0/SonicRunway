//
//  DoubleRainbowPattern.hpp
//  SonicRunway
//
//  Created by jdn on 7/9/16.
//
//

#ifndef SR_DOUBLE_RAINBOW_PATTERN_HPP
#define SR_DOUBLE_RAINBOW_PATTERN_HPP

#include "ScrollingPattern.hpp"
//#include "BeatPattern.hpp"

//
// SrDoubleRainbowPattern draws the beat with rainbow colors
//
class SrDoubleRainbowPattern : public SrScrollingPattern {
public:
    SrDoubleRainbowPattern(const std::string & name,
                     SrModel * model, SrAudio * audio,
                     SrGlobalParameters * globalParameters);
    virtual ~SrDoubleRainbowPattern();
    
protected:
    virtual void _DrawCurrentGate(std::vector<ofColor> * buffer) const;
    
    virtual void _Update();
    
private:
    ofParameter<float> _hueParam;
    ofParameter<float> _decayTimeParam;
    ofParameter<float> _spinSpeedParam;
    ofParameter<float> _hueSpeedParam;
    ofParameter<float> _doubleRainbowsParam;
    ofParameter<float> _trippelRainbowsParam;
    ofParameter<float> _filterParam;
    
    float _spinOffset;
    float _hueOffset;
};

#endif
