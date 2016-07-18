//
//  Audio.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 1/9/16.
//
//  Based on the example by Paul Brossier included in ofxAubio:
//  https://github.com/aubio/ofxAubio
//

#ifndef _SR_AUDIO_HPP_
#define _SR_AUDIO_HPP_

#include <stdio.h>
#include "ofxAubio.h"
#include "Types.hpp"
#include "Buffer.hpp"
#include "UiMixin.hpp"
#include "BeatHistory.hpp"
#include "RmsFilter.hpp"

#include "ofMain.h"

#include <essentia/essentia.h>
#include <essentia/algorithmfactory.h>

class ofApp;
class SrModel;


// XXX this should be defined in essentia somewhere..
typedef float Real;

//
// SrAudio - Audio input and processing.
//
class SrAudio : public SrUiMixin {
    typedef SrAudio This;
    
public:
    SrAudio(const std::string & name, SrModel * model);
    ~SrAudio();
    
    const SrBeatHistory & GetBeatHistory() const;
    const vector<SrFloatBuffer> & GetFfts() const;
    
    const SrFloatBuffer & GetLows() const;
    const SrFloatBuffer & GetMids() const;
    const SrFloatBuffer & GetHighs() const;
    
    void AudioIn(float * input, int bufferSize, int nChannels);
    //void AudioOut(float * output, int bufferSize, int nChannels) const;
    void AudioOutDelayed(float * output, int bufferSize, int nChannels,
                         float delayInSeconds) const;
    
    void SetOutputAudioDelayed(bool sendAudioOut);
    
    // Reset it so the next beat will be 1.
    void ResetDownbeat();
    
    // Reset it so the next measure will be 1.
    void ResetMeasure();
    
    typedef vector<vector<float> > AudioVecBuffer;
    const AudioVecBuffer & GetFullAudioBuffer(float delayInSeconds) const;
    
    std::vector<float> GetCurrentFftValues() const;
    float GetCurrentFftSum() const;
    float GetCalibratedFftSum() const;
    
    void UpdateUI();
    
private:
    void _InitAlgorithms();
    void _InitUI();
    
    void _OnPlayDelayedAudioButtonPressed(bool &on);
    void _OnTapDownbeatButtonPressed(bool &on);
    
private:
    SrModel * _model;
    
    vector<Real> _inputBuffer;
    AudioVecBuffer _fullAudioBuffer;
    
    SrRmsFilter _lowFilter;
    SrRmsFilter _midFilter;
    SrRmsFilter _highFilter;
    
    SrBeatHistory _beatHistory;
    vector<SrFloatBuffer> _ffts;
    int _fullAudioBufferIndex;
    
    bool _outputDelayed;
    
    ofxAubioMelBands _bands;
    
    float _fftSumMax;
    
    // UI ------------------
    ofxPanel _beatGui;
    
    ofParameter<bool> _playDelayedAudioParam;
    ofParameter<bool> _resetDownbeatParam;
    ofParameter<bool> _resetMeasureParam;
    
    ofxFloatSlider _fftSumSlider;
    
    ofxFloatSlider _bpmSlider;
    ofxFloatSlider _beatIndexSlider;
    ofxFloatSlider _measureIndexSlider;
    
    ofxPanel _bandsGui;
    ofPolyline _bandPlot;
};

#endif
