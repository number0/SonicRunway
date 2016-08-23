//
//  AmbientRainbowPattern.hpp
//  SonicRunway
//
//  Created by jdn on 7/9/16.
//
//

#ifndef SR_AMBIENT_RAINBOW_PATTERN_HPP
#define SR_AMBIENT_RAINBOW_PATTERN_HPP

#include "ScrollingPattern.hpp"
//#include "BeatPattern.hpp"

//
// SrAmbientRainbowPattern draws the beat with rainbow colors
//
class SrAmbientRainbowPattern : public SrScrollingPattern {
public:
    SrAmbientRainbowPattern(const std::string & name,
                     SrModel * model, SrAudio * audio,
                     SrGlobalParameters * globalParameters);
    virtual ~SrAmbientRainbowPattern();
    
    virtual std::string GetGlobalParameterLabel(const std::string & name) const;
    
    virtual bool IsAudioReactive() const;
    
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
