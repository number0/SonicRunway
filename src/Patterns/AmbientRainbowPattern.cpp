//
//  AmbientRainbowPattern.cpp
//  SonicRunway
//
//  Created by jdn on 7/9/16.
//
//

#include "AmbientRainbowPattern.hpp"
#include "Audio.hpp"
#include "BeatHistory.hpp"
#include "GlobalParameters.hpp"
#include <math.h>

SrAmbientRainbowPattern::SrAmbientRainbowPattern(const std::string & name,
                                   SrModel * model, SrAudio * audio,
                                   SrGlobalParameters * globalParameters) :
    SrScrollingPattern(name, model, audio, globalParameters),
    _hueParam(1.0),
    _decayTimeParam(0.9),
    _spinSpeedParam(0.25),
    _hueSpeedParam(0.0),
    _doubleRainbowsParam(2.55),
    _trippelRainbowsParam(-2.19),
    _spinOffset(0.2),
    _hueOffset(0.0),
    _filterParam(0.20)
{
    /*
    _hueParam.setName("Hue");
    _hueParam.setMin(0.0);
    _hueParam.setMax(1.0);
    _AddUIParameter(_hueParam);
     */
     
    /*
    _decayTimeParam.setName("DecayTime");
    _decayTimeParam.setMin(0.0);
    _decayTimeParam.setMax(2.0);
    _AddUIParameter(_decayTimeParam);
    */
     
    _spinSpeedParam.setName("SpinSpeed");
    _spinSpeedParam.setMin(-3.0);
    _spinSpeedParam.setMax(3.0);
    _AddUIParameter(_spinSpeedParam);
    
    _hueSpeedParam.setName("HueSpeed");
    _hueSpeedParam.setMin(-0.1);
    _hueSpeedParam.setMax(0.1);
    _AddUIParameter(_hueSpeedParam);
    
    _doubleRainbowsParam.setName("DoubleRainbows");
    _doubleRainbowsParam.setMin(-5.0);
    _doubleRainbowsParam.setMax(5.0);
    _AddUIParameter(_doubleRainbowsParam);

    _trippelRainbowsParam.setName("TrippelRainbows");
    _trippelRainbowsParam.setMin(-5.0);
    _trippelRainbowsParam.setMax(5.0);
    _AddUIParameter(_trippelRainbowsParam);

    /*
    _filterParam.setName("MusicFilterTime");
    _filterParam.setMin(0.0);
    _filterParam.setMax(1.0);
    _AddUIParameter(_filterParam);
     */
}

SrAmbientRainbowPattern::~SrAmbientRainbowPattern()
{
}

void
SrAmbientRainbowPattern::_Update()
{
    SrScrollingPattern::_Update();

    SrGlobalParameters * globals = GetGlobalParameters();
    
    if (globals->WasRecentManualInput()) {
        _hueOffset += (float) ofMap(GetGlobalParameters()->GetSlider1(), 0.0f, 1.0f, -0.2f, 0.2f);
        _spinOffset += (float) ofMap(GetGlobalParameters()->GetSlider2(), 0.0f, 1.0f, -3.0f, 3.0f);
    } else {
        _spinOffset += (float) _spinSpeedParam;
        _hueOffset += (float) _hueSpeedParam;
    }
}

std::string
SrAmbientRainbowPattern::GetGlobalParameterLabel(const std::string & name) const
{
    if (name == "Dial1") {
        return "Double";
    } else if (name == "Dial2") {
        return "Trippel";
    } else if (name == "Slider1") {
        return "HueSpeed";
    } else if (name == "Slider2") {
        return "SpinSpeed";
    }
    
    return std::string();
}


void
SrAmbientRainbowPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    SrGlobalParameters * globals = GetGlobalParameters();

    // grab parameter values
    float hue = (float) _hueParam;
    float doubleRainbows = (float) _doubleRainbowsParam;
    float trippelRainbows = (float) _trippelRainbowsParam;
    
    // react to parameter inputs
    // Uses GlobalParams like so:
    // Dial1 - Double Rainbow +/- 5
    // Dial2 - Trippel Rainbow +/- 5
    // Slider1 - Hue Speed
    // Slider2 - Spin Speed
    
    if (globals->WasRecentManualInput()) {
        doubleRainbows = ofMap(GetGlobalParameters()->GetDial1(), 0.0f, 1.0f, -5.0f, 5.0f);
        trippelRainbows = ofMap(GetGlobalParameters()->GetDial2(), 0.0f, 1.0f, -5.0f, 5.0f);
    }
    
    float rainbowTheta = globals->GetSlowCycle();
    float brightness = fabs(sin(globals->GetPhraseCycle()*10.0f));
    float low = globals->GetTwoBeatCycle();
    float high = globals->GetMeasureCycle();
    doubleRainbows *= globals->GetSlowCycle();
    trippelRainbows *= globals->GetVerySlowCycle();
    
    for(int i = 0; i < buffer->size(); i++) {
        
        float gatePhase = float(i) / buffer->size();
        float gateHue = rainbowTheta;
        
        gateHue += doubleRainbows*gatePhase*low; //*_spinOffset;
        gateHue *= 1.0f - (trippelRainbows*gatePhase*high); // *_spinOffset);
        
        //gateHue += _hueOffset;
        
        if (gateHue > 1.0f) {
            gateHue = gateHue - (long)gateHue;
        } else if (gateHue < 0.0f) {
            gateHue = (long)gateHue - gateHue;
        }

        // limit hue to 0.8 so we don't wrap over into red after purple
        ofFloatColor thisC;
        thisC.setHsb(gateHue*0.8f, 1.0, brightness);
        (*buffer)[i] = thisC;
    }
    return;
}

bool
SrAmbientRainbowPattern::IsAudioReactive() const
{
    return false;
}

