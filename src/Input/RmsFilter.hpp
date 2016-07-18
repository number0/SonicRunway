//
//  RmsFilter.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/17/16.
//
//

#ifndef SR_RMS_FILTER_HPP
#define SR_RMS_FILTER_HPP

#include "UiMixin.hpp"
#include "Buffer.hpp"
#include <essentia/essentia.h>
#include <essentia/algorithmfactory.h>

class SrModel;
class SrAudio;

// XXX this should be defined in essentia somewhere..
typedef float Real;

//
// An audio filter that combines a bandPass and an RMS filter.
// It also provides parameters to bound the frequency range.
//
class SrRmsFilter : public SrUiMixin {
public:
    SrRmsFilter(const std::string & name,
                SrModel * model,
                SrAudio * audio,
                float initialMinFrequency,
                float initialMaxFrequency,
                const vector<Real> & _inputBuffer);
    ~SrRmsFilter();
    
    // Evaluate the filters.  Call this once per audio in.
    void Compute();
    
    // Get the output of the RMS filter.  Values are scaled
    // so that the maximum value is 1.0.  The scale factor is
    // adjusted over time to handle volume changes.
    const SrFloatBuffer & GetOutput() const;
    
    // Get access to the band-passed signal (so it can be played back)
    const vector<Real> & GetBandPassOutput() const;
    
private:
    void _UpdateFrequencyRange();
    
private:
    SrModel * _model;
    SrAudio * _audio;
    
    ofParameter<float> _minFrequencyParam;
    ofParameter<float> _maxFrequencyParam;
    
    essentia::standard::Algorithm *_bandPass;
    essentia::standard::Algorithm *_rms;
    vector<Real> _bandPassBuffer;
    
    float _maxValue;
    float _minMaxValue;
    Real _rmsOutput;
    SrFloatBuffer _output;
};

#endif
