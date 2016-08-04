//
//  Switcher.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 8/2/16.
//
//

#include "Switcher.hpp"
#include "Preset.hpp"
#include "App.hpp"
#include "Model.hpp"
#include "Pattern.hpp"
#include "Debug.hpp"

SrSwitcher::SrSwitcher(const std::string & name,
                       SrApp * app) :
    SrUiMixin(name),
    _app(app),
    _cycleAutomatically(false),
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
    
    _newButton.setup("New", 40, 40);
    _newButton.addListener(this, &This::_OnNewButtonPressed);
    _AddUI(&_newButton);
    
    _presetPanel.setup("Presets");
    _AddUI(&_presetPanel);
    
    // Make a few presets for testing..
    for(size_t i = 0; i < 3; i++) {
        char * name;
        char * fileName;
        asprintf(&name, "Test %zu", i);
        asprintf(&fileName, "/tmp/test.%zu.preset", i);
        
        SrModel * model = _app->GetModel();
        SrPreset * testPreset = new SrPreset(name, model, fileName, this);
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
SrSwitcher::_OnNewButtonPressed()
{
    std::string name =
        ofSystemTextBoxDialog("New Preset Name:", "NewPreset");
    printf("name: %s\n", name.c_str());
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
    
    float fps = _app->GetModel()->ComputeFramesPerSecond();
    float secondsPerFrame = 1.0 / fps;
    _secondsToNextPreset = (float) _secondsToNextPreset - secondsPerFrame;
    
    if (_secondsToNextPreset <= 0.0) {
        SrPreset * preset = _GetRandomPreset();
        if (preset) {
            _ApplyPreset(preset);
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

void
SrSwitcher::OnPresetApplyClicked(SrPreset * preset)
{
    _ApplyPreset(preset);
}

void
SrSwitcher::_ApplyPreset(SrPreset * preset)
{
    if (not preset) {
        SrError("Null preset");
        return;
    }
    
    // First, disable all patterns.  This way we end up with only
    // the ones explicitly enabled in the preset.
    const std::vector<SrPattern *> & patterns = _app->GetPatterns();
    for (size_t i = 0; i < patterns.size(); i++) {
        patterns[i]->SetEnabled(false);
    }
    
    // Now apply the new preset.
    preset->Apply();
}
