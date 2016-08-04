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
#include "Util.hpp"

SrSwitcher::SrSwitcher(const std::string & name,
                       const std::string & fileName,
                       SrApp * app) :
    SrUiMixin(name),
    _fileName(fileName),
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
    
    _saveButton.setup("Save", 40, 40);
    _saveButton.addListener(this, &This::_OnSaveButtonPressed);
    _AddUI(&_saveButton);
    
    _presetPanel.setup("Presets");
    _AddUI(&_presetPanel);
    
    _ReadPresets();
}

void
SrSwitcher::_ReadPresets()
{
    std::string installedFileName =
        SrUtil_GetAbsolutePathForResource(_fileName);
    std::ifstream ifs(installedFileName, std::ifstream::in);
    if (not ifs) {
        SrError("ReadPresets: error opening file %s\n",
                installedFileName.c_str());
        return;
    }
    
    // XXX note:  not deleting existing presets!  This only works
    // when called at initialization for now.
    
    // Read strings into a map
    std::map<std::string, std::vector<std::string> > _presetMap;
    
    std::string str;
    while (std::getline(ifs, str)) {
        std::vector<std::string> strVec = SrUtil_SplitString(str, '|');
        if (strVec.size() != 2) {
            SrError("error parsing line: %s\n", str.c_str());
            continue;
        }
        
        const std::string & presetName = strVec[0];
        std::string presetStr = strVec[1];
        presetStr += std::string("\n");
        
        _presetMap[presetName].push_back(presetStr);
    }
    
    for(auto iter = _presetMap.begin(); iter != _presetMap.end(); iter++) {
        const std::string name = iter->first;
        const std::vector<std::string> & strings = iter->second;
        
        SrPreset * newPreset = new SrPreset(name, _app->GetModel(), this);
        newPreset->Unpickle(strings);
        
        _AddPreset(newPreset);
        
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
    SrPreset * newPreset = new SrPreset(name, _app->GetModel(), this);
    newPreset->Store();
    _AddPreset(newPreset);
}

void
SrSwitcher::_OnSaveButtonPressed()
{
    std::string tmpFileName = std::string("/tmp/") + _fileName;
    FILE *fp = fopen(tmpFileName.c_str(), "w");
    if (not fp) {
        SrError("Error opening file");
        return;
    }
    
    for (int i = 0; i < _presets.size(); i++) {
        SrPreset * preset = _presets[i];
        const std::vector<std::string> & strings = preset->Pickle();
        for(int j = 0; j < strings.size(); j++) {
            fprintf(fp, "%s|%s", preset->GetName().c_str(), strings[j].c_str());
        }
    }
    
    fclose(fp);
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
