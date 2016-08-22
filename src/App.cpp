//
//  App.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 1/13/16.
//
//

#include "App.hpp"
#include "ofApp.h"
#include "Util.hpp"
#include "Debug.hpp"
#include "BeatPattern.hpp"
#include "PaletteBandsPattern.hpp"
#include "FftPattern.hpp"
#include "ExamplePattern.hpp"
#include "PhrasePattern.hpp"
#include "StripesPattern.hpp"
#include "DoubleRainbowPattern.hpp"
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
#include "WhompPattern.hpp"
#include "BigTrailsPattern.hpp"
#include "NetworkInputPattern.hpp"
#include "SpinTrailsPattern.hpp"
#include "GlobalParameters.hpp"

#include "Switcher.hpp"

extern "C" void CGSSetDebugOptions(int);
extern "C" void CGSDeferredUpdates(int);

static const size_t PATTERNS_PER_COLUMN = 5;

SrApp::SrApp(ofBaseApp * ofApp) :
    _model(),
    _audio("Audio", &_model),
    _globalParameters("GlobalParams", &_model, &_audio),
    _artnet("Artnet", &_model),
    _previs(&_model, &_audio, this),
    _switcher("Switcher", "presets.txt", this),
    _oscSync(&_model, &_audio, &_switcher, 8000, "", 9000),
    _showGlobals(true),
    _showFft(true),
    _showPatternParameters(true),
    _showPrevis(true),
    _uiColumnWidth(220),
    _uiMargin(10),
    _previsXCoord(0),
    _previsWidth(1280),
    _previsHeight(720),
    _leftAlignScale(.7),
    _leftAlignPrevis(true)
{
    ofSetFrameRate(_model.ComputeFramesPerSecond());
    
    // Disable vertical sync so FPS can be greater than 60.
    ofSetVerticalSync(false);
    CGSSetDebugOptions(0x08000000);
    CGSDeferredUpdates(true);
    
    _uiTogglesPanel.setup("UI Toggles");
    _uiTogglesPanel.setPosition(_uiMargin, _uiMargin);
    _showGlobals.setName("Show Globals");
    _showFft.setName("Show FFT");
    _showPatternParameters.setName("Show Patterns");
    _showPrevis.setName("Show Previs");
    _uiTogglesPanel.add(_showGlobals);
    _uiTogglesPanel.add(_showFft);
    _uiTogglesPanel.add(_showPatternParameters);
    _uiTogglesPanel.add(_showPrevis);
    
    _globalPanel.setup("Global");
    
    _globalPanel.add(_previs.GetUiPanel());
    _globalPanel.add(_artnet.GetUiPanel());
    _globalPanel.add(_audio.GetUiPanel());
    
    _switcher.GetUiPanel()->setPosition(_uiMargin + _uiColumnWidth, _uiMargin);
    
    _patternsParameterGroup.setName("Patterns");
    _model.GetParameterGroup().add(_patternsParameterGroup);
    _model.GetParameterGroup().add(_audio.GetParameterGroup());
    _model.GetParameterGroup().add(_previs.GetParameterGroup());
    _model.GetParameterGroup().add(_globalParameters.GetParameterGroup());

    _MakeAnimPatterns();
    
    SrExamplePattern *examplePattern =
    new SrExamplePattern("Example", &_model, &_audio, &_globalParameters);
    _AddPattern(examplePattern);
    
    SrPaletteBandsPattern *paletteBouncePattern =
    new SrPaletteBandsPattern("Palette Bands", &_model, &_audio, &_globalParameters);
    _AddPattern(paletteBouncePattern);
    
    SrBeatPattern * beatPattern =
        new SrBeatPattern("Beat", &_model, &_audio, &_globalParameters);
    _AddPattern(beatPattern);
    beatPattern->SetEnabled(false);
    
    SrFftPattern *fftPattern =
        new SrFftPattern("Fft", &_model, &_audio, &_globalParameters);
    _AddPattern(fftPattern);

    SrStripesPattern *stripesPattern =
        new SrStripesPattern("Stripes", &_model, &_audio, &_globalParameters);
    _AddPattern(stripesPattern);

    SrDoubleRainbowPattern *DoubleRainbowPattern =
    new SrDoubleRainbowPattern("DoubleRainbow", &_model, &_audio, &_globalParameters);
    _AddPattern(DoubleRainbowPattern);
    
    SrStarPattern *starPattern =
        new SrStarPattern("Star", &_model, &_audio, &_globalParameters);
    _AddPattern(starPattern);
    
    SrRainbowPattern *rainbowPattern =
    new SrRainbowPattern("Rainbow", &_model, &_audio, &_globalParameters);
    _AddPattern(rainbowPattern);
    
    SrArcPattern *arcPattern =
    new SrArcPattern("Arc", &_model, &_audio, &_globalParameters);
    _AddPattern(arcPattern);
    
    SrPhasorPattern *phasorPattern =
    new SrPhasorPattern("Phasor", &_model, &_audio, &_globalParameters);
    _AddPattern(phasorPattern);
    
    SrTrailsPattern *trailsPattern =
    new SrTrailsPattern("Trails", &_model, &_audio, &_globalParameters);
    _AddPattern(trailsPattern);
    
    /*
    SrVideoPattern *videoPattern =
        new SrVideoPattern("Fireplace", "fireplace2.mov",
                           &_model, &_audio, &_globalParameters);
    _AddPattern(videoPattern);
     */
    
    SrAnimPattern *animPattern =
        new SrAnimPattern("Lightning", "", "lightning", 82, false,
                          &_model, &_audio, &_globalParameters);
    _AddPattern(animPattern);
    
    SrTriggerPattern *triggerPattern =
        new SrTriggerPattern("Trigger", &_model, &_audio, &_globalParameters);
    _AddPattern(triggerPattern);
    
    SrPhrasePattern *phrasePattern =
        new SrPhrasePattern("Phrase", &_model, &_audio, &_globalParameters);
    _AddPattern(phrasePattern);
    
    SrDiagnosticPattern *diagnosticPattern =
        new SrDiagnosticPattern("Diagnostic", &_model, &_audio, &_globalParameters);
    _AddPattern(diagnosticPattern);
    
    SrRmsPattern *rmsPattern =
        new SrRmsPattern("RMS", &_model, &_audio, &_globalParameters);
    _AddPattern(rmsPattern);

    SrBigTrailsPattern *bigTrailsPattern =
    new SrBigTrailsPattern("Big Trails", &_model, &_audio, &_globalParameters);
    _AddPattern(bigTrailsPattern);

    SrNetworkInputPattern *networkInputPattern =
    new SrNetworkInputPattern("Network Input", &_model, &_audio, &_globalParameters);
    _AddPattern(networkInputPattern);
    
    //_MakeVideoPatterns();

    // Enable the patterns we want on by default.
    //diagnosticPattern->SetEnabled(true);
    fftPattern->SetEnabled(false);
    
    SrWhompPattern *whompPattern =
    new SrWhompPattern("Whomp", &_model, &_audio, &_globalParameters);
    _AddPattern(whompPattern);
    
    SrSpinTrailsPattern *spinTrailsPattern =
    new SrSpinTrailsPattern("SpinTrails", &_model, &_audio, &_globalParameters);
    _AddPattern(spinTrailsPattern);
    
    //_MakeVideoPatterns();

    // Enable the patterns we want on by default.
    //diagnosticPattern->SetEnabled(true);
    //fftPattern->SetEnabled(true);
    //whompPattern->SetEnabled(true);
    //phrasePattern->SetEnabled(true);
    
    // Add global parameters to the model so they will be accessible
    // from osc
    _model.GetParameterGroup().add(_globalParameters.GetParameterGroup());
    
    ofSoundStreamListDevices();
    
    _soundStream.setup(ofApp,_model.GetNumChannels(), _model.GetNumChannels(),
                      _model.GetSampleRate(), _model.GetBufferSize(), 4);
    
    std::vector<ofSoundDevice> loopbackDevices =
        _soundStream.getMatchingDevices("Loopback Audio");
    printf("number of matching loopback devices %zu\n", loopbackDevices.size());
    if (not loopbackDevices.empty()) {
        int loopbackDeviceID = loopbackDevices[0].deviceID;
        printf("setting loopback device %d\n", loopbackDeviceID);
        _soundStream.setDeviceID(loopbackDeviceID);
    }
    
    // Calculate X position of the previs
    _previsXCoord = _uiMargin + _uiColumnWidth * (_patternPanels.size() + 2);
    
    // This must happen after patterns are initialized.
    _switcher.ReadPresets();
    
    LeftAlignPrevis(_leftAlignPrevis);
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
SrApp::_MakeAnimPatterns()
{
    std::string animFolder = SrUtil_GetAbsolutePathForResource("../../../data/Anim/");
    
    ofDirectory dir(animFolder);
    dir.listDir();
    
    for(size_t i = 0; i < dir.size(); i++) {
        std::string fname = dir.getName(i);
        ofDirectory current(animFolder + "/" + fname);
        current.listDir();
        
        if (fname.compare(0,5, "Anim_") != 0) {
            continue;
        }
        
        std::string patternName = fname.substr(5, fname.size());
        
        cout << "Loading anim pattern '" <<
            current.getName(i) << "' with num frames: "
            << current.getFiles().size() << "\n\n";

        SrAnimPattern *animPattern =
        new SrAnimPattern(patternName, "../../../data/Anim/" + fname, fname,
                          current.getFiles().size(), false,
                          &_model, &_audio, &_globalParameters);
        _AddPattern(animPattern);
        
    }
}

void
SrApp::_MakeVideoPatterns()
{
    std::string videoFolder = SrUtil_GetAbsolutePathForResource("");
    
    ofDirectory dir(videoFolder);
    dir.allowExt("mov");
    dir.listDir();
    
    for(size_t i = 0; i < dir.size(); i++) {
        std::string fname = dir.getName(i);
        if (fname.compare(0,5, "Auto_") != 0) {
            continue;
        }
        
        SrDebug("Found auto movie %s\n", dir.getPath(i).c_str());
        
        std::string patternName = fname.substr(5, fname.size() - 9);
        
        SrVideoPattern *videoPattern =
            new SrVideoPattern(patternName, fname,
                               &_model, &_audio, &_globalParameters);
        _AddPattern(videoPattern);
    }
}

void
SrApp::LeftAlignPrevis(bool &on)
{
    _leftAlignPrevis = on;
    
    float globalParamsY = 560;
    
    if( on ) {
        float x = _uiMargin + _previsWidth * _leftAlignScale + _uiMargin;
        _globalPanel.setPosition(x, _uiMargin);
        _globalParameters.GetUiPanel()->setPosition(x, _uiMargin + globalParamsY);
    }
    else {
        _globalPanel.setPosition(_uiMargin,_uiMargin + 100);
        _globalParameters.GetUiPanel()->setPosition(
                        _uiMargin, _uiMargin + 100 + globalParamsY);
    }
    
    if (on) {
        _switcher.GetUiPanel()->setPosition(_uiMargin + _uiColumnWidth + _previsWidth *_leftAlignScale + _uiMargin, _uiMargin);
    }
    else {
        _switcher.GetUiPanel()->setPosition(_uiMargin + _uiColumnWidth, _uiMargin);
    }
    
    int idx = 0;
    for(auto iter = _patternPanels.begin(); iter != _patternPanels.end(); iter++) {
        ofxPanel *panel = *iter;
        if (on) {
            panel->setPosition(_uiMargin + _uiColumnWidth * (2 + idx) + _previsWidth *_leftAlignScale + _uiMargin, _uiMargin);
        }
        else {
            panel->setPosition(_uiMargin + _uiColumnWidth * (2 + idx), _uiMargin);
        }
        idx++;
    }
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
        newPanel->setPosition(_uiMargin + _uiColumnWidth * (2 + panelIdx), _uiMargin);
        _patternPanels.push_back(newPanel);
    }
    
    // Add the Ui
    _patternPanels[panelIdx]->add(pattern->GetUiPanel());
    
    _patternsParameterGroup.add(pattern->GetParameterGroup());
}

const std::vector<SrPattern *> &
SrApp::GetPatterns() const
{
    return _patterns;
}

SrModel *
SrApp::GetModel()
{
    return &_model;
}

SrAudio *
SrApp::GetAudio()
{
    return &_audio;
}

SrOscSync *
SrApp::GetOscSync()
{
    return &_oscSync;
}

SrGlobalParameters *
SrApp::GetGlobalParameters()
{
    return &_globalParameters;
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
    _globalParameters.Update();
    
    _oscSync.Update();
    
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
        // need to draw if pattern fading out to prevent pops
        // on preset transitions
        if( pattern->GetEnabled() || pattern->IsOnAtAnyGate() ) {
            pattern->Draw();
        }
    }
    
    _model.EndDrawing();
    
    ofBackground(40,40,40);
    
    if ( _leftAlignPrevis ){
        _model.RenderFrameBuffer(_uiMargin + _uiColumnWidth * 2,
                                 _uiMargin, _uiColumnWidth * 2, 75);
    }
    else {
        _model.RenderFrameBuffer(_previsXCoord, _uiMargin, _uiColumnWidth * 2, 75);
    }
        
    _uiTogglesPanel.draw();
    
    if (_showGlobals) {
        _globalPanel.draw();
        _globalParameters.GetUiPanel()->draw();
        _switcher.GetUiPanel()->draw();
    }
    
    if (_showFft) {
        if ( _leftAlignPrevis ){
            _audio.DrawFftBands(_uiColumnWidth + _uiMargin + _previsWidth*_leftAlignScale, 400, _uiColumnWidth, _uiColumnWidth);
        }
        else {
            _audio.DrawFftBands(_uiColumnWidth + _uiMargin, 400, _uiColumnWidth, _uiColumnWidth);
        }
    }
    
    if (_showPatternParameters) {
        for(size_t i = 0; i < _patternPanels.size(); i++) {
            _patternPanels[i]->draw();
        }
    }
    
    if (_showPrevis) {
        if( _leftAlignPrevis ) {
            _previs.Draw(_uiMargin, 100, _previsWidth*_leftAlignScale, _previsHeight*_leftAlignScale);
        }
        else {
            _previs.Draw(_previsXCoord, 100, _previsWidth, _previsHeight);
        }
    }
    
    _artnet.UpdateLights();
}
