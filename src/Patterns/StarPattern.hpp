//
//  StarPattern.hpp
//  SonicRunway
//
//  Created by Paul Roales on 7/14/16.
//
//

#ifndef SR_STAR_PATTERN_HPP
#define SR_STAR_PATTERN_HPP

#include "ScrollingPattern.hpp"

//
// SrStarPattern draws a group of white stars to the beat.
//
class SrStarPattern : public SrScrollingPattern {
public:
    SrStarPattern(const std::string & name,
                  SrModel * model, SrAudio * audio);
    virtual ~SrStarPattern();
    
protected:
    virtual void _DrawCurrentGate(std::vector<ofColor> * buffer) const;
    
private:
    ofParameter<float> _numberParam;
    ofParameter<float> _thresholdParam;
};


#endif