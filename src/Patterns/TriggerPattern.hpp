//
//  TriggerPattern.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/9/16.
//
//

#ifndef SR_TRIGGER_PATTERN_HPP
#define SR_TRIGGER_PATTERN_HPP

#include "ScrollingPattern.hpp"

//
// SrTriggerPattern demonstrates how to set up a
// button and osc receiver and respond to clicks or signals
//
class SrTriggerPattern : public SrScrollingPattern {
public:
    SrTriggerPattern(const std::string & name,
                     SrModel * model, SrAudio * audio,
                     SrGlobalParameters * globalParameters);
    virtual ~SrTriggerPattern();
    
protected:
    virtual void _DrawCurrentGate(std::vector<ofColor> * buffer) const;
    
    virtual void _Update();
    
private:
    ofParameter<bool> _triggerParam;
    bool _triggerWasPressed;
};

#endif
