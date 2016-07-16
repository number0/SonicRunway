//
//  App.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 1/13/16.
//
//

#ifndef SR_APP_HPP
#define SR_APP_HPP

#include <stdio.h>

#include "Audio.hpp"
#include "AudioUI.hpp"
#include "Model.hpp"
#include "Artnet.hpp"
#include "Previs.hpp"
#include "ofxOscParameterSync.h"

class SrPattern;
class SrPreset;

//
// Main app.
// Owns the audio buffer, patterns, etc.
//
class SrApp {
public:
    SrApp();
    ~SrApp();
    
    void Update();
    void Draw();
    void AudioIn(float * input, int bufferSize, int nChannels);
    void AudioOut(float * output, int buferSize, int nChannels);
    
private:
    void _AddPattern(SrPattern * pattern);
    void _AddPreset(SrPreset * preset);
    
private:
    SrModel _model;
    SrAudio _audio;
    SrAudioUI _audioUI;
    SrPrevis _previs;
    SrArtnet _artnet;
    
    std::vector<SrPattern *> _patterns;
    ofParameterGroup _patternsParameterGroup;
    
    std::vector<SrPreset *> _presets;
    
    ofxPanel _globalPanel;
    ofxPanel _patternPanel;
    ofxPanel _presetPanel;
    ofxOscParameterSync _oscParameterSync;
    float _uiColumnWidth;
    float _uiMargin;
    
};

#endif
