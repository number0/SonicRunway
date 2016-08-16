//
//  RainbowPattern.hpp
//  SonicRunway
//
//  Created by Paul Roales on 7/16/16.
//
//

#ifndef SR_RAINBOW_PATTERN_HPP
#define SR_RAINBOW_PATTERN_HPP

#include "ScrollingPattern.hpp"

//
// SrRainbowPattern draws a group of white Rainbows to the beat.
//
class SrRainbowPattern : public SrScrollingPattern {
public:
    SrRainbowPattern(const std::string & name,
                  SrModel * model, SrAudio * audio,
                     SrGlobalParameters * globalParameters);
    virtual ~SrRainbowPattern();
    
protected:
    virtual void _Update();
    virtual void _DrawCurrentGate(std::vector<ofColor> * buffer) const;
    
private:
    ofParameter<bool> _manualControlParam;
    ofParameter<float> _hueBiasParam;
    ofParameter<float> _saturationParam;
    ofParameter<float> _brightnessParam;
    
    float _hueBiasChange;
    float _saturationChange;
    float _brightnessChange;
};


#endif