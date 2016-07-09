//
//  BeatDetect.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 3/14/16.
//
//

#ifndef SR_BEAT_HISTORY_HPP
#define SR_BEAT_HISTORY_HPP

#include "EventHistory.hpp"
#include "ofxAubioBeat.h"

//
// SrBeatHistory - an SrEventHistory that records beat detection and BPM.
//
class SrBeatHistory : public SrEventHistory {
public:
    SrBeatHistory(SrModel * model);
    virtual ~SrBeatHistory();
    
    void AudioIn(float * input, int bufferSize, int nChannels);
    
    const SrFloatBuffer & GetBpm() const;
    
    // Buffer containing the current beat [1-4]
    const SrIntBuffer & GetBeatIndex() const;
    
    // Buffer containing the current measure in the phrase [1-8]
    const SrIntBuffer & GetMeasureIndex() const;
    
    // Make it so the next beat will be 1.
    void ResetDownbeat();
    
    // Make it so the next measure will be 1.
    void ResetMeasure();
    
private:
    void _OnBeatEvent(float & time);
    
private:
    SrFloatBuffer _bpm;
    SrIntBuffer _beatIndex;
    SrIntBuffer _measureIndex;
    
    bool _gotBeat;
    
    int _currentBeatIndex;
    int _currentMeasureIndex;
    
    bool _resetBeat;
    bool _resetMeasure;
    
    ofxAubioBeat _beat;
};

#endif 
