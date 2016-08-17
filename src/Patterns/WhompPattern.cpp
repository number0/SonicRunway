//
//  WhompPattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 8/16/16.
//
//

#include "WhompPattern.hpp"
#include "Audio.hpp"
#include "Model.hpp"
#include "GlobalParameters.hpp"
#include "Util.hpp"

SrWhompPattern::SrWhompPattern(const std::string & name,
                           SrModel * model, SrAudio * audio,
                           SrGlobalParameters * globalParameters) :
    SrScrollingPattern(name, model, audio, globalParameters),
        _exponent(3.6),
        _multiplier(1.0),
        _scale(0.5),
        _flipLeftRight(true),
        _hueOffset(0.0),
        _hueShift(0.5),
        _minSaturation(0.3),
        _maxSaturation(1.0),
        _cutoffValue(0.1),
        _vibrate(false),
        _cycleHues(true)
{
    _exponent.setName("Exponent");
    _exponent.setMin(0.5);
    _exponent.setMax(4.0);
    _AddUIParameter(_exponent);
    
    _multiplier.setName("Multiplier");
    _multiplier.setMin(0.5);
    _multiplier.setMax(4.0);
    _AddUIParameter(_multiplier);
    
    _scale.setName("Scale");
    _scale.setMin(0.01);
    _scale.setMax(1.0);
    _AddUIParameter(_scale);
    
    _hueOffset.setName("Hue Offset");
    _hueOffset.setMin(0.0);
    _hueOffset.setMax(1.0);
    _AddUIParameter(_hueOffset);
    
    _hueShift.setName("Hue Shift");
    _hueShift.setMin(0.0);
    _hueShift.setMax(2.0);
    _AddUIParameter(_hueShift);
    
    _minSaturation.setName("Min Saturation");
    _minSaturation.setMin(0.0);
    _minSaturation.setMax(1.0);
    _AddUIParameter(_minSaturation);
    
    _maxSaturation.setName("Max Saturation");
    _maxSaturation.setMin(0.0);
    _maxSaturation.setMax(1.0);
    _AddUIParameter(_maxSaturation);
    
    _cutoffValue.setName("Cutoff Value");
    _cutoffValue.setMin(0.0);
    _cutoffValue.setMax(1.0);
    _AddUIParameter(_cutoffValue);
    
    _vibrate.setName("Vibrate");
    _AddUIParameter(_vibrate);
    
    _cycleHues.setName("Cycle Hues");
    _AddUIParameter(_cycleHues);
}

SrWhompPattern::~SrWhompPattern()
{
    
}

void
SrWhompPattern::_Update()
{
    SrScrollingPattern::_Update();
    
    if (_vibrate) {
        _flipLeftRight = not _flipLeftRight;
    }
}

bool
SrWhompPattern::_FlipLeftRight() const
{
    return _flipLeftRight;
}

void
SrWhompPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    const SrModel * model = GetModel();
    int numLights = model->GetLightsPerGate();
    
    float low = GetAudio()->GetLows()[0];
    
    low = pow(low * _multiplier, _exponent);
    
    if (low < _cutoffValue) {
        return;
    }
    
    float heightInLights = numLights * low * _scale;
    
    float hue = _hueOffset +
        _cycleHues * GetGlobalParameters()->GetVerySlowCycle() +
        GetGlobalParameters()->GetDial1() +
        (3.0 - low) * _hueShift;
    
    hue = SrUtil_ClampCycle(0, 1, hue);
    
    for (size_t i = 0; i < heightInLights; i++) {
        
        float t = i / heightInLights;
        
        ofFloatColor c;
        float saturation = SrUtil_Lerp(_minSaturation, _maxSaturation, t);
        c.setHsb(hue, saturation, 1.0);
    
        int idx1 = i;
        int idx2 = numLights - 1 - i;
        
        if (not _vibrate or _flipLeftRight) {
            (*buffer)[idx1] = c;
        }
        
        if (not _vibrate or not _flipLeftRight) {
            (*buffer)[idx2] = c;
        }
    }
}
