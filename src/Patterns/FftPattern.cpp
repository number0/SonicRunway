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
    _hueShiftParam(0.0)
{
    _hueShiftParam.setName("Hue");
    _hueShiftParam.setMin(0.0);
    _hueShiftParam.setMax(1.0);
    
    _AddUIParameter(_hueShiftParam);
}

SrFftPattern::~SrFftPattern()
{
    
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
    
    float hueShift = (float) _hueShiftParam +
        GetGlobalParameters()->GetPhraseCycle() +
        GetGlobalParameters()->GetDial1();
    
    for(int i = 0; i < numLights; i++) {
        float t = (float) i / numLights;
        
        // Reduce t since we're mirroring below..
        t *= 0.7;
        
        int band = t * ffts.size() - 1;
        if (band >= ffts.size()) {
            band = ffts.size() - 1;
        }
        
        float fftValue = ffts[band];

        ofFloatColor c;
        float baseColor = 0.15 + hueShift;
        float hue = baseColor - 0.4 * (1.0 - fftValue);
        
        hue = SrUtil_ClampCycle(0.0, 1.0, hue);
        
        c.setHsb(hue, 1.0, fftValue * 1.5);
        
        (*buffer)[i] += c;
        
        // mirror image
        (*buffer)[numLights - 1 - i] += c;
    }
}
