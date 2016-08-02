//
//  Switcher.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 8/2/16.
//
//

#include "Switcher.hpp"
#include "Preset.hpp"
#include "Model.hpp"

SrSwitcher::SrSwitcher(const std::string & name,
                       SrModel * model,
                       SrAudio * audio) :
    SrUiMixin(name),
    _model(model),
    _audio(audio),
    _cycleAutomatically(true),
    _secondsBetweenPresets(3.0),
    _secondsToNextPreset((float) _secondsBetweenPresets)
{
    _cycleAutomatically.setName("Cycle Automatically");
    _AddUIParameter(_cycleAutomatically);
    
    _secondsBetweenPresets.setName("Time between presets");
    _secondsBetweenPresets.setMin(0.0);
    _secondsBetweenPresets.setMax(10.0);
    _AddUIParameter(_secondsBetweenPresets);
   
    _secondsToNextPreset.setName("Time to next preset");
    _secondsToNextPreset.setMin(0.0);
    _secondsToNextPreset.setMax(10.0);
    _AddUIParameter(_secondsToNextPreset);
    
    _AddUI(&_presetPanel);
    
    // Make a few presets for testing..
    for(size_t i = 0; i < 3; i++) {
        char * name;
        char * fileName;
        asprintf(&name, "Test %zu", i);
        asprintf(&fileName, "/tmp/test.%zu.preset", i);
        
        SrPreset * testPreset = new SrPreset(name, model, fileName);
        _AddPreset(testPreset);
        
        free(name);
        free(fileName);
    }
    
}

SrSwitcher::~SrSwitcher()
{
    // XXX Should delete presets here...
    
}

void
SrSwitcher::_AddPreset(SrPreset * preset)
{
    _presets.push_back(preset);
    _presetPanel.add(preset->GetUiPanel());
}

void
SrSwitcher::Update()
{
    if (not (bool) _cycleAutomatically) {
        return;
    }
    
    float secondsPerFrame = 1.0 / _model->ComputeFramesPerSecond();
    _secondsToNextPreset = (float) _secondsToNextPreset - secondsPerFrame;
    
    if (_secondsToNextPreset <= 0.0) {
        SrPreset * preset = _GetRandomPreset();
        if (preset) {
            preset->Apply();
        }
        _secondsToNextPreset = (float) _secondsBetweenPresets;
    }
}

SrPreset *
SrSwitcher::_GetRandomPreset() const
{
    size_t idx = rand() % _presets.size();
    return _presets[idx];
}