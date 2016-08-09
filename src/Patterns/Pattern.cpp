//
//  Pattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 3/2/16.
//
//

#include "Pattern.hpp"
#include "Model.hpp"
#include "Audio.hpp"
#include "Debug.hpp"

SrPattern::SrPattern(const std::string & name,
                     SrModel * model,
                     SrAudio * audio) :
    SrUiMixin(name),
    _model(model),
    _audio(audio),
    _enabledBuffer(model),
    _enabledParam(false),
    _opacityBuffer(model)
{
    SrDebug("constructed pattern %s\n", name.c_str());
    _enabledToggle.setup(_enabledParam);
    _enabledParam.setName("Enabled");
    _AddUIParameter(_enabledParam);
}

SrPattern::~SrPattern()
{
    SrDebug("destroyed pattern\n");
}

SrModel *
SrPattern::GetModel() const
{
    return _model;
}

SrAudio *
SrPattern::GetAudio() const
{
    return _audio;
}

SrGlobalParameters *
SrPattern::GetGlobalParameters() const
{
    return _model->GetGlobalParameters();
}

void
SrPattern::SetEnabled(bool enabled)
{
    _enabledParam = enabled;
}

const SrFloatSimpleBuffer &
SrPattern::GetOpacity() const
{
    return _opacityBuffer;
}

bool
SrPattern::IsOnAtAnyGate() const
{
    for(size_t i = 0; i < _opacityBuffer.size(); i++) {
        if (_opacityBuffer[i] > 0.0) {
            return true;
        }
    }
    
    return false;
}

void
SrPattern::Update()
{
    bool enabled = (bool) _enabledToggle;
    _enabledBuffer.Push((float) enabled);
    
    float lastOpacity = _opacityBuffer[0];
    float thisOpacity = lastOpacity;
    
    if (lastOpacity != (float) enabled) {
        float fadeDurationSeconds = 1.0;
        float fadeDurationFrames = fadeDurationSeconds * _model->ComputeFramesPerSecond();
        
        float delta = 1.0 / fadeDurationFrames;
       
        // Nudge value
        thisOpacity = lastOpacity;
        if (enabled) {
            thisOpacity += delta;
        } else {
            thisOpacity -= delta;
        }
        
        // Clamp
        if (thisOpacity > 1.0) {
            thisOpacity = 1.0;
        }
        if (thisOpacity < 0.0) {
            thisOpacity = 0.0;
        }
    }
    
    _opacityBuffer.Push(thisOpacity);
    
    // Call subclass update
    _Update();
}

void
SrPattern::Draw()
{
    
    // Call subclass draw
    _Draw();
}
