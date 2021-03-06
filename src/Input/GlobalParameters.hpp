//
//  GlobalParameters.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 8/8/16.
//
//

#ifndef SR_GLOBAL_PARAMETERS_HPP
#define SR_GLOBAL_PARAMETERS_HPP

class SrModel;
class SrAudio;

#include "UiMixin.hpp"

//
// SrGlobalParameters -- a few global knobs and dials that are intended
// to control values across many patterns at once.  These include:
//
//   - cycles that vary periodically, along with the beat.  These cycle from
//     [0,1) unless 'cycleAutomatically' is off.
//
//   - a few manual knobs/dials intended to be surfaced as TouchOSC controls
//
// Patterns should use these use these values to modify the style of their
// display, such as color, speed, decay, etc..
//
// See SrExamplePattern for an example of how to make use of global parameters.
//
// Note:  some parameters are called 'Dial' even though they show up as sliders
// in the default UI.  We intend to represent them as dials through TouchOSC
//
// Patterns can set the labels for the global parameters to indicate
// their meaning.
//
// Global parameters are accessible through OSC with the paths like this:
//
// /Runway/GlobalParams/Dial1
// /Runway/GlobalParams/Dial2
//
class SrGlobalParameters : public SrUiMixin {
    typedef SrGlobalParameters This;
    
public:
    SrGlobalParameters(const std::string & name,
                       SrModel * model, SrAudio * audio);
    virtual ~SrGlobalParameters();
    
    void Update();
    
    float GetCycleAutomatically() const;
    float GetTwoBeatCycle() const;
    float GetMeasureCycle() const;
    float GetPhraseCycle() const;
    float GetSlowCycle() const;
    float GetVerySlowCycle() const;
    
    float GetFadeDuration() const;
    
    float GetDial1() const;
    float GetDial2() const;
    float GetDial3() const;
    float GetSlider1() const;
    float GetSlider2() const;
    
    float ComputeSecondsSinceManualInput() const;
    bool WasRecentManualInput() const;
    
    void OnReceivedManualInput();
    void OnReceivedPresetInput();
    
private:
    float _ComputeUpdate(float value, int beatsPerCycle) const;
    
    void _OnParameterChanged(float & value);
    void _OnCycleAutomaticallyChanged(bool & value);
    
private:
    SrModel * _model;
    SrAudio * _audio;
    float _timeOfLastManualParameterChange;
    float _timeOfLastPresetParameterChange;
    
    ofParameter<bool> _cycleAutomatically;
    ofParameter<bool> _lockToLocalParams;
    ofParameter<float> _delayBeforeAutomaticMode;
    
    ofParameter<float> _twoBeatCycle;
    ofParameter<float> _measureCycle;
    ofParameter<float> _phraseCycle;
    ofParameter<float> _slowCycle;
    ofParameter<float> _verySlowCycle;
    
    ofParameter<float> _fadeDuration;
    
    ofParameter<float> _dial1Parameter;
    ofParameter<float> _dial2Parameter;
    ofParameter<float> _dial3Parameter;
    ofParameter<float> _slider1Parameter;
    ofParameter<float> _slider2Parameter;
    
};


#endif
