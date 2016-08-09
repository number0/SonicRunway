//
//  Audio.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 1/9/16.
//
//  Based on the example by Paul Brossier included in ofxAubio:
//  https://github.com/aubio/ofxAubio
//

#include "Audio.hpp"

#include "ofMain.h"
#include "ofxAubio.h"
#include "ofxGui.h"
#include "Model.hpp"

using namespace essentia;
using namespace standard;

SrAudio::SrAudio(const std::string & name,
                 SrModel * model) :
    SrUiMixin(name),
    _model(model),
    _inputBuffer(_model->GetBufferSize()),
    _lowFilter("Low", model, this, 200, 700, _inputBuffer),
    _midFilter("Mid", model, this, 700, 3000, _inputBuffer),
    _highFilter("High", model, this, 3000, 20000, _inputBuffer),
    _beatHistory(model),
    _outputDelayed(false),
    _fullAudioBufferIndex(0),
    _playDelayedAudioParam(true),
    _resetDownbeatParam(false),
    _resetMeasureParam(false),
    _fftSumMax(0.1)
{
    _InitAlgorithms();
    _InitUI();
}

void
SrAudio::_InitAlgorithms()
{
    // Allocate one float buffer for each FFT band
    for (size_t i = 0; i < 40; i++) {
        _ffts.push_back(
            SrFloatBuffer(_model, SrFrequencyOncePerAudioIn));
    }
    
    // Allocate the full audio buffer, with enough space to
    // store delayed audio for the full length of the runway.
    _fullAudioBuffer.resize(_model->GetBuffersPerSecond() *
                            _model->GetMaxBufferDuration());
    for(size_t i=0; i<_fullAudioBuffer.size(); i++) {
        _fullAudioBuffer[i].resize(_model->GetBufferSize());
    }
    
    AlgorithmFactory & factory = AlgorithmFactory::instance();
    
    int sampleRate = _model->GetSampleRate();
    int bufferSize = _model->GetBufferSize();
    
    int hopSize = bufferSize / 2;
    
    _bands.setup("default", bufferSize, hopSize, sampleRate);
}

void
SrAudio::_InitUI()
{
    _playDelayedAudioParam.setName("Play Delayed Audio");
    _playDelayedAudioParam.addListener(
                                       this, &This::_OnPlayDelayedAudioButtonPressed);
    _AddUIParameter(_playDelayedAudioParam);
    
    _AddUI(_lowFilter.GetUiPanel());
    _AddUI(_midFilter.GetUiPanel());
    _AddUI(_highFilter.GetUiPanel());
    
    _resetDownbeatParam.setName("Tap Downbeat");
    _AddUIParameter(_resetDownbeatParam);
    _resetMeasureParam.setName("Tap Measure 1");
    _AddUIParameter(_resetMeasureParam);
    
    _beatGui.setup("Beat");
    _beatGui.add(_bpmSlider.setup("bpm", 0, 0, 250));
    _beatGui.add(_beatIndexSlider.setup("index", 0, 0, 4));
    _beatGui.add(_measureIndexSlider.setup("index", 0, 0, 8));
    
    // Commenting this out because the value of calibrated
    // FFT is occasionally nan, which [I think] is causing a crash
    // in the drawing code.  More debugging to follow...
    //
    //_beatGui.add(_fftSumSlider.setup("Calibrated FFT", 0, 0, 1)); // XXX Not really part of 'beat'
    _AddUI(&_beatGui);
    
    /*
     _bandsGui.setup("SrAudioMelBends");
     for (int i = 0; i < 40; i++) {
     _bandPlot.addVertex(50 + i * 650 / 40.,
     240 - 100 *
     _audio->GetCurrentFftValues()[i]);
     }
     */
    
}

SrAudio::~SrAudio()
{
    // XXX delete / exit aubio stuff?
}

const SrBeatHistory &
SrAudio::GetBeatHistory() const
{
    return _beatHistory;
}

const vector<SrFloatBuffer> &
SrAudio::GetFfts() const
{
    return _ffts;
}

const SrFloatBuffer &
SrAudio::GetLows() const
{
    return _lowFilter.GetOutput();
}

const SrFloatBuffer &
SrAudio::GetMids() const
{
    return _midFilter.GetOutput();
}

const SrFloatBuffer &
SrAudio::GetHighs() const
{
    return _highFilter.GetOutput();
}

std::vector<float>
SrAudio::GetCurrentFftValues() const
{
    std::vector<float> ret;
    for(size_t i=0; i < _ffts.size(); i++) {
        ret.push_back(_ffts[i][0]);
    }
    
    return ret;
}

float
SrAudio::GetCurrentFftSum() const
{
    std::vector<float> fftValues = GetCurrentFftValues();
    return std::accumulate(fftValues.begin(), fftValues.end(), 0.0);;
}


float
SrAudio::GetCalibratedFftSum() const
{
    float fftSum = GetCurrentFftSum();
    
    // Rescale the FFT Sum to be between 0-1 using the following:
    // OldRange = (OldMax - OldMin)
    // NewRange = (NewMax - NewMin)
    // NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin
    float oldRange = _fftSumMax - 0;
    float newRange = 1 - 0;
    float newValue = (((fftSum - 0) * newRange) / oldRange) + 0;
    
    return newValue;
}

void
SrAudio::AudioIn(float *input, int bufferSize, int nChannels)
{
    // Increment full audio buffer index, and loop around if
    // we get to the end.
    _fullAudioBufferIndex--;
    if (_fullAudioBufferIndex < 0) {
        _fullAudioBufferIndex = _fullAudioBuffer.size() - 1;
    }
    
    // Copy left channel audio into mono input buffer
    // XXX could maybe memcpy to be faster?
    for(int i=0; i < bufferSize; i++) {
        _fullAudioBuffer[_fullAudioBufferIndex][i] = input[i * nChannels];
        _inputBuffer[i] = input[i * nChannels];
    }
    
    _beatHistory.AudioIn(input, bufferSize, nChannels);
    
    _bands.audioIn(input, bufferSize, nChannels);
    
    // Run audio analysis
    _lowFilter.Compute();
    _midFilter.Compute();
    _highFilter.Compute();
    
    float *energies = _bands.energies;
    for (size_t i = 0; i < _ffts.size(); i++) {
        _ffts[i].Push(energies[i]);
    }
    
    float fftSum = GetCurrentFftSum();
    if (_fftSumMax < fftSum) {
        _fftSumMax = fftSum;
    } else {
        // If not setting a new max slowly walk the max down so that
        // if audio levels go down max will slowly adjust.
        _fftSumMax -= 0.001;
    }
}

/*
void
SrAudio::AudioOut(float *output, int bufferSize, int nChannels) const
{
    // XXX only handling mono for now.
    for(int i=0; i < bufferSize; i++) {
        //output[i*nChannels] = _bandPassBuffer[i];
        output[i*nChannels] = _inputBuffer[i];
    }
}
 */

void
SrAudio::SetOutputAudioDelayed(bool sendAudioOut)
{
    _outputDelayed = sendAudioOut;
}

void
SrAudio::AudioOutDelayed(float * output, int bufferSize, int nChannels,
                         float delayInSeconds) const
{
    if ( ! _outputDelayed ){
        return;
    }
    
    int bufferOffset = delayInSeconds * _model->GetBuffersPerSecond();
    int thisIndex = _fullAudioBufferIndex + bufferOffset;
    thisIndex %= _fullAudioBuffer.size();
   
    const vector<float> & thisBuffer = _fullAudioBuffer[thisIndex];
    
    // XXX memcpy for speed?
    for(int i=0; i < bufferSize; i++) {
        output[i*nChannels] = thisBuffer[i];
    }
}

void
SrAudio::ResetDownbeat()
{
    _beatHistory.ResetDownbeat();
}

void
SrAudio::ResetMeasure()
{
    _beatHistory.ResetMeasure();
}

void
SrAudio::UpdateUI()
{
    if ((bool) _resetDownbeatParam) {
        ResetDownbeat();
        _resetDownbeatParam = false;
    }
    
    if ((bool) _resetMeasureParam) {
        ResetMeasure();
        _resetMeasureParam = false;
    }
    
    _bpmSlider = GetBeatHistory().GetBpm()[0];
    _beatIndexSlider = GetBeatHistory().GetBeatIndex()[0];
    _measureIndexSlider = GetBeatHistory().GetMeasureIndex()[0];
    _fftSumSlider = GetCalibratedFftSum();
}

void
SrAudio::_OnPlayDelayedAudioButtonPressed(bool &on)
{
    SetOutputAudioDelayed(on);
}
