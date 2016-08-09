//
//  GlobalParameters.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 8/8/16.
//
//

#include "GlobalParameters.hpp"
#include "Model.hpp"

SrGlobalParameters::SrGlobalParameters(const std::string & name,
                                       SrModel * model) :
    SrUiMixin(name),
    _model(model),
    _cycleAutomatically(false),
    _secondsPerCycle(2),
    _dial1Parameter(0.5),
    _dial2Parameter(0.5),
    _slider1Parameter(0.5),
    _slider2Parameter(0.5)
{
    _cycleAutomatically.setName("Auto Cycle");
    
    _secondsPerCycle.setName("Sec / Cycle");
    _secondsPerCycle.setMin(0.1);
    _secondsPerCycle.setMax(60.0);
    
    _dial1Parameter.setName("Dial 1");
    _dial1Parameter.setMin(0.0);
    _dial1Parameter.setMax(1.0);
    
    _dial2Parameter.setName("Dial 2");
    _dial2Parameter.setMin(0.0);
    _dial2Parameter.setMax(1.0);
    
    _slider1Parameter.setName("Slider 1");
    _slider1Parameter.setMin(0.0);
    _slider1Parameter.setMax(1.0);
    
    _slider2Parameter.setName("Slider 2");
    _slider2Parameter.setMin(0.0);
    _slider2Parameter.setMax(1.0);
    
    _AddUIParameter(_cycleAutomatically);
    _AddUIParameter(_secondsPerCycle);
    _AddUIParameter(_dial1Parameter);
    _AddUIParameter(_dial2Parameter);
    _AddUIParameter(_slider1Parameter);
    _AddUIParameter(_slider2Parameter);
    
}

SrGlobalParameters::~SrGlobalParameters()
{
    
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
SrGlobalParameters::GetSlider1() const
{
    return (float) _slider1Parameter;
}

float
SrGlobalParameters::GetSlider2() const
{
    return (float) _slider2Parameter;
}

void
SrGlobalParameters::Update()
{
    if (not (bool) _cycleAutomatically) {
        return;
    }
   
    float delta = 1.0 / (_secondsPerCycle * _model->ComputeFramesPerSecond());
    
    _dial1Parameter += delta;
    _dial1Parameter = fmod((float) _dial1Parameter, 1.0);
}
