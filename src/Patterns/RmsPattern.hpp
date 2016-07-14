//
//  RmsPattern.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/14/16.
//
//

#ifndef SR_RMS_PATTERN_HPP
#define SR_RMS_PATTERN_HPP

#include "ScrollingPattern.hpp"

//
// A demo pattern that draws the value of the RMS filter
// to the lights for debugging.
//
class SrRmsPattern : public SrScrollingPattern {
public:
    SrRmsPattern(const std::string & name,
                 SrModel * model, SrAudio * audio);
    virtual ~SrRmsPattern();
    
protected:
    virtual void _DrawCurrentGate(std::vector<ofColor> * buffer) const;
    
};



#endif
