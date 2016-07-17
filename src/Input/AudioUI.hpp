//
//  srAudioUI.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 1/10/16.
//
//  Based on the example by Paul Brossier included in ofxAubio:
//  https://github.com/aubio/ofxAubio
//

#ifndef _SR_AUDIOUI_HPP_
#define _SR_AUDIOUI_HPP_

#include <stdio.h>
#include "ofxGui.h"
#include "UiMixin.hpp"

class SrAudio;

//
// SrAudioUI - Draw some sliders showing audio algorithm settings
//
class SrAudioUI : public SrUiMixin {
        typedef SrAudioUI This;
public:
    SrAudioUI(SrAudio * audio);
    ~SrAudioUI();
    
    void Update();
    
private:
    void _OnPlayDelayedAudioButtonPressed(bool &on);
    void _OnTapDownbeatButtonPressed(bool &on);

private:
    
    SrAudio *_audio;
    
    ofxPanel _beatGui;
    
    ofParameter<bool> _playDelayedAudioParam;
    ofParameter<bool> _resetDownbeatParam;
    ofParameter<bool> _resetMeasureParam;
    
    ofxFloatSlider _bpmSlider;
    ofxFloatSlider _beatIndexSlider;
    ofxFloatSlider _measureIndexSlider;
    ofxFloatSlider _beatFftSumSlider;
    
    ofxPanel _onsetGui;
    
    ofxFloatSlider _gotOnsetSlider;
    ofxFloatSlider _onsetThresholdSlider;
    ofxFloatSlider _onsetNoveltySlider;
    ofxFloatSlider _onsetThresholdedNoveltySlider;
    
    ofxPanel _bandsGui;
    ofPolyline _bandPlot;
};

#endif 
