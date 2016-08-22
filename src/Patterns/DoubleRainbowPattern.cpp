//
//  DoubleRainbowPattern.cpp
//  SonicRunway
//
//  Created by jdn on 7/9/16.
//
//

#include "DoubleRainbowPattern.hpp"
#include "Audio.hpp"
#include "BeatHistory.hpp"
#include "GlobalParameters.hpp"
#include <math.h>

SrDoubleRainbowPattern::SrDoubleRainbowPattern(const std::string & name,
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
    _hueParam.setName("Hue");
    _hueParam.setMin(0.0);
    _hueParam.setMax(1.0);
    _AddUIParameter(_hueParam);
    
    _decayTimeParam.setName("DecayTime");
    _decayTimeParam.setMin(0.0);
    _decayTimeParam.setMax(2.0);
    _AddUIParameter(_decayTimeParam);
    
    _spinSpeedParam.setName("SpinSpeed");
    _spinSpeedParam.setMin(-3.0);
    _spinSpeedParam.setMax(3.0);
    _AddUIParameter(_spinSpeedParam);
    
    _hueSpeedParam.setName("HueSpeed");
    _hueSpeedParam.setMin(-0.1);
    _hueSpeedParam.setMax(0.1);
    //_AddUIParameter(_hueSpeedParam);
    
    _doubleRainbowsParam.setName("DoubleRainbows");
    _doubleRainbowsParam.setMin(-5.0);
    _doubleRainbowsParam.setMax(5.0);
    _AddUIParameter(_doubleRainbowsParam);

    _trippelRainbowsParam.setName("TrippelRainbows");
    _trippelRainbowsParam.setMin(-5.0);
    _trippelRainbowsParam.setMax(5.0);
    _AddUIParameter(_trippelRainbowsParam);

    _filterParam.setName("MusicFilterTime");
    _filterParam.setMin(0.0);
    _filterParam.setMax(1.0);
    _AddUIParameter(_filterParam);

}

SrDoubleRainbowPattern::~SrDoubleRainbowPattern()
{
}

void
SrDoubleRainbowPattern::_Update()
{
    SrScrollingPattern::_Update();
    
    _spinOffset += (float) _spinSpeedParam;
    _hueOffset += (float) _hueSpeedParam;
}

std::string
SrDoubleRainbowPattern::GetGlobalParameterLabel(const std::string & name) const
{
    if (name == "Dial1") {
        return "Double";
    } else if (name == "Dial2") {
        return "Trippel";
    } else if (name == "Slider1") {
        return "Length";
    } else if (name == "Slider2") {
        return "Smoothness";
    }
    
    return std::string();
}


void
SrDoubleRainbowPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    SrGlobalParameters * globals = GetGlobalParameters();

    //    float elapsedTime = ofGetElapsedTimef();
//    int beatIndex = GetAudio()->GetBeatHistory().GetBeatIndex()[0];

    // grab parameter values
    float onsetDecayTime = (float) _decayTimeParam;
    float hue = (float) _hueParam;
    float doubleRainbows = (float) _doubleRainbowsParam;
    float trippelRainbows = (float) _trippelRainbowsParam;
    float filterTime = (float) _filterParam;

    // grab RMS audio band values
    float low = GetAudio()->GetLows().ComputeValue(0.0f, filterTime);
    float mid = GetAudio()->GetMids()[0];
    float high = GetAudio()->GetHighs().ComputeValue(0.0f, filterTime);
    
    
    float timeSinceBeat =
        GetAudio()->GetBeatHistory().GetSecondsSinceLastEvent()[0];
    
    bool beatless = false;
    if(timeSinceBeat > 5.0f) { // fallback mode
        beatless = true;
    }
    
    float bpm = GetAudio()->GetBeatHistory().GetBpm()[0];
    float quarterNoteSeconds = 60.0/bpm;
    
    // react to parameter inputs
    // Uses GlobalParams like so:
    // Dial1 - Double Rainbow +/- 5
    // Dial2 - Trippel Rainbow +/- 5
    // Slider1 - Pulse Length
    // Slider2 - Music Filter Time
    
    if (globals->WasRecentManualInput()) {
        doubleRainbows = ofMap(GetGlobalParameters()->GetDial1(), 0.0f, 1.0f, -5.0f, 5.0f);
        trippelRainbows = ofMap(GetGlobalParameters()->GetDial2(), 0.0f, 1.0f, -5.0f, 5.0f);
        onsetDecayTime = GetGlobalParameters()->GetSlider1();
        filterTime = ofMap(GetGlobalParameters()->GetSlider2(), 0.0f, 1.0f, 0.05f, 1.0f);
    }
    
    float amplitude = GetAudio()->GetCalibratedFftSum();
    float brightness = pow(amplitude, 0.4f); // scale baseline brightness by amplitude
        
    // map the length of the rainbow bands based on the amplitude
    onsetDecayTime = ofMap(amplitude, 0.0f, 1.0f, 0.2f*onsetDecayTime, 1.2f*onsetDecayTime);
    
    float rainbowTheta = std::min(1.0f, timeSinceBeat / (quarterNoteSeconds*onsetDecayTime));
    
    
    
    // also scale brightness based on beats, if present
    if ( rainbowTheta == 1.0f) {
        brightness = 0.0f;
    }

    // fallback for beatless mode
    if (beatless) {
        rainbowTheta = globals->GetSlowCycle();
        brightness = fabs(sin(globals->GetPhraseCycle()*10.0f));
        low = globals->GetTwoBeatCycle();
        high = globals->GetMeasureCycle();
        doubleRainbows *= globals->GetSlowCycle();
        trippelRainbows *= globals->GetVerySlowCycle();
    }
    
    for(int i = 0; i < buffer->size(); i++) {
        
        float gatePhase = float(i) / buffer->size();
        float gateHue = rainbowTheta;
        
        // rotate gate phase slowly over time so things don't stay lopsided
//        gatePhase += elapsedTime/10.0f;
//        gatePhase = gatePhase - (long)gatePhase;
    
        // NOTE: keep for now, this is a cool rainbow arc look
        /*
        if (doubleRainbows) {
            gateHue *= gatePhase;
        }
        */

        gateHue += doubleRainbows*gatePhase*low;
        gateHue *= 1.0f - (trippelRainbows*gatePhase*high);
        
        if (gateHue > 1.0f) {
            gateHue = gateHue - (long)gateHue;
        } else if (gateHue < 0.0f) {
            gateHue = (long)gateHue - gateHue;
        }
        /*
        while (gateHue > 1.0f) {
            gateHue -= 1.0f; // XXX code fail
        }
        while (gateHue < 0.0f) {
            gateHue += 1.0f; // XXX code fail
        }
        */
      
        // limit hue to 0.8 so we don't wrap over into red after purple
        ofFloatColor thisC;
        /*
        if (beatIndex % 2 == 1) {
            gateHue = 1.0f - gateHue;
        }
        */
        thisC.setHsb(gateHue*0.8f, 1.0, brightness);
        (*buffer)[i] = thisC;
    }
    
    return;
}
