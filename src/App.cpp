//
//  App.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 1/13/16.
//
//

#include "App.hpp"
#include "ofApp.h"

#include "BeatPattern.hpp"
#include "FftPattern.hpp"
#include "ExamplePattern.hpp"
#include "PhrasePattern.hpp"
#include "StripesPattern.hpp"
#include "StarPattern.hpp"
#include "RainbowPattern.hpp"
#include "ArcPattern.hpp"
#include "PhasorPattern.hpp"
#include "TrailsPattern.hpp"
#include "TriggerPattern.hpp"
#include "VideoPattern.hpp"
#include "AnimPattern.hpp"
#include "DiagnosticPattern.hpp"
#include "RmsPattern.hpp"
#include "BigTrailsPattern.hpp"

#include "Switcher.hpp"

extern "C" void CGSSetDebugOptions(int);
extern "C" void CGSDeferredUpdates(int);

static const size_t PATTERNS_PER_COLUMN = 10;

SrApp::SrApp() :
    _model(),
    _audio("Audio", &_model),
    _artnet("Artnet", &_model),
    _previs(&_model, &_audio),
    _switcher("Switcher", &_model, &_audio),
    _uiColumnWidth(220),
    _uiMargin(10)
{
    ofSetFrameRate(_model.ComputeFramesPerSecond());
    
    // Disable vertical sync so FPS can be greater than 60.
    ofSetVerticalSync(false);
    CGSSetDebugOptions(0x08000000);
    CGSDeferredUpdates(true);
    
    _globalPanel.setup("Global");
    _globalPanel.setPosition(_uiMargin,_uiMargin);
    
    _globalPanel.add(_previs.GetUiPanel());
    _globalPanel.add(_artnet.GetUiPanel());
    _globalPanel.add(_audio.GetUiPanel());
    _globalPanel.add(_switcher.GetUiPanel());
    
    _patternsParameterGroup.setName("Patterns");
    _model.GetParameterGroup().add(_patternsParameterGroup);
    _model.GetParameterGroup().add(_audio.GetParameterGroup());
    _model.GetParameterGroup().add(_previs.GetParameterGroup());
    
    SrExamplePattern *examplePattern =
        new SrExamplePattern("Example", &_model, &_audio);
    _AddPattern(examplePattern);
    
    SrBeatPattern * beatPattern =
        new SrBeatPattern("Beat", &_model, &_audio);
    _AddPattern(beatPattern);
    
    SrFftPattern *fftPattern =
        new SrFftPattern("Fft", &_model, &_audio);
    _AddPattern(fftPattern);
    
    SrStripesPattern *stripesPattern =
        new SrStripesPattern("Stripes", &_model, &_audio);
    _AddPattern(stripesPattern);
    
    SrStarPattern *starPattern =
        new SrStarPattern("Star", &_model, &_audio);
    _AddPattern(starPattern);
    
    SrRainbowPattern *rainbowPattern =
    new SrRainbowPattern("Rainbow", &_model, &_audio);
    _AddPattern(rainbowPattern);
    
    SrArcPattern *arcPattern =
    new SrArcPattern("Arc", &_model, &_audio);
    _AddPattern(arcPattern);
    
    SrPhasorPattern *phasorPattern =
    new SrPhasorPattern("Phasor", &_model, &_audio);
    _AddPattern(phasorPattern);
    
    SrTrailsPattern *trailsPattern =
    new SrTrailsPattern("Trails", &_model, &_audio);
    _AddPattern(trailsPattern);
    
     // Disabled b/c it seems like it might be slow.
     //
    SrVideoPattern *videoPattern =
        new SrVideoPattern("Video", "fireplace2.mov", &_model, &_audio);
    _AddPattern(videoPattern);
    
    SrAnimPattern *animPattern =
        new SrAnimPattern("Anim", "lightning", 82, false, &_model, &_audio);
    _AddPattern(animPattern);
    
    SrTriggerPattern *triggerPattern =
        new SrTriggerPattern("Trigger", &_model, &_audio);
    _AddPattern(triggerPattern);
    
    SrPhrasePattern *phrasePattern =
        new SrPhrasePattern("Phrase", &_model, &_audio);
    _AddPattern(phrasePattern);
    
    SrDiagnosticPattern *diagnosticPattern =
        new SrDiagnosticPattern("Diagnostic", &_model, &_audio);
    _AddPattern(diagnosticPattern);
    
    SrRmsPattern *rmsPattern =
        new SrRmsPattern("RMS", &_model, &_audio);
    _AddPattern(rmsPattern);

    SrBigTrailsPattern *bigTrailsPattern =
    new SrBigTrailsPattern("Big Trails", &_model, &_audio);
    _AddPattern(bigTrailsPattern);
    
    // Enable the patterns we want on by default.
    //triggerPattern->SetEnabled(true);
    //diagnosticPattern->SetEnabled(true);
    fftPattern->SetEnabled(true);
    
    _oscParameterSync.setup(_model.GetParameterGroup(), 8000, "", 9000);
    
    ofSoundStreamSetup(_model.GetNumChannels(), _model.GetNumChannels(),
                       _model.GetSampleRate(), _model.GetBufferSize(), 4);
    
    stripesPattern->GetUiPanel()->minimizeAll();
    
}

SrApp::~SrApp()
{
    // XXX remove panels first?
    // XXX need to figure out clean pattern for deleting...
    
    /*
    for(auto iter = _patterns.begin(); iter != _patterns.end(); iter++) {
        delete *iter;
    }
     */
}

void
SrApp::_AddPattern(SrPattern * pattern)
{
    _patterns.push_back(pattern);
    
    // Figure out which panel it should be in
    // Allocate another patternPanel (column) if we need it
    size_t panelIdx = _patterns.size() / PATTERNS_PER_COLUMN;
    if (panelIdx >= _patternPanels.size()) {
        ofxPanel * newPanel = new ofxPanel();
        newPanel->setup("Patterns");
        newPanel->setPosition(_uiMargin + _uiColumnWidth * (1 + panelIdx), _uiMargin);
        _patternPanels.push_back(newPanel);
    }
    
    // Add the Ui
    _patternPanels[panelIdx]->add(pattern->GetUiPanel());
    
    _patternsParameterGroup.add(pattern->GetParameterGroup());
}

void
SrApp::AudioIn(float * input, int bufferSize, int nChannels)
{
    _audio.AudioIn(input, bufferSize, nChannels);
}

void
SrApp::AudioOut(float *output, int bufferSize, int nChannels)
{
    _previs.AudioOut(output, bufferSize, nChannels);
}

void
SrApp::Update()
{
    _oscParameterSync.update();
    
    _switcher.Update();
    
    for(auto iter = _patterns.begin(); iter != _patterns.end(); iter++) {
        SrPattern *pattern = *iter;
        pattern->Update();
    }
    
    _audio.UpdateUI();
    _previs.Update();
    
    std::string fpsStr = "frame rate: "
            + ofToString(ofGetTargetFrameRate(), 2)
            + " / " + ofToString(ofGetFrameRate(), 2);
    ofSetWindowTitle(fpsStr);
    
    
}

void
SrApp::Draw()
{
    _model.Clear();
    _model.BeginDrawing();
    
    for(auto iter = _patterns.begin(); iter != _patterns.end(); iter++) {
        SrPattern *pattern = *iter;
        pattern->Draw();
    }
    
    _model.EndDrawing();
    
    ofBackground(40,40,40);
    
    int previsXCoord = _uiMargin + _uiColumnWidth * (_patternPanels.size() + 1);
    _model.RenderFrameBuffer(previsXCoord, _uiMargin, _uiColumnWidth * 2, 75);
    
    _globalPanel.draw();
    
    for(size_t i = 0; i < _patternPanels.size(); i++) {
        _patternPanels[i]->draw();
    }
    
    _previs.Draw(previsXCoord, 100, 1280, 720);
    
    _artnet.UpdateLights();
}