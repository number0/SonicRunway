//
//  ArcPattern.hpp
//  SonicRunway
//
//  Created by Paul Roales on 7/17/16.
//
//

#ifndef SR_ARC_PATTERN_HPP
#define SR_ARC_PATTERN_HPP

#include "ScrollingPattern.hpp"

//
// SrArcPattern draws a group of white Arcs to the beat.
//
class SrArcPattern : public SrScrollingPattern {
public:
    SrArcPattern(const std::string & name,
                  SrModel * model, SrAudio * audio,
                 SrGlobalParameters * globalParameters);
    virtual ~SrArcPattern();
    
protected:
    virtual void _Update();
    virtual void _DrawCurrentGate(std::vector<ofColor> * buffer) const;
    
private:
    ofParameter<float> _hueParam;
    ofParameter<float> _thresholdParam;
    
    int segments = 10;
    float _segmentTreshold[10] = {0,0,0,0,0,0,0,0,0,0};
    float _fftSumBySegment[10] = {0,0,0,0,0,0,0,0,0,0};
};


#endif