//
//  PaletteBandsPattern.hpp
//  SonicRunway
//
//  Created by Michael Frederickson on 7/6/16.
//
//

#ifndef SR_PALETTEBANDS_PATTERN_HPP
#define SR_PALETTEBANDS_PATTERN_HPP

#include "ScrollingPattern.hpp"

//
// SrPaletteBandsPattern draws the beat
//
class SrPaletteBandsPattern : public SrScrollingPattern {
public:
    SrPaletteBandsPattern(const std::string & name,
                  SrModel * model, SrAudio * audio,
                  SrGlobalParameters * globalParameters);
    virtual ~SrPaletteBandsPattern();
    
protected:
    virtual void _DrawCurrentGate(std::vector<ofColor> * buffer) const;
    
    virtual void _Update();
    
private:
    ofParameter<float> _hueParam;
    ofParameter<float> _decayTimeParam;
    ofParameter<float> _spinSpeedParam;
    
    std::vector<std::vector<ofFloatColor>> _palettes;
    
    float _spinOffset;

private:
    std::vector<ofFloatColor> _RandomPalette() const;
    
};

#endif
