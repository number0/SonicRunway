//
//  PhasorPattern.hpp
//  SonicRunway
//
//  Created by Paul Roales on 7/17/16.
//
//

#ifndef SR_PHASOR_PATTERN_HPP
#define SR_PHASOR_PATTERN_HPP

#include "ScrollingPattern.hpp"

//
// SrPhasorPattern draws a group of white Phasors to the beat.
//
class SrPhasorPattern : public SrScrollingPattern {
public:
    SrPhasorPattern(const std::string & name,
                 SrModel * model, SrAudio * audio,
                    SrGlobalParameters * globalParameters);
    virtual ~SrPhasorPattern();
    
protected:
    virtual void _Update();
    virtual void _DrawCurrentGate(std::vector<ofColor> * buffer) const;
    
private:
    ofParameter<float> _hueParam;
    ofParameter<float> _thresholdParam;
    ofParameter<float> _trailParam;
    
    int segments = 40;
    float _segmentTreshold[40] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int _segmentCountdown[40] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
};


#endif