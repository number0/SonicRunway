//
//  DropoutPattern.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/9/16.
//
//

#ifndef SR_DROPOUT_PATTERN_HPP
#define SR_DROPOUT_PATTERN_HPP

#include "ScrollingPattern.hpp"

//
// SrDropoutPattern demonstrates how to set up a
// button and osc receiver and respond to clicks or signals
//
class SrDropoutPattern : public SrScrollingPattern {
public:
    SrDropoutPattern(const std::string & name,
                     SrModel * model, SrAudio * audio,
                     SrGlobalParameters * globalParameters);
    virtual ~SrDropoutPattern();
    
    virtual bool IsAudioReactive() const;
    
protected:
    virtual void _DrawCurrentGate(std::vector<ofColor> * buffer) const;
    
    virtual void _Update();
    
private:
    ofParameter<bool> _dropoutParam;
    bool _dropoutWasPressed;
    int _trailLength;
    int _framesSinceDropout;
};

#endif
