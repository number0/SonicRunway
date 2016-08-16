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
    _hueShiftParam(0.0),
    _mirror(true),
    _scale(0.7),
    _blendBuckets(true),
    _spinSlowly(false),
    _cycleHues(true)
{
    _hueShiftParam.setName("Hue");
    _hueShiftParam.setMin(0.0);
    _hueShiftParam.setMax(1.0);
    _AddUIParameter(_hueShiftParam);
    
    _mirror.setName("Mirror");
    _AddUIParameter(_mirror);
    
    _scale.setName("Scale");
    _scale.setMin(0.01);
    _scale.setMax(1.5);
    _AddUIParameter(_scale);
    
    _blendBuckets.setName("Blend Buckets");
    _AddUIParameter(_blendBuckets);
    
    _spinSlowly.setName("Spin Slowly");
    _AddUIParameter(_spinSlowly);
    
    _cycleHues.setName("Cycle Hues");
    _AddUIParameter(_cycleHues);
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
        (bool) _cycleHues * GetGlobalParameters()->GetPhraseCycle() +
        GetGlobalParameters()->GetDial1();
    
    for(int fftIdx = 0; fftIdx < ffts.size(); fftIdx++) {
        float baseAngle = (bool) _spinSlowly *
            GetGlobalParameters()->GetPhraseCycle();
        
        float angleSpan = (float) _scale;
        
        float beginT = (float) fftIdx / ffts.size();
        float endT = (float) (fftIdx + 1) / ffts.size();
        
        float beginAngle = baseAngle + angleSpan * beginT;
        float endAngle = baseAngle + angleSpan * endT;
        
        int beginLight = beginAngle * numLights;
        int endLight = endAngle * numLights;
        
        float fftValue = ffts[fftIdx];

        ofFloatColor c;
        float baseColor = 0.15 + hueShift;
        float hue = baseColor - 0.4 * (1.0 - fftValue);
        
        hue = SrUtil_ClampCycle(0.0, 1.0, hue);
        
        c.setHsb(hue, 1.0, fftValue * 2.0);
        
        for(int lightIdx = beginLight; lightIdx < endLight; lightIdx++) {
           
            int idx = lightIdx % numLights;
            
            (*buffer)[idx] += c;
            if ((bool) _mirror) {
                (*buffer)[numLights - 1 - idx] += c;
            }
        }
        
    }
    
    /*
    
    for(int i = 0; i < numLights; i++) {
        
        float t = (float) i / (numLights - 1);
        printf("t %f\n", t);
        
        t *= (float) 1.0 / _scale;
        
        if (t > 1.0) {
            continue;
        }
        
        float value = t;
        
        float fBand = t * (ffts.size() - 1);
        int lowerBand = floor(fBand);
        int upperBand = ceil(fBand);
        
        printf("fBand %f\n", fBand);
        printf("lowerBand %d\n", lowerBand);
        float hue = fmod((lowerBand / 5.0), 1);
        printf("hue %f\n", hue);
        
        ofFloatColor c;
        c.setHsb(hue, 1.0, value);
        
        (*buffer)[i] += c;
     */
        
        /*
        
        
        // Reduce t since we're mirroring below..
        t *= (float) _scale;
        
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
         */
    
    /*
    }
     */
}
