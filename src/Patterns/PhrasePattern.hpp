//
//  PhrasePattern.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/9/16.
//
//

#ifndef SR_PHRASE_PATTERN_HPP
#define SR_PHRASE_PATTERN_HPP

#include "ScrollingPattern.hpp"

//
// SrPhrasePattern visualizes the beat and phrase indices
//
class SrPhrasePattern : public SrScrollingPattern {
public:
    SrPhrasePattern(const std::string & name,
                    SrModel * model, SrAudio * audio,
                    SrGlobalParameters * globalParameters);
    virtual ~SrPhrasePattern();
    
protected:
    virtual void _DrawCurrentGate(std::vector<ofColor> * buffer) const;
    
    virtual void _Update();
    
private:
    
};

#endif 
