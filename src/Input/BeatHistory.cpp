//
//  BeatHistory.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 3/14/16.
//
//

#include "BeatHistory.hpp"
#include "Model.hpp"

SrBeatHistory::SrBeatHistory(SrModel * model) :
    SrEventHistory(model, SrFrequencyOncePerAudioIn),
    _bpm(model, SrFrequencyOncePerAudioIn),
    _beatIndex(model, SrFrequencyOncePerAudioIn),
    _measureIndex(model, SrFrequencyOncePerAudioIn),
    _globalBeatIndex(model, SrFrequencyOncePerAudioIn),
    _gotBeat(false),
    _currentBeatIndex(1),
    _currentMeasureIndex(1),
    _resetBeat(true),
    _resetMeasure(true),
    _globalBeat(0)
{
    int bufferSize = model->GetBufferSize();
    int hopSize = bufferSize / 2;
    int sampleRate = model->GetSampleRate();
    
    _beat.setup("default", bufferSize, hopSize, sampleRate);
    
    // Set up listener for beat event
    ofAddListener(_beat.gotBeat, this, &SrBeatHistory::_OnBeatEvent);
}

SrBeatHistory::~SrBeatHistory()
{
    // XXX delete _beat?  .exit()?
}

void
SrBeatHistory::_OnBeatEvent(float & time)
{
    _gotBeat = true;
    _currentBeatIndex++;
    _globalBeat++;
    
    // Yes, we're only supporting 4/4 time :)
    if (_currentBeatIndex > 4) {
        _currentBeatIndex = 1;
    }
    
    if (_resetBeat) {
        _currentBeatIndex = 1;
        _resetBeat = false;
    }
    
    if (_currentBeatIndex == 1) {
        _currentMeasureIndex++;
        
        if (_currentMeasureIndex > 8) {
            _currentMeasureIndex = 1;
        }
        
        if (_resetMeasure) {
            _currentMeasureIndex = 1;
            _resetMeasure = false;
        }
    }
}

void
SrBeatHistory::AudioIn(float * input, int bufferSize, int nChannels)
{
    _beat.audioIn(input, bufferSize, nChannels);
    
    // Push a bool value indicating whether or not a beat was received.
    _Push(_gotBeat);
    
    _beatIndex.Push(_currentBeatIndex);
    _measureIndex.Push(_currentMeasureIndex);
    _globalBeatIndex.Push(_globalBeat);
    
    // Reset the flag
    _gotBeat = false;
          
    _bpm.Push(_beat.bpm);
}

const SrFloatBuffer &
SrBeatHistory::GetBpm() const
{
    return _bpm;
}

const SrIntBuffer &
SrBeatHistory::GetBeatIndex() const
{
    return _beatIndex;
}

const SrIntBuffer &
SrBeatHistory::GetMeasureIndex() const
{
    return _measureIndex;
}

const SrIntBuffer &
SrBeatHistory::GetGlobalBeat() const
{
    return _globalBeatIndex;
}

void
SrBeatHistory::ResetDownbeat()
{
    _resetBeat = true;
}

void
SrBeatHistory::ResetMeasure()
{
    _resetMeasure = true;
}
