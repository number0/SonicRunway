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

//
// A group of parameter settings that can be applied
// to the app
//
// XXX in progress!
//

class SrPreset : public SrUiMixin {
public:
    SrPreset(const std::string & name, const std::string & file);
    ~SrPreset();
    
    void Apply() const;
    void Save();
    
private:
    ofParameter<bool> _applyParam;
    
};

#endif /* Preset_hpp */
