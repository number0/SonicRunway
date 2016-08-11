//
//  ExamplePattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 3/17/16.
//
//

#include "ExamplePattern.hpp"
#include "GlobalParameters.hpp"
#include "Util.hpp"
#include <algorithm>

SrExamplePattern::SrExamplePattern(const std::string & name,
                                   SrModel * model, SrAudio * audio,
                                   SrGlobalParameters * globalParameters) :
    SrScrollingPattern(name, model, audio, globalParameters),
    _hueParam(0.0),
    _angleParam(135.0)
{
    _hueParam.setName("Hue");
    _hueParam.setMin(0.0);
    _hueParam.setMax(1.0);
    _AddUIParameter(_hueParam);
    
    _angleParam.setName("Angle");
    _angleParam.setMin(0.0);
    _angleParam.setMax(270.0);
    _AddUIParameter(_angleParam);
}

SrExamplePattern::~SrExamplePattern()
{
    
}

bool
SrExamplePattern::IsAudioReactive() const
{
    return false;
}

void
SrExamplePattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    float hue = _hueParam + GetGlobalParameters()->GetTwoBeatCycle() +
                GetGlobalParameters()->GetDial1();
    
    hue = SrUtil_ClampCycle(0.0, 1.0, hue);
    
    float angle = _angleParam;
    
    float t = angle / 270; // degrees
    int index = t * buffer->size();
    
    // Clamp to buffer size
    index = std::min(index, (int) buffer->size() - 1);
    
    ofFloatColor c;
    c.setHsb(hue, 1.0, 0.8);
    
    (*buffer)[index] = c;
}
