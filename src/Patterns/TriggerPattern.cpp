//
//  TriggerPattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/9/16.
//
//

#include "TriggerPattern.hpp"
#include "Audio.hpp"
#include "GlobalParameters.hpp"
#include "Util.hpp"

SrTriggerPattern::SrTriggerPattern(const std::string & name,
                                   SrModel * model, SrAudio * audio,
                                   SrGlobalParameters * globalParameters) :
    SrScrollingPattern(name, model, audio, globalParameters),
    _triggerParam(false),
    _triggerWasPressed(false),
    _trailLength(10),
    _framesSinceTrigger(_trailLength)
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
        _framesSinceTrigger = 0;
    } else {
        if (_framesSinceTrigger <= _trailLength) {
            _framesSinceTrigger++;
        }
    }
}

void
SrTriggerPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    if (_framesSinceTrigger >= _trailLength) {
        return;
    }
    
    SrGlobalParameters * globals = GetGlobalParameters();
    
    float hue = globals->GetVerySlowCycle();
    hue = SrUtil_ClampCycle(0, 1, hue);
    float valueT = (float) _framesSinceTrigger / _trailLength;
    float value = 1.0 - valueT;
    
    ofFloatColor color;
    color.setHsb(hue, 0.8, value);
    for(int i = 0; i < buffer->size(); i++) {
        (*buffer)[i] = color;
    }
}