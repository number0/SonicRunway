//
//  StripesPattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 3/22/16.
//
//

#include "StripesPattern.hpp"
#include "GlobalParameters.hpp"
#include "Util.hpp"

SrStripesPattern::SrStripesPattern(const std::string & name,
                                   SrModel * model, SrAudio * audio,
                                   SrGlobalParameters * globalParameters) :
    SrScrollingPattern(name, model, audio, globalParameters),
    _hueParam(0.0),
    _numStripesParam(6),
    _spinSpeedParam(300.0), // revolutions per second
    _angle(0.0) // degrees
{
    _hueParam.setName("Hue");
    _hueParam.setMin(0.0);
    _hueParam.setMax(1.0);
    _AddUIParameter(_hueParam);
    
    _numStripesParam.setName("NumStripes");
    _numStripesParam.setMin(0);
    _numStripesParam.setMax(60);
    _AddUIParameter(_numStripesParam);
    
    _spinSpeedParam.setName("SpinSpeed");
    _spinSpeedParam.setMin(-2000.0);
    _spinSpeedParam.setMax(2000.0);
    _AddUIParameter(_spinSpeedParam);
}

SrStripesPattern::~SrStripesPattern()
{
    
}

std::string
SrStripesPattern::GetGlobalParameterLabel(const std::string & name) const
{
    if (name == "Dial1") {
        return "Hue";
    }
    
    return std::string();
}

void
SrStripesPattern::_Update()
{
    SrScrollingPattern::_Update();
    
    _angle += _spinSpeedParam / GetModel()->ComputeFramesPerSecond();
}

void
SrStripesPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    const SrModel * model = GetModel();
    
    // Extract the values for each parameter for this gate
    
    float hue = (float) _hueParam + GetGlobalParameters()->GetDial1() +
                GetGlobalParameters()->GetSlowCycle();
    hue = SrUtil_ClampCycle(0.0, 1.0, hue);
    
    float t = fmod(_angle, 360.0) / 360.0;
    int ledIndex = t * buffer->size();
    
    ofFloatColor c;
    int numDecay = (float) buffer->size() * 0.2;
    for(int i = 0; i < numDecay; i++) {
        float t = (float) i / numDecay;
        float thisHue = fmod(hue + t * 0.2, 1.0);
        c.setHsb(thisHue, 1.0, (1.0-t));
        
        int thisIndex = ledIndex + i;
        if (thisIndex >= buffer->size()) {
            continue;
        }
        
        (*buffer)[ledIndex + i] = c;
    }
}
