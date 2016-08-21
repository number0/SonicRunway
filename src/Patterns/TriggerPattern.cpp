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
                                   SrModel * model, SrAudio * audio,
                                   SrGlobalParameters * globalParameters) :
    SrScrollingPattern(name, model, audio, globalParameters),
    _triggerParam(false),
    _triggerWasPressed(false)
{
    _triggerParam.setName("Trigger");
    _AddUIParameter(_triggerParam);
}

SrTriggerPattern::~SrTriggerPattern()
{
    
}

bool
SrTriggerPattern::IsAudioReactive() const
{
    return false;
}

void
SrTriggerPattern::_Update()
{
    SrScrollingPattern::_Update();
    
    _triggerWasPressed = (bool) _triggerParam;
    if (_triggerParam) {
        _triggerParam = false;
    }
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