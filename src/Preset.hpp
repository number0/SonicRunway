//
//  Preset.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/9/16.
//
//

#ifndef SR_PRESET_HPP
#define SR_PRESET_HPP

#include <stdio.h>
#include <string>

#include "UiMixin.hpp"

class SrModel;
class SrSwitcher;

//
// A group of parameter settings that can be applied
// to the app.  When 'stored', a preset stores the settings
// of the current patterns. When 'applied', it sets the
// values.
//
// The intent here is to provide a way to configure the
// runway across a variety of patterns and settings, and
// return to that state with a single button.
//
class SrPreset : public SrUiMixin {
    typedef SrPreset This;
    
public:
    SrPreset(const std::string & name,
             SrModel * model,
             SrSwitcher * switcher);
    ~SrPreset();
    
    void Apply() const;
    void Store();
    
    const std::vector<std::string> & Pickle() const;
    void Unpickle(const std::vector<std::string> & strings);
    
private:
    void _OnApplyPressed();
    void _OnStorePressed();
    
    void _WriteParamRecurse(const ofAbstractParameter & param,
                            ofParameterGroup & rootGroup,
                            const std::string & parentPath);
    
private:
    SrModel * _model;
    std::vector<std::string> _strings;
    SrSwitcher * _switcher;
    
    ofxButton _applyButton;
    ofxButton _storeButton;
    
    ofParameter<bool> _applyParam;
    ofParameter<bool> _storeParam;
};

#endif /* Preset_hpp */
