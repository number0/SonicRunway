//
//  SpinTrailsPattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 8/18/16.
//
//

#include "SpinTrailsPattern.hpp"
#include "Audio.hpp"
#include "Model.hpp"
#include "GlobalParameters.hpp"
#include "Util.hpp"

static const int MAX_NUM_TRAILS = 20;

SrSpinTrailsPattern::SrSpinTrailsPattern(const std::string & name,
                               SrModel * model, SrAudio * audio,
                               SrGlobalParameters * globalParameters) :
    SrScrollingPattern(name, model, audio, globalParameters),
    _numTrails(15),
    _duration(5),
    _wander(10),
    _spin(12),
    _width(16),
    _exponent(3.6),
    _multiplier(1.0),
    _scale(0.5),
    _hueOffset(0.0),
    _hueShift(0.5),
    _hueVariation(0.45),
    _minSaturation(0.3),
    _maxSaturation(1.0),
    _cutoffValue(0.1),
    _cycleHues(true)
{
    _numTrails.setName("Num Trails");
    _numTrails.setMin(1);
    _numTrails.setMax(MAX_NUM_TRAILS);
    _AddUIParameter(_numTrails);
    
    _duration.setName("Duration");
    _duration.setMin(1);
    _duration.setMax(20);
    _AddUIParameter(_duration);
    
    _wander.setName("Wander");
    _wander.setMin(0);
    _wander.setMax(20);
    _AddUIParameter(_wander);
    
    _spin.setName("Spin");
    _spin.setMin(0);
    _spin.setMax(20);
    _AddUIParameter(_spin);
    
    _width.setName("Width");
    _width.setMin(0);
    _width.setMax(20);
    _AddUIParameter(_width);
    
    _exponent.setName("Exponent");
    _exponent.setMin(0.5);
    _exponent.setMax(4.0);
    _AddUIParameter(_exponent);
    
    _multiplier.setName("Multiplier");
    _multiplier.setMin(0.5);
    _multiplier.setMax(4.0);
    _AddUIParameter(_multiplier);
    
    _scale.setName("Scale");
    _scale.setMin(0.01);
    _scale.setMax(1.0);
    _AddUIParameter(_scale);
    
    _hueOffset.setName("Hue Offset");
    _hueOffset.setMin(0.0);
    _hueOffset.setMax(1.0);
    _AddUIParameter(_hueOffset);
    
    _hueShift.setName("Hue Shift");
    _hueShift.setMin(0.0);
    _hueShift.setMax(1.0);
    _AddUIParameter(_hueShift);
    
    _hueVariation.setName("Hue Variation");
    _hueVariation.setMin(0.0);
    _hueVariation.setMax(2.0);
    _AddUIParameter(_hueVariation);
    
    _minSaturation.setName("Min Saturation");
    _minSaturation.setMin(0.0);
    _minSaturation.setMax(1.0);
    _AddUIParameter(_minSaturation);
    
    _maxSaturation.setName("Max Saturation");
    _maxSaturation.setMin(0.0);
    _maxSaturation.setMax(1.0);
    _AddUIParameter(_maxSaturation);
    
    _cutoffValue.setName("Cutoff Value");
    _cutoffValue.setMin(0.0);
    _cutoffValue.setMax(1.0);
    _AddUIParameter(_cutoffValue);
    
    _cycleHues.setName("Cycle Hues");
    _AddUIParameter(_cycleHues);
    
    _ages.resize(MAX_NUM_TRAILS);
    _offsets.resize(MAX_NUM_TRAILS);
    _velocities.resize(MAX_NUM_TRAILS);
    _flipped.resize(MAX_NUM_TRAILS);
    _hues.resize(MAX_NUM_TRAILS);
    
    for(size_t i = 0; i < _ages.size(); i++) {
        _ages[i] = 10000; // some large value
        _velocities[i] = 0;
    }
}

SrSpinTrailsPattern::~SrSpinTrailsPattern()
{
    
}

void
SrSpinTrailsPattern::_Update()
{
    if (not IsOnAtAnyGate()) {
        return;
    }
    
    SrScrollingPattern::_Update();
    
    // Get a usable value for the low RMS, between 0 and 1
    float low = GetAudio()->GetLows()[0];
    
    low = pow(low * _multiplier, _exponent);
    low = std::min(low, (float) 1.0);
    
    int numTrails = std::min((int) _numTrails, MAX_NUM_TRAILS);
    
    // Increment all the ages and spins
    for(size_t i = 0; i < MAX_NUM_TRAILS; i++) {
        _ages[i]++;
        _offsets[i] += _velocities[i];
    }
    
    // Reset the ages of the affected trails to 0
    int height = low * numTrails;
    if (height >= MAX_NUM_TRAILS) {
        height = MAX_NUM_TRAILS - 1;
    }
    
    for(size_t i = 0; i < height; i++) {
        // If this is a new trail..
        if (_ages[i] > 1) {
            if (_wander > 0) {
                _offsets[i] = rand() % (int) _wander;
            } else {
                _offsets[i] = 0;
            }
            
            if (_spin > 0) {
                _velocities[i] = rand() % (int) _spin;
            } else {
                _velocities[i] = 0;
            }
            
            _flipped[i] = rand() % 2;
            if (rand() % 2) {
                _velocities[i] *= -1;
            }
            
            float hue = _hueOffset +
                _cycleHues * GetGlobalParameters()->GetVerySlowCycle() +
                GetGlobalParameters()->GetDial1();
            
            hue += ((float) rand() / (RAND_MAX)) * _hueVariation;
            
            _hues[i] = SrUtil_ClampCycle(0, 1, hue);
        }
        
        _ages[i] = 0;
    }
    
}

void
SrSpinTrailsPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    const SrModel * model = GetModel();
    int numLights = model->GetLightsPerGate();
    
    int numTrails = std::min((int) _numTrails, MAX_NUM_TRAILS);
    
    for(size_t i = 0; i < numTrails; i++) {
        int age = _ages[i];
        if (age > (int) _duration) {
            continue;
        }
        
        float t = (float) i / numTrails * (float) _scale;
        int lightIndex = t * numLights + _offsets[i];
        
        if (_flipped[i]) {
            lightIndex = numLights - 1 - lightIndex;
        }
        
        float widthMult = 1.0 - (float) age / _duration;
        int width = _width * widthMult;
        
        for(int j = 0; j < width; j++) {
            int idxL = lightIndex + j;
            int idxR = lightIndex - j;
            
            float dist = (1.0 - (float) j / width) * widthMult;
            
            ofFloatColor c;
            float hue = _hues[i] + dist * _hueShift;
            hue = SrUtil_ClampCycle(0, 1, hue);
            float saturation = SrUtil_Lerp(_minSaturation, _maxSaturation, dist);
            float value = dist;
            
            c.setHsb(hue, saturation, value);
            
            if (idxL < numLights and idxL >= 0) {
                (*buffer)[idxL] = c;
            }
            
            if (idxR < numLights and idxR >= 0) {
                (*buffer)[idxR] = c;
            }
        
        }
    }
}
