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
#include <vector>
#include "ofMain.h"
#include "ofxGui.h"

class SrModel;
class SrSwitcher;
class SrPattern;

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
class SrPreset {
    typedef SrPreset This;
    
public:
    SrPreset(const std::string & name,
             SrModel * model,
             SrSwitcher * switcher);
    virtual ~SrPreset();
    
    const std::string & GetName() const;
    
    // Access UI so we can stick it in the panel
    ofxToggle * GetToggle();
    
    bool IsCurrentPreset() const;
    void SetIsCurrentPreset(bool isCurrentPreset);
    
    // Returns true if any of the patterns that are enabled by
    // this preset are audio reactive.
    bool IsAudioReactive() const;
    
    void Apply() const;
    void Store();
    
    const std::vector<std::string> & Pickle() const;
    void Unpickle(const std::vector<std::string> & strings);
    
    std::set<SrPattern *> GetPatterns() const;
    
private:
    void _WriteParamRecurse(const ofAbstractParameter & param,
                            ofParameterGroup & rootGroup,
                            const std::string & parentPath);
    
    void _OnTogglePressed(bool & value);
    
    SrPattern * _FindPattern(const std::string & str) const;
    
private:
    std::string _name;
    SrModel * _model;
    std::vector<std::string> _strings;
    SrSwitcher * _switcher;
    
    ofParameter<bool> _isCurrentPreset;
    ofxToggle _toggle;
};

#endif /* Preset_hpp */
