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

SrDoubleRainbowPattern::SrDoubleRainbowPattern(const std::string & name,
                                   SrModel * model, SrAudio * audio,
                                   SrGlobalParameters * globalParameters) :
    SrScrollingPattern(name, model, audio, globalParameters),
    _hueParam(1.0),
    _decayTimeParam(0.5),
    _spinSpeedParam(0.25),
    _hueSpeedParam(0.0),
    _doubleRainbowsParam(5.0),
    _trippelRainbowsParam(-5.0),
    _spinOffset(0.2),
    _hueOffset(0.0),
    _filterParam(0.40)
{
    _hueParam.setName("Hue");
    _hueParam.setMin(0.0);
    _hueParam.setMax(1.0);
    _AddUIParameter(_hueParam);
    
    _decayTimeParam.setName("DecayTime");
    _decayTimeParam.setMin(0.0);
    _decayTimeParam.setMax(1.0);
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

void
SrDoubleRainbowPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
//    float elapsedTime = ofGetElapsedTimef();
//    int beatIndex = GetAudio()->GetBeatHistory().GetBeatIndex()[0];
    
    float filterTime = (float) _filterParam;
    float low = GetAudio()->GetLows().ComputeValue(0.0f, filterTime);
    float mid = GetAudio()->GetMids()[0];
    float high = GetAudio()->GetHighs().ComputeValue(0.0f, filterTime);
    
    //float hue = fmod(fabs(_hueParam + _hueOffset), 1.0);
    float hue = (float) _hueParam;
    
    float timeSinceBeat =
        GetAudio()->GetBeatHistory().GetSecondsSinceLastEvent()[0];
    
    float bpm = GetAudio()->GetBeatHistory().GetBpm()[0];
    float quarterNoteSeconds = 60.0/bpm;
    
    float onsetDecayTime = (float) _decayTimeParam;
    
    float rainbowTheta = std::min(1.0f, timeSinceBeat / (quarterNoteSeconds*onsetDecayTime));
    
    float doubleRainbows = (float) _doubleRainbowsParam;
    float trippelRainbows = (float) _trippelRainbowsParam;
    
    for(int i = 0; i < buffer->size(); i++) {
        float brightness = 1.0f;
        if ( rainbowTheta == 1.0f) {
            brightness = 0.0f;
        }
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

    
    /*
    float onsetAmount = 1.0 - timeSinceBeat / onsetDecayTime;
    
    if (onsetAmount < 0.0) {
        return;
    }
    
    float onsetMult = onsetAmount;
    
    // Force full onsetAmount if we're less than one full gate behind.
    if (timeSinceBeat < GetModel()->ComputeDelayPerGate()) {
        onsetMult = 1.0;
    }
    
    onsetMult *= onsetMult;
    
    ofFloatColor c;
    c.setHsb(hue, 1.0, onsetMult); // XXX never using this color below?
    for(int i = 0; i < buffer->size(); i++) {
        
        int lightsPerTrailer = buffer->size() / 10;
        
        // XXX this is bad b/c it isn't normalized to number of lights.
        int iNumerator = i + _spinOffset;
        
        float trailDiminish = (float) (iNumerator % lightsPerTrailer) / lightsPerTrailer;
        trailDiminish = onsetAmount * 1.0 + (1.0 - onsetAmount) * trailDiminish;
        trailDiminish *= trailDiminish;
       
        float thisMult = onsetMult * trailDiminish;
        float thisHue = hue; // + 0.3 * (1.0 - thisMult);
        if (thisHue < 0.0) {
            thisHue += 1.0;
        }
        thisHue = fmod(thisHue, 0.8);
       
        ofFloatColor thisC;
        thisC.setHsb(hue*onsetAmount, 1.0, 1.0); //thisMult);
        
        (*buffer)[i] = thisC;
    }
    */
}
