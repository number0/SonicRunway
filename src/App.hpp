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
#include "Model.hpp"
#include "GlobalParameters.hpp"
#include "Artnet.hpp"
#include "Previs.hpp"
#include "Switcher.hpp"
#include "ofxOscParameterSync.h"

class SrPattern;

//
// Main app.
// Owns the audio buffer, patterns, etc.
//
class SrApp {
public:
    SrApp(ofBaseApp *ofApp);
    ~SrApp();
    
    void Update();
    void Draw();
    void AudioIn(float * input, int bufferSize, int nChannels);
    void AudioOut(float * output, int buferSize, int nChannels);
    
    const std::vector<SrPattern *> & GetPatterns() const;
    SrModel * GetModel();
    SrAudio * GetAudio();
    SrGlobalParameters * GetGlobalParameters();
    
private:
    void _AddPattern(SrPattern * pattern);
    
    void _MakeVideoPatterns();
    
private:
    SrModel _model;
    SrAudio _audio;
    SrGlobalParameters _globalParameters;
    SrPrevis _previs;
    SrArtnet _artnet;
    SrSwitcher _switcher;
    
    ofSoundStream _soundStream;
    
    std::vector<SrPattern *> _patterns;
    ofParameterGroup _patternsParameterGroup;
    
    ofxPanel _globalPanel;
    std::vector<ofxPanel *> _patternPanels;
    ofxOscParameterSync _oscParameterSync;
    float _uiColumnWidth;
    float _uiMargin;
    float _previsXCoord;
    
};

#endif
