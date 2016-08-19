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
                 SrModel * model, SrAudio * audio,
                 SrGlobalParameters * globalParameters);
    virtual ~SrRmsPattern();
    
protected:
    virtual void _DrawCurrentGate(std::vector<ofColor> * buffer) const;

private:
    ofParameter<float> _hue1Param;
    ofParameter<float> _hue2Param;
    ofParameter<float> _hue3Param;
    ofParameter<float> _saturationParam;
    ofParameter<float> _brightnessParam;
    
};



#endif
