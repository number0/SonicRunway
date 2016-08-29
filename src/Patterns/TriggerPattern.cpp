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
    
    /*
    
    _triggerLeftWasPressed = (bool) _triggerLeftParam;
    if (_triggerLeftParam) {
        _triggerLeftParam = false;
        _framesSinceLeftTrigger = 0;
    } else {
        if (_framesSinceLeftTrigger <= _trailLength) {
            _framesSinceLeftTrigger++;
        }
    }
    
    _triggerTopWasPressed = (bool) _triggerTopParam;
    if (_triggerTopParam) {
        _triggerTopParam = false;
        _framesSinceTopTrigger = 0;
    } else {
        if (_framesSinceTopTrigger <= _trailLength) {
            _framesSinceTopTrigger++;
        }
    }
    
    _triggerRightWasPressed = (bool) _triggerRightParam;
    if (_triggerRightParam) {
        _triggerRightParam = false;
        _framesSinceRightTrigger = 0;
    } else {
        if (_framesSinceRightTrigger <= _trailLength) {
            _framesSinceRightTrigger++;
        }
    }
    
    _triggerBoomWasPressed = (bool) _triggerBoomParam;
    if (_triggerBoomParam) {
        _triggerBoomParam = false;
        _framesSinceBoomTrigger = 0;
    } else {
        if (_framesSinceBoomTrigger <= _trailLength) {
            _framesSinceBoomTrigger++;
        }
    }
    */
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
    
    /*
    int third = buffer->size()/3;
    
    hue = SrUtil_ClampCycle(0, 1, hue);
    valueT = (float) _framesSinceLeftTrigger / _trailLength;
    value = 1.0 - valueT;
    color.set(1.0, 0.0, 0.0);
    for(int i = 0; i < third; i++) {
        (*buffer)[i] = color;
    }
    
    hue = SrUtil_ClampCycle(0, 1, hue);
    valueT = (float) _framesSinceTopTrigger / _trailLength;
    value = 1.0 - valueT;
    color.set(0.0, 1.0, 0.0);
    
    for(int i = third; i < 2*third; i++) {
        (*buffer)[i] = color;
    }
    
    hue = SrUtil_ClampCycle(0, 1, hue);
    valueT = (float) _framesSinceRightTrigger / _trailLength;
    value = 1.0 - valueT;
    color.set(0.0, 0.0, 1.0);
    
    for(int i = 2*third; i < (3*third)-1; i++) {
        (*buffer)[i] = color;
    }
     */
}