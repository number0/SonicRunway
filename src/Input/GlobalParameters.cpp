//
//  GlobalParameters.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 8/8/16.
//
//

#include "GlobalParameters.hpp"
#include "Model.hpp"
#include "Audio.hpp"
#include "BeatHistory.hpp"

SrGlobalParameters::SrGlobalParameters(const std::string & name,
                                       SrModel * model,
                                       SrAudio * audio) :
    SrUiMixin(name),
    _model(model),
    _audio(audio),
    _cycleAutomatically(true),
    _delayBeforeAutomaticMode(60.0), // seconds
    _twoBeatCycle(0.0),
    _measureCycle(0.0),
    _phraseCycle(0.0),
    _slowCycle(0.0),
    _verySlowCycle(0.0),
    _fadeDuration(1.0),
    _dial1Parameter(0.25),
    _dial2Parameter(0.5),
    _dial3Parameter(0.75),
    _slider1Parameter(0.5),
    _slider2Parameter(0.5)
{
    _cycleAutomatically.setName("Auto Cycle");
    _cycleAutomatically.addListener(this, &This::_OnCycleAutomaticallyChanged);
    
    _delayBeforeAutomaticMode.setName("Delay b4 auto");
    _delayBeforeAutomaticMode.setMin(0.0);
    _delayBeforeAutomaticMode.setMax(100.0);
    
    _twoBeatCycle.setName("Two Beat");
    _twoBeatCycle.setMin(0.0);
    _twoBeatCycle.setMax(1.0);
    
    _measureCycle.setName("Measure");
    _measureCycle.setMin(0.0);
    _measureCycle.setMax(1.0);
    
    _phraseCycle.setName("Phrase");
    _phraseCycle.setMin(0.0);
    _phraseCycle.setMax(1.0);
    
    _slowCycle.setName("Slow Cycle");
    _slowCycle.setMin(0.0);
    _slowCycle.setMax(1.0);
    
    _verySlowCycle.setName("Very Slow");
    _verySlowCycle.setMin(0.0);
    _verySlowCycle.setMax(1.0);
    
    _fadeDuration.setName("FadeDuration");
    _fadeDuration.setMin(0.01);
    _fadeDuration.setMax(10.0);
    
    _dial1Parameter.setName("Dial1");
    _dial1Parameter.setMin(0.0);
    _dial1Parameter.setMax(1.0);
    
    _dial2Parameter.setName("Dial2");
    _dial2Parameter.setMin(0.0);
    _dial2Parameter.setMax(1.0);
    
    _dial3Parameter.setName("Dial3");
    _dial3Parameter.setMin(0.0);
    _dial3Parameter.setMax(1.0);
    
    _slider1Parameter.setName("Slider1");
    _slider1Parameter.setMin(0.0);
    _slider1Parameter.setMax(1.0);
    
    _slider2Parameter.setName("Slider2");
    _slider2Parameter.setMin(0.0);
    _slider2Parameter.setMax(1.0);
    
    _AddUIParameter(_cycleAutomatically);
    _AddUIParameter(_delayBeforeAutomaticMode);
    
    _AddUIParameter(_fadeDuration);
    _AddUIParameter(_dial1Parameter);
    _AddUIParameter(_dial2Parameter);
    _AddUIParameter(_dial3Parameter);
    _AddUIParameter(_slider1Parameter);
    _AddUIParameter(_slider2Parameter);
    
    _AddUIParameter(_twoBeatCycle);
    _AddUIParameter(_measureCycle);
    _AddUIParameter(_phraseCycle);
    _AddUIParameter(_slowCycle);
    _AddUIParameter(_verySlowCycle);
    
    _dial1Parameter.addListener(this, &This::_OnParameterChanged);
    _dial2Parameter.addListener(this, &This::_OnParameterChanged);
    _slider1Parameter.addListener(this, &This::_OnParameterChanged);
    _slider2Parameter.addListener(this, &This::_OnParameterChanged);
}

SrGlobalParameters::~SrGlobalParameters()
{
    
}

float
SrGlobalParameters::GetCycleAutomatically() const
{
    return (float) _cycleAutomatically;
}

float
SrGlobalParameters::GetTwoBeatCycle() const
{
    return (float) _twoBeatCycle;
}

float
SrGlobalParameters::GetMeasureCycle() const
{
    return (float) _measureCycle;
}

float
SrGlobalParameters::GetPhraseCycle() const
{
    return (float) _phraseCycle;
}

float
SrGlobalParameters::GetSlowCycle() const
{
    return (float) _slowCycle;
}

float
SrGlobalParameters::GetVerySlowCycle() const
{
    return (float) _verySlowCycle;
}

float
SrGlobalParameters::GetFadeDuration() const
{
    return (float) _fadeDuration;
}

float
SrGlobalParameters::GetDial1() const
{
    return (float) _dial1Parameter;
}

float
SrGlobalParameters::GetDial2() const
{
    return (float) _dial2Parameter;
}

float
SrGlobalParameters::GetDial3() const
{
    return (float) _dial3Parameter;
}

float
SrGlobalParameters::GetSlider1() const
{
    return (float) _slider1Parameter;
}

float
SrGlobalParameters::GetSlider2() const
{
    return (float) _slider2Parameter;
}

float
SrGlobalParameters::_ComputeUpdate(float value, int beatsPerCycle) const
{
    float bpm = _audio->GetBeatHistory().GetBpm()[0];
    
    // Clamp in case BPM is unreasonable
    if (bpm < 50.0) {
        bpm = 50.0;
    }
    if (bpm > 200.0) {
        bpm = 200.0;
    }
    
    float beatsPerSecond = bpm / 60.0;
    float secondsPerCycle = beatsPerCycle / beatsPerSecond;
    
    value += 1.0 / (secondsPerCycle * _model->ComputeFramesPerSecond());
    
    return fmod(value, 1.0);
}

void
SrGlobalParameters::Update()
{
    // If we haven't touched anything for a while, turn the
    // cycles back on.
    if (not _cycleAutomatically and
        ComputeSecondsSinceManualInput() > _delayBeforeAutomaticMode) {
        _cycleAutomatically = true;
    }
    
    if ((bool) _cycleAutomatically) {
        _twoBeatCycle = _ComputeUpdate((float) _twoBeatCycle, 2);
        _measureCycle = _ComputeUpdate((float) _measureCycle, 4);
        _phraseCycle = _ComputeUpdate((float) _phraseCycle, 16);
        _slowCycle = _ComputeUpdate((float) _slowCycle, 64);
        _verySlowCycle = _ComputeUpdate((float) _verySlowCycle, 256);
    } else {
        float fadeDurationSeconds = 0.5;
        float fadeDurationFrames = fadeDurationSeconds * _model->ComputeFramesPerSecond();
        float delta = 1.0 / fadeDurationFrames;
        
        _twoBeatCycle -= delta;
        if ((float) _twoBeatCycle < 0.0) {
            _twoBeatCycle = 0.0;
        }
        
        _measureCycle -= delta;
        if ((float) _measureCycle < 0.0) {
            _measureCycle = 0.0;
        }
        _phraseCycle -= delta;
        if ((float) _phraseCycle < 0.0) {
            _phraseCycle = 0.0;
        }
        
        _slowCycle -= delta;
        if ((float) _slowCycle < 0.0) {
            _slowCycle = 0.0;
        }
        
        _verySlowCycle -= delta;
        if ((float) _verySlowCycle < 0.0) {
            _verySlowCycle = 0.0;
        }
    }
}

void
SrGlobalParameters::OnReceivedManualInput()
{
    _timeOfLastManualParameterChange = ofGetElapsedTimef();
}

void
SrGlobalParameters::_OnParameterChanged(float & value)
{
    OnReceivedManualInput();
}

void
SrGlobalParameters::_OnCycleAutomaticallyChanged(bool & value)
{
    // Turning off cycling counts as manual input.
    if (not value) {
        OnReceivedManualInput();
    }
}

float
SrGlobalParameters::ComputeSecondsSinceManualInput() const
{
    return ofGetElapsedTimef() - _timeOfLastManualParameterChange;
}
