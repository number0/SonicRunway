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

class SrModel;
class SrAudio;
class SrPreset;

class SrSwitcher : public SrUiMixin {
    typedef SrSwitcher This;
    
public:
    SrSwitcher(const std::string & name, SrModel * model, SrAudio * audio);
    virtual ~SrSwitcher();
    
    void Update();
    
private:
    void _AddPreset(SrPreset * preset);
    SrPreset * _GetRandomPreset() const;
    
private:
    SrModel * _model;
    SrAudio * _audio;
    
    ofParameter<bool> _cycleAutomatically;
    ofParameter<float> _secondsBetweenPresets;
    ofParameter<float> _secondsToNextPreset;
    
    ofxPanel _presetPanel;
    std::vector<SrPreset *> _presets;
};

#endif
