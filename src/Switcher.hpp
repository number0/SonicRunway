//
//  Switcher.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 8/2/16.
//
//

#ifndef SR_SWITCHER_HPP
#define SR_SWITCHER_HPP

#include "UiMixin.hpp"

class SrApp;
class SrPreset;
class SrPattern;

//
// SrSwitcher - owns a set of presets and controls switching between them.
//
// Preset values are stored in a "presets.txt" file, which is read at startup.
// Presets can be stored and applied, reconfiguring the parameters of patterns.
//
// The presets file can be written out to a temp file, then copied
// over the installed version.
//
// The switcher can also automatically switch between presets, choosing
// a new one randomly from the group at a specified interval.
//
// Presets must be uniquely named, and not contain a '|', '/', or ':'
//
class SrSwitcher : public SrUiMixin {
    typedef SrSwitcher This;
    
public:
    SrSwitcher(const std::string & name,
               const std::string & fileName,
               SrApp * app);
    virtual ~SrSwitcher();
    
    void Update();
    
    void OnPresetTogglePressed(SrPreset *preset);
    
    SrPattern * FindPatternByName(const std::string & name) const;
    
    void ReadPresets();
    
    void ApplyPresetAtIndex(size_t index);
    
private:
    void _AddPreset(SrPreset * preset);
    SrPreset * _GetRandomPreset() const;
    void _ApplyPreset(SrPreset * preset);
    
    void _OnNewButtonPressed();
    void _OnSaveButtonPressed();
    
private:
    std::string _fileName;
    SrApp * _app;
    SrPreset * _currentPreset;
    
    ofParameter<bool> _cyclePresets;
    ofParameter<float> _delayBeforeAutomatic;
    ofParameter<float> _secondsBetweenPresets;
    ofParameter<float> _secondsToNextPreset;
    
    ofParameter<float> _secondsBeforeSwitchingToNonAudioPresets;
    ofParameter<bool> _isChoosingAudioReactivePresets;
    
    ofxButton _newButton;
    ofxButton _saveButton;
    
    ofxPanel _presetPanel;
    std::vector<SrPreset *> _presets;
    std::vector<SrPreset *> _audioReactivePresets;
    std::vector<SrPreset *> _nonAudioReactivePresets;
};

#endif
