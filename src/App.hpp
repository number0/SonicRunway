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
#include "OscSync.hpp"

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
    void LeftAlignPrevis(bool &on);
    
    const std::vector<SrPattern *> & GetPatterns() const;
    SrModel * GetModel();
    SrAudio * GetAudio();
    SrOscSync * GetOscSync();
    SrGlobalParameters * GetGlobalParameters();
    
private:
    void _AddPattern(SrPattern * pattern);
    
    void _MakeVideoPatterns();
    void _MakeAnimPatterns();
    
private:
    SrModel _model;
    SrAudio _audio;
    SrGlobalParameters _globalParameters;
    SrPrevis _previs;
    SrArtnet _artnet;
    SrSwitcher _switcher;
    
    ofxPanel _uiTogglesPanel;
    ofParameter<bool> _showGlobals;
    ofParameter<bool> _showFft;
    ofParameter<bool> _showPatternParameters;
    ofParameter<bool> _showPrevis;
    ofParameter<bool> _intentionalCrash;
    
    ofSoundStream _soundStream;
    
    std::vector<SrPattern *> _patterns;
    ofParameterGroup _patternsParameterGroup;
    
    ofxPanel _globalPanel;
    std::vector<ofxPanel *> _patternPanels;
    SrOscSync _oscSync;
    float _uiColumnWidth;
    float _uiMargin;
    float _previsXCoord;
    
    float _previsWidth;
    float _previsHeight;
    float _leftAlignScale;
    
    bool _leftAlignPrevis;
    
};

#endif
