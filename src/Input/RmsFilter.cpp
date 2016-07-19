//
//  RmsFilter.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/17/16.
//
//

#include "RmsFilter.hpp"

using namespace essentia;
using namespace standard;

SrRmsFilter::SrRmsFilter(const std::string & name,
                         SrModel * model,
                         SrAudio * audio,
                         float initialMinFrequency,
                         float initialMaxFrequency,
                         const vector<Real> & inputBuffer) :
    SrUiMixin(name),
    _model(model),
    _audio(audio),
    _output(model, SrFrequencyOncePerAudioIn),
    _minFrequencyParam(initialMinFrequency),
    _maxFrequencyParam(initialMaxFrequency),
    _maxValue(0.00001),
    _minMaxValue(0.001)
{
    AlgorithmFactory & factory = AlgorithmFactory::instance();

    _bandPass = factory.create("BandPass",
                               "sampleRate", _model->GetSampleRate());
    _bandPass->input("signal").set(inputBuffer);
    _bandPass->output("signal").set(_bandPassBuffer);
    
    _rms = factory.create("RMS");
    _rms->input("array").set(_bandPassBuffer);
    _rms->output("rms").set(_rmsOutput);
    
    _minFrequencyParam.setMin(20);
    _minFrequencyParam.setMax(20000);
    _maxFrequencyParam.setMin(20);
    _maxFrequencyParam.setMax(20000);
    
    _AddUIParameter(_minFrequencyParam);
    _AddUIParameter(_maxFrequencyParam);
    
    _UpdateFrequencyRange();
}

SrRmsFilter::~SrRmsFilter()
{
    
}

void
SrRmsFilter::_UpdateFrequencyRange()
{
    if (_maxFrequencyParam < _minFrequencyParam) {
        _maxFrequencyParam = _minFrequencyParam + 2;
    }
    
    float cutoffFrequency = (float) _minFrequencyParam;
    float bandwidth = (float) _maxFrequencyParam -
                      (float) _minFrequencyParam;
   
    _bandPass->configure("cutoffFrequency", cutoffFrequency,
                         "bandwidth", bandwidth);
}

void
SrRmsFilter::Compute()
{
    _UpdateFrequencyRange();
    
    // Creep the max value downwards to adjust for volume changes
    // XXX this multiplier should really be related to the sample
    // and buffer size...
    _maxValue = _maxValue * 0.995;
    
    // Clamp the max value to a minimum-maximum.  This prevents us
    // from over-amplifying a signal that should truly be quiet.
    if (_maxValue < _minMaxValue) {
        _maxValue = _minMaxValue;
    }
    
    //printf("_maxValue %f\n", _maxValue);
    
    _bandPass->compute();
    _rms->compute();
    
    if (_rmsOutput > _maxValue) {
        _maxValue = _rmsOutput;
    }
    
    _output.Push(_rmsOutput / _maxValue);
}

const SrFloatBuffer &
SrRmsFilter::GetOutput() const
{
    return _output;
}

const vector<Real> &
SrRmsFilter::GetBandPassOutput() const
{
    return _bandPassBuffer;
}