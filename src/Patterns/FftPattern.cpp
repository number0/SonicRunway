//
//  FftPattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 3/4/16.
//
//

#include "FftPattern.hpp"
#include "Audio.hpp"
#include "Model.hpp"
#include "GlobalParameters.hpp"
#include "Util.hpp"

SrFftPattern::SrFftPattern(const std::string & name,
                           SrModel * model, SrAudio * audio,
                           SrGlobalParameters * globalParameters) :
    SrScrollingPattern(name, model, audio, globalParameters),
    _exponent(2.0),
    _multiplier(2.0),
    _spreadExponent(0.37),
    _hueOffset(0.0),
    _hueShift(0.3),
    _mirror(true),
    _scale(1.0),
    _scaleRegions(true),
    _spinSlowly(true),
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
    
    _spreadExponent.setName("Spread Exponent");
    _spreadExponent.setMin(0.1);
    _spreadExponent.setMax(1.0);
    _AddUIParameter(_spreadExponent);
    
    _hueOffset.setName("Hue Offset");
    _hueOffset.setMin(0.0);
    _hueOffset.setMax(1.0);
    _AddUIParameter(_hueOffset);
    
    _hueShift.setName("Hue Shift");
    _hueShift.setMin(0.0);
    _hueShift.setMax(2.0);
    _AddUIParameter(_hueShift);
    
    _mirror.setName("Mirror");
    _AddUIParameter(_mirror);
    
    _scale.setName("Scale");
    _scale.setMin(0.01);
    _scale.setMax(1.5);
    _AddUIParameter(_scale);
    
    _scaleRegions.setName("Scale Regions");
    _AddUIParameter(_scaleRegions);
    
    _spinSlowly.setName("Spin Slowly");
    _AddUIParameter(_spinSlowly);
    
    _cycleHues.setName("Cycle Hues");
    _AddUIParameter(_cycleHues);
}

SrFftPattern::~SrFftPattern()
{
    
}

ofFloatColor
SrFftPattern::_ComputeColor(float fftValue, float hueOffset) const
{
    // Cut-off low end.
    if (fftValue < 0.1) {
        return ofFloatColor::black;
    }
    
    ofFloatColor c;
    
    float baseColor = 0.15 + hueOffset;
    float hue = baseColor - (float) _hueShift * (1.0 - fftValue);
    
    hue = SrUtil_ClampCycle(0.0, 1.0, hue);
    
    float value = (bool) _scaleRegions ?
                        1.0 : fftValue * 2.0;
    c.setHsb(hue, 1.0, value);
    
    return c;
}

float
SrFftPattern::_ComputeValue(float value) const
{
    return pow(value * (float) _multiplier, (float) _exponent);
}

void
SrFftPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    const SrModel * model = GetModel();
    int numLights = model->GetLightsPerGate();
    
    std::vector<float> ffts = GetAudio()->GetCurrentSmoothFftValues();
    
    if (ffts.empty()) {
        return;
    }
    
    float hueOffset = (float) _hueOffset +
        (bool) _cycleHues * GetGlobalParameters()->GetSlowCycle() +
        GetGlobalParameters()->GetDial1();
    
    for(int fftIdx = 0; fftIdx < ffts.size(); fftIdx++) {
        float baseAngle = (bool) _spinSlowly *
            GetGlobalParameters()->GetPhraseCycle();
        
        float angleSpan = (float) _scale;
        
        float beginT = (float) fftIdx / ffts.size();
        float endT = (float) (fftIdx + 1) / ffts.size();
        
        beginT = pow(beginT, (float) _spreadExponent);
        endT = pow(endT, (float) _spreadExponent);
        
        float beginAngle = baseAngle + angleSpan * beginT;
        float endAngle = baseAngle + angleSpan * endT;
        
        int beginLight = beginAngle * numLights;
        int endLight = endAngle * numLights;
        
        float fftValue = _ComputeValue(ffts[fftIdx]);
        int nextIdx = fftIdx + 1;
        if (nextIdx >= ffts.size()) {
            nextIdx = fftIdx;
        }
        
        float nextFftValue = _ComputeValue(ffts[nextIdx]);
        
        ofFloatColor c0 = _ComputeColor(fftValue, hueOffset);
        ofFloatColor c1 = _ComputeColor(nextFftValue, hueOffset);

        for(int lightIdx = beginLight; lightIdx < endLight; lightIdx++) {
            
            float t = (float) (lightIdx - beginLight) / (endLight - beginLight);
            
            if ((bool) _scaleRegions and t > fftValue) {
                continue;
            }
           
            int idx = lightIdx % numLights;
            
            ofFloatColor c = SrUtil_LerpColors(c0, c1, t);
            
            (*buffer)[idx] += c;
            if ((bool) _mirror) {
                (*buffer)[numLights - 1 - idx] += c;
            }
        }
        
    }
}
