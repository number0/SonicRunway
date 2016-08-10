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
    
    // Get a vector of buffers containing raw FFT values over time.
    // Note: these are not filtered  or normalized, so they may not
    // be as useful for pattern generation.
    const vector<SrFloatBuffer> & GetRawFfts() const;
    
    // Get a buffer containing FFT values that have been smoothed
    // and normalized.  The value range should be roughly between
    // zero and 1, although spikes may occasionally exceed 1.0
    // The values are normalized given the current overall volume
    // level, which adjusts on the fly to keep things in a reasonable
    // range.
    const vector<SrFloatBuffer> & GetSmoothFfts() const;
    
    // Convenience for getting a vector of just the current fft
    // values.  See GetRawFfts above.
    std::vector<float> GetCurrentRawFftValues() const;
    
    // Convenience for getting a vctor of just the current smoothed
    // fft values.  See GetSmoothFfts above.
    std::vector<float> GetCurrentSmoothFftValues() const;
    
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
    
    float GetCurrentFftSum() const;
    float GetCalibratedFftSum() const;
    
    void DrawFftBands(float x, float y, float width, float height) const;
    
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
    vector<SrFloatBuffer> _rawFfts;
    vector<SrFloatBuffer> _smoothFfts;
    int _fullAudioBufferIndex;
    
    bool _outputDelayed;
    
    ofxAubioMelBands _bands;
    
    float _fftSumMax;
    
    float _currentFftValueRange;
    
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
