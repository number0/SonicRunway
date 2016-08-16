//
//  TrailsPattern.hpp
//  SonicRunway
//
//  Created by Paul Roales on 7/17/16.
//
//

#ifndef SR_TRAILS_PATTERN_HPP
#define SR_TRAILS_PATTERN_HPP

#include "ScrollingPattern.hpp"

//
// SrTrailsPattern draws a group of white Trailss to the beat.
//
class SrTrailsPattern : public SrScrollingPattern {
public:
    SrTrailsPattern(const std::string & name,
                    SrModel * model, SrAudio * audio,
                    SrGlobalParameters * globalParameters);
    virtual ~SrTrailsPattern();
    
protected:
    virtual void _DrawCurrentGate(std::vector<ofColor> * buffer) const;
    
private:
    ofParameter<float> _hueParam;
    ofParameter<float> _jitterParam;
    
    int segments = 20;
};


#endif