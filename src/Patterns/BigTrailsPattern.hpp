//
//  BigTrailsPattern.hpp
//  SonicRunway
//
//  Created by Paul Roales on 7/17/16.
//
//

#ifndef SR_BIGTrails_PATTERN_HPP
#define SR_BIGTrails_PATTERN_HPP

#include "ScrollingPattern.hpp"

//
// SrBigTrailsPattern draws a group of white BigTrailss to the beat.
//
class SrBigTrailsPattern : public SrScrollingPattern {
public:
    SrBigTrailsPattern(const std::string & name,
                    SrModel * model, SrAudio * audio,
                       SrGlobalParameters * globalParameters);
    virtual ~SrBigTrailsPattern();
    
protected:
    virtual void _Update();
    virtual void _DrawCurrentGate(std::vector<ofColor> * buffer) const;
    
private:
    float _average;
    
    ofParameter<float> _hueParam;
    ofParameter<float> _saturationParam;
    ofParameter<float> _brightnessParam;
    ofParameter<float> _jitterParam;
    ofParameter<float> _rotationParam;
};


#endif