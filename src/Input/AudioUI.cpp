//
//  AudioUI.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 1/10/16.
//
//  Based on the example by Paul Brossier included in ofxAubio:
//  https://github.com/aubio/ofxAubio
//

#include "AudioUI.hpp"

#include "Audio.hpp"

#include "ofMain.h"

SrAudioUI::SrAudioUI(SrAudio * audio) :
    SrUiMixin("Audio"),
    _audio(audio),
    _playDelayedAudioParam(false),
    _resetDownbeatParam(false),
    _resetMeasureParam(false)
{
    _audio = audio;
    
    _playDelayedAudioParam.setName("Play Delayed Audio");
    _playDelayedAudioParam.addListener(
        this, &This::_OnPlayDelayedAudioButtonPressed);
    _AddUIParameter(_playDelayedAudioParam);
    
    _resetDownbeatParam.setName("Tap Downbeat");
    _AddUIParameter(_resetDownbeatParam);
    _resetMeasureParam.setName("Tap Measure 1");
    _AddUIParameter(_resetMeasureParam);
   
    _beatGui.setup("Beat");
    _beatGui.add(_bpmSlider.setup("bpm", 0, 0, 250));
    _beatGui.add(_beatIndexSlider.setup("index", 0, 0, 4));
    _beatGui.add(_measureIndexSlider.setup("index", 0, 0, 8));
    _AddUI(&_beatGui);
    
    _onsetGui.setup("Onset");
    _onsetGui.add(_gotOnsetSlider.setup("onset", 0, 0, 1.0));
    _onsetGui.add(_onsetThresholdSlider.setup("threshold", 0, 0, 2));
    _onsetGui.add(_onsetNoveltySlider.setup(
                  "onset novelty", 0, 0, 10000));
    _onsetGui.add(_onsetThresholdedNoveltySlider.setup(
                  "thr. novelty", 0, -1000, 1000));
    _AddUI(&_onsetGui);
    
    // Set a default value for the slider (assignment op overloaded)
    _onsetThresholdSlider =
        _audio->GetLowOnsetHistory().GetThreshold()[0];
    
    /*
    _bandsGui.setup("SrAudioMelBends");
    for (int i = 0; i < 40; i++) {
        _bandPlot.addVertex(50 + i * 650 / 40.,
                            240 - 100 *
                            _audio->GetCurrentFftValues()[i]);
    }
     */
}

SrAudioUI::~SrAudioUI()
{
    
}

void
SrAudioUI::Update()
{
    const SrOnsetHistory & onset =
        _audio->GetLowOnsetHistory();
    
    bool isRecentOnset = (onset.GetSecondsSinceLastEvent()[0] < 0.05);
    
    if ((bool) _resetDownbeatParam) {
        _audio->ResetDownbeat();
        _resetDownbeatParam = false;
    }
    
    if ((bool) _resetMeasureParam) {
        _audio->ResetMeasure();
        _resetMeasureParam = false;
    }
    
    _gotOnsetSlider = (float) isRecentOnset;
    _onsetNoveltySlider = onset.GetNovelty()[0];
    _onsetThresholdedNoveltySlider = onset.GetThresholdedNovelty()[0];
    _bpmSlider = _audio->GetBeatHistory().GetBpm()[0];
    _beatIndexSlider = _audio->GetBeatHistory().GetBeatIndex()[0];
    _measureIndexSlider = _audio->GetBeatHistory().GetMeasureIndex()[0];
    
    // XXX should set onset threshold here from slider..
}

void
SrAudioUI::_OnPlayDelayedAudioButtonPressed(bool &on)
{
    _audio->SetOutputAudioDelayed(on);
}


/*
void
SrAudioUI::Draw()
{
    _beatGui.draw();
    _onsetGui.draw();
    
    ofPushStyle();
    
    ofSetColor(ofColor::orange);
    ofSetLineWidth(3.0);
    _bandsGui.draw();
    for (int i = 0; i < _bandPlot.size(); i++) {
        _bandPlot[i].y = 240 - 100 * _audio->GetCurrentFftValues()[i];
    }
    _bandPlot.draw();
    
    ofPopStyle();
}
*/