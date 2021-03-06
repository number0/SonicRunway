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
                       SrApp * app) :
    SrUiMixin(name),
    _app(app),
    _currentPreset(NULL),
    _cyclePresets(false),
    _delayBeforeAutomatic(55.0),
    _secondsBetweenPresets(15.0),
    _secondsToNextPreset((float) _secondsBetweenPresets),
    _secondsBeforeSwitchingToNonAudioPresets(5),
    _isChoosingAudioReactivePresets(false)
{
    _cyclePresets.setName("Cycle Presets");
    _AddUIParameter(_cyclePresets);
    
    _delayBeforeAutomatic.setName("Delay b4 auto");
    _delayBeforeAutomatic.setMin(1.0);
    _delayBeforeAutomatic.setMax(60.0);
    _AddUIParameter(_delayBeforeAutomatic);
    
    _secondsBetweenPresets.setName("Time between presets");
    _secondsBetweenPresets.setMin(0.0);
    _secondsBetweenPresets.setMax(40.0);
    _AddUIParameter(_secondsBetweenPresets);
   
    _secondsToNextPreset.setName("Time to next preset");
    _secondsToNextPreset.setMin(0.0);
    _secondsToNextPreset.setMax(40.0);
    _AddUIParameter(_secondsToNextPreset);
    
    _newButton.setup("New", 40, 40);
    _newButton.addListener(this, &This::_OnNewButtonPressed);
    _AddUI(&_newButton);
    
    _saveButton.setup("Write /tmp/presets", 40, 40);
    _saveButton.addListener(this, &This::_OnSaveButtonPressed);
    _AddUI(&_saveButton);
    
    _secondsBeforeSwitchingToNonAudioPresets.setName("Delay b4 non-audio");
    _secondsBeforeSwitchingToNonAudioPresets.setMin(0.0);
    _secondsBeforeSwitchingToNonAudioPresets.setMax(45.0);
    _AddUIParameter(_secondsBeforeSwitchingToNonAudioPresets);
    
    _isChoosingAudioReactivePresets.setName("Audio Reactive");
    _AddUIParameter(_isChoosingAudioReactivePresets);
    
    _presetPanel.setup("Presets");
    _AddUI(&_presetPanel);
}

void
SrSwitcher::ReadPresets()
{
    _ReadPresets("presets_noVideos.txt");
    _ReadPresets("presets_audioVideos.txt");
    _ReadPresets("presets_justVideos.txt");
    
    //mtf 8/26 - not sure these will work properly as is, b/c
    //they don't explicitly reset the patterns to their default
    //params
    //_ReadPresets("presets_patterns.txt");
}

void
SrSwitcher::_ReadPresets(const std::string & fileName)
{
    std::string installedFileName =
        SrUtil_GetAbsolutePathForResource(fileName);
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
    
    std::vector<std::string> orderedPresetList;
    
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
        
        auto findIter = _presetMap.find(presetName);
        if (findIter == _presetMap.end()) {
            orderedPresetList.push_back(presetName);
        }
        
        _presetMap[presetName].push_back(presetStr);
    }
    
    for(auto iter = orderedPresetList.begin(); iter != orderedPresetList.end(); iter++) {
        const std::string name = *iter;
        
        auto findIter = _presetMap.find(name);
        if (findIter == _presetMap.end()) {
            SrError("Couldn't find new preset in map");
            continue;
        }
        
        const std::vector<std::string> & strings = findIter->second;
        
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
    _ApplyPreset(newPreset);
}

void
SrSwitcher::_OnSaveButtonPressed()
{
    std::string tmpFileName = std::string("/tmp/tmpPresets.txt");
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
    _presetPanel.add(preset->GetToggle());
    
    if (preset->IsAudioReactive()) {
        _audioReactivePresets.push_back(preset);
    } else {
        _nonAudioReactivePresets.push_back(preset);
    }
}

void
SrSwitcher::Update()
{
    // If it's been a while since we poked any buttons, turn
    // automatic preset cycling back on.
    if (_app->GetGlobalParameters()->ComputeSecondsSinceManualInput() >
        (float) _delayBeforeAutomatic) {
        //mtf : This is making debugging difficult
        //_cyclePresets = true;
    }
    
    bool wasChoosingAudioReactivePresets = _isChoosingAudioReactivePresets;
    
    if (_app->GetAudio()->ComputeSecondsSinceAudioSignal() >
        (float) _secondsBeforeSwitchingToNonAudioPresets) {
        _isChoosingAudioReactivePresets = false;
    } else {
        _isChoosingAudioReactivePresets = true;
    }
    
    if (not (bool) _cyclePresets) {
        return;
    }
    
    // See if we *just* got some new audio after a period of
    // silence.  In that case, force a pattern switch immediately.
    bool justGotNewAudio = not wasChoosingAudioReactivePresets and
                         (bool) _isChoosingAudioReactivePresets;
    
    float fps = _app->GetModel()->ComputeFramesPerSecond();
    float secondsPerFrame = 1.0 / fps;
    _secondsToNextPreset = (float) _secondsToNextPreset - secondsPerFrame;
    
    if (_secondsToNextPreset <= 0.0 or justGotNewAudio) {
        SrPreset * preset = _GetRandomPreset();
        if (preset) {
            _ApplyPreset(preset);
            cout << "Preset cycling switching to: " << preset -> GetName() << "\n\n";
        }
        _secondsToNextPreset = (float) _secondsBetweenPresets;
    }
    
    // catch case where we're scrubbed the "time between presets"
    // slider lower and want the timer to "catch up".
    if ((float) _secondsToNextPreset > (float) _secondsBetweenPresets) {
        _secondsToNextPreset = (float) _secondsBetweenPresets;
    }
}

SrPreset *
SrSwitcher::_GetRandomPreset() const
{
    const std::vector<SrPreset *> & presets =
        (bool) _isChoosingAudioReactivePresets ?
        _audioReactivePresets : _nonAudioReactivePresets;
    
    // XXX Considering adding this to bias the default.  Would be nicer to
    // have a file weighting how col
    // If we're choosing audio reactive presets, choose the first one
    // more often..
    if (_isChoosingAudioReactivePresets) {
        if (rand() % 3 == 0) {
            return presets[0];
        }
    }

    int r = rand();
    int s = presets.size();
    
    // Handle no presets of that type
    if ( s == 0 ){
        return nullptr;
    }
    
    size_t idx = r % s;
    
    return presets[idx];
}

void
SrSwitcher::_ApplyPreset(SrPreset * preset)
{
    if (not preset) {
        SrError("Null preset");
        return;
    }
    
    _currentPreset = preset;
    
    // Sync the preset buttons with the current state.
    for (size_t i = 0; i < _presets.size(); i++) {
        _presets[i]->SetIsCurrentPreset(false);
    }
    preset->SetIsCurrentPreset(true);
    
    // Disable all patterns.  This way we end up with only
    // the ones explicitly enabled in the preset.
    const std::vector<SrPattern *> & patterns = _app->GetPatterns();
    for (size_t i = 0; i < patterns.size(); i++) {
        if (patterns[i]->GetName() == "Trigger" || patterns[i] -> GetName() == "Dropout") {
            continue;
        }
        patterns[i]->SetEnabled(false);
    }
    
    // Now apply the new preset.
    preset->Apply();
    
    _app->GetOscSync()->BroadcastPresetInfo(preset);
}

void
SrSwitcher::OnPresetTogglePressed(SrPreset * preset)
{
    if (preset != _currentPreset) {
        _ApplyPreset(preset);
    }
    
    // Then tell the global params that we changed a preset
    _app->GetGlobalParameters()->OnReceivedPresetInput();
}

SrPattern *
SrSwitcher::FindPatternByName(const std::string & name) const
{
    const std::vector<SrPattern *> & patterns = _app->GetPatterns();
    
    for(size_t i = 0; i < patterns.size(); i++) {
        if (patterns[i]->GetName() == name) {
            return patterns[i];
        }
    }
    
    return NULL;
}

void
SrSwitcher::ApplyPresetAtIndex(size_t index)
{
    const std::vector<SrPreset *> presets =
        (bool) _isChoosingAudioReactivePresets ?
        _audioReactivePresets : _nonAudioReactivePresets;
    
    if (index >= presets.size()) {
        SrError("Unknown preset with index %zu\n", index);
        return;
    }
    
    _ApplyPreset(presets[index]);
}

int
SrSwitcher::GetPresetIndex(const SrPreset * preset) const
{
    if (not preset) {
        return;
    }
    
    for(size_t i = 0; i < _presets.size(); i++) {
        if (_presets[i] == preset) {
            return i;
        }
    }
    
    return -1;
}
