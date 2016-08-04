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

class SrSwitcher : public SrUiMixin {
    typedef SrSwitcher This;
    
public:
    SrSwitcher(const std::string & name,
               const std::string & fileName,
               SrApp * app);
    virtual ~SrSwitcher();
    
    void Update();
    
    void OnPresetApplyClicked(SrPreset * preset);
    
private:
    void _AddPreset(SrPreset * preset);
    SrPreset * _GetRandomPreset() const;
    void _ApplyPreset(SrPreset * preset);
    void _ReadPresets();
    
    void _OnNewButtonPressed();
    void _OnSaveButtonPressed();
    
private:
    std::string _fileName;
    SrApp * _app;
    
    ofParameter<bool> _cycleAutomatically;
    ofParameter<float> _secondsBetweenPresets;
    ofParameter<float> _secondsToNextPreset;
    
    ofxButton _newButton;
    ofxButton _saveButton;
    
    ofxPanel _presetPanel;
    std::vector<SrPreset *> _presets;
};

#endif
