//
//  RainbowPattern.cpp
//  SonicRunway
//
//  Created by Paul Roales on 7/16/16.
//
//

#include "RainbowPattern.hpp"
#include "Audio.hpp"
#include "GlobalParameters.hpp"

SrRainbowPattern::SrRainbowPattern(const std::string & name,
                             SrModel * model, SrAudio * audio,
                                   SrGlobalParameters * globalParameters) :
SrScrollingPattern(name, model, audio, globalParameters),
_manualControlParam(false),
_hueBiasParam(0.88),
_saturationParam(0.84),
_brightnessParam(0.77),
_hueBiasChange(0.001),
_saturationChange(-0.00075),
_brightnessChange(0.0015)
{
    _hueBiasParam.setName("Hue Bias");
    _hueBiasParam.setMin(0.0);
    _hueBiasParam.setMax(1.0);
    _AddUIParameter(_hueBiasParam);
    
    _saturationParam.setName("Saturation");
    _saturationParam.setMin(0.0);
    _saturationParam.setMax(1.0);
    _AddUIParameter(_saturationParam);
    
    _brightnessParam.setName("Brightness");
    _brightnessParam.setMin(0.0);
    _brightnessParam.setMax(1.0);
    _AddUIParameter(_brightnessParam);
}

SrRainbowPattern::~SrRainbowPattern()
{
    // Uses GlobalParams like so:
    // Dial1 - Hue Bias
    // Slider1 - Saturation
    // Slider2 - Brightness
}

void
SrRainbowPattern::_Update()
{
    SrScrollingPattern::_Update();

    if (IsOnAtAnyGate()) {
        SrGlobalParameters * globals = GetGlobalParameters();
        
        if (globals->WasRecentManualInput()) {
            _hueBiasParam = GetGlobalParameters()->GetDial1();
            _saturationParam = GetGlobalParameters()->GetSlider1();
            _brightnessParam = GetGlobalParameters()->GetSlider2();
        } else {
            _hueBiasParam += _hueBiasChange;
            if (_hueBiasParam >= 0.9 || _hueBiasParam <= -0.4) {
                _hueBiasChange = -_hueBiasChange;
            }
            
            _saturationParam += _saturationChange;
            if (_saturationParam >= 0.9 || _saturationParam <= 0.0) {
                _saturationChange = -_saturationChange;
            }
            
            _brightnessParam += _brightnessChange;
            if (_brightnessParam >= 0.9 || _brightnessParam <= 0.0) {
                _brightnessChange = -_brightnessChange;
            }
        }
    }
}

void
SrRainbowPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    float hue = GetAudio()->GetCalibratedFftSum();
    float hueBias = (float) _hueBiasParam;
    float saturation = (float) _saturationParam;
    float brightness = (float) _brightnessParam;
    
    float adjustedHue = hue + hueBias;
    
    ofFloatColor color;
    color.setHsb(adjustedHue, saturation, brightness);
    for(int i = 0; i < buffer->size(); i++) {
        (*buffer)[i] = color;
    }
}