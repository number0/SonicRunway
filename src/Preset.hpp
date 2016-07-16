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

//
// A group of parameter settings that can be applied
// to the app
//
// XXX in progress!
//
class SrPreset : public SrUiMixin {
    typedef SrPreset This;
    
public:
    SrPreset(const std::string & name,
             SrModel * model,
             const std::string & fileName);
    ~SrPreset();
    
    void Apply() const;
    void Save();
    
private:
    void _OnApplyPressed();
    void _OnSavePressed();
    
    void _WriteParamRecurse(const ofAbstractParameter & param,
                            const std::string & parentPath,
                            FILE *fp);
    
private:
    SrModel * _model;
    std::string _fileName;
    ofxButton _applyButton;
    ofxButton _saveButton;
    
    ofParameter<bool> _applyParam;
    ofParameter<bool> _saveParam;
};

#endif /* Preset_hpp */
