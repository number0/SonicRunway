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
#include "TriggerPattern.hpp"
#include "VideoPattern.hpp"
#include "DiagnosticPattern.hpp"
#include "RmsPattern.hpp"

#include "Preset.hpp"

extern "C" void CGSSetDebugOptions(int);
extern "C" void CGSDeferredUpdates(int);

SrApp::SrApp() :
    _model(),
    _audio(&_model),
    _audioUI(&_audio),
    _artnet("Artnet", &_model),
    _previs(&_model, &_audio),
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
    
    _patternPanel.setup("PatternsUI");
    _patternPanel.setPosition(_uiMargin + _uiColumnWidth, _uiMargin);
    
    _presetPanel.setup("Presets");
    _presetPanel.setPosition(_uiMargin + _uiColumnWidth, 600);
    
    _globalPanel.add(_previs.GetUiPanel());
    _globalPanel.add(_artnet.GetUiPanel());
    _globalPanel.add(_audioUI.GetUiPanel());
    
    _patternsParameterGroup.setName("Patterns");
    _model.GetParameterGroup().add(_patternsParameterGroup);
    _model.GetParameterGroup().add(_audioUI.GetParameterGroup());
    _model.GetParameterGroup().add(_previs.GetParameterGroup());
    
    SrExamplePattern *examplePattern =
        new SrExamplePattern("Example", &_model, &_audio);
    _AddPattern(examplePattern);
    examplePattern->SetEnabled(false);
    
    SrBeatPattern * beatPattern =
        new SrBeatPattern("Beat", &_model, &_audio);
    _AddPattern(beatPattern);
    beatPattern->SetEnabled(false);
    
    SrFftPattern *fftPattern =
        new SrFftPattern("Fft", &_model, &_audio);
    _AddPattern(fftPattern);
    fftPattern->SetEnabled(true);
    
    SrStripesPattern *stripesPattern =
        new SrStripesPattern("Stripes", &_model, &_audio);
    _AddPattern(stripesPattern);
    stripesPattern->SetEnabled(false);
    
    SrStarPattern *starPattern =
        new SrStarPattern("Star", &_model, &_audio);
    _AddPattern(starPattern);
    
    /*
     // Disabled b/c it seems like it might be slow.
     //
    SrVideoPattern *videoPattern =
        new SrVideoPattern("Video", "fireplace2.mov", &_model, &_audio);
    _AddPattern(videoPattern);
    videoPattern->SetEnabled(false);
     */
    
    SrTriggerPattern *triggerPattern =
        new SrTriggerPattern("Trigger", &_model, &_audio);
    _AddPattern(triggerPattern);
    triggerPattern->SetEnabled(true);
    
    SrPhrasePattern *phrasePattern =
        new SrPhrasePattern("Phrase", &_model, &_audio);
    _AddPattern(phrasePattern);
    phrasePattern->SetEnabled(false);
    
    SrDiagnosticPattern *diagnosticPattern =
        new SrDiagnosticPattern("Diagnostic", &_model, &_audio);
    _AddPattern(diagnosticPattern);
    diagnosticPattern->SetEnabled(false);
    
    SrRmsPattern *rmsPattern =
        new SrRmsPattern("RMS", &_model, &_audio);
    _AddPattern(rmsPattern);
    rmsPattern->SetEnabled(false);
    
    _oscParameterSync.setup(_model.GetParameterGroup(), 8000, "", 9000);
    
    ofSoundStreamSetup(_model.GetNumChannels(), _model.GetNumChannels(),
                       _model.GetSampleRate(), _model.GetBufferSize(), 4);
    
    SrPreset *testPreset = new SrPreset("Test Preset", &_model,
                                        "/tmp/test.preset");
    _AddPreset(testPreset);
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
    _patternPanel.add(pattern->GetUiPanel());
    _patternsParameterGroup.add(pattern->GetParameterGroup());
}

void
SrApp::_AddPreset(SrPreset * preset)
{
    _presets.push_back(preset);
    _presetPanel.add(preset->GetUiPanel());
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
    
    for(auto iter = _patterns.begin(); iter != _patterns.end(); iter++) {
        SrPattern *pattern = *iter;
        pattern->Update();
    }
    
    _audioUI.Update();
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
    
    _model.RenderFrameBuffer(_uiMargin + _uiColumnWidth * 2, _uiMargin,
                             _uiColumnWidth * 2, 75);
    
    _globalPanel.draw();
    _patternPanel.draw();
    _presetPanel.draw();
    
    _previs.Draw(_uiMargin + _uiColumnWidth * 2, 100,
                 1280, 720);
    
    _artnet.UpdateLights();
}