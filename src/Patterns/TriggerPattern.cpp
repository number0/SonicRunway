//
//  TriggerPattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/9/16.
//
//

#include "TriggerPattern.hpp"
#include "Audio.hpp"

SrTriggerPattern::SrTriggerPattern(const std::string & name,
                             SrModel * model, SrAudio * audio) :
    SrScrollingPattern(name, model, audio),
    _triggerParam(false),
    _triggerWasPressed(false)
{
    _triggerParam.setName("Trigger");
    _AddUIParameter(_triggerParam);
}

SrTriggerPattern::~SrTriggerPattern()
{
    
}

void
SrTriggerPattern::_Update()
{
    SrScrollingPattern::_Update();
    
    _triggerWasPressed = (bool) _triggerParam;
    _triggerParam = false;
}

void
SrTriggerPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    if (not _triggerWasPressed) {
        return;
    }
    
    ofFloatColor color;
    color.setHsb(0.5, 1.0, 1.0);
    for(int i = 0; i < buffer->size(); i++) {
        (*buffer)[i] = color;
    }
}