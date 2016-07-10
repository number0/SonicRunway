//
//  Preset.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/9/16.
//
//

#include "Preset.hpp"

SrPreset::SrPreset(const std::string & name,
                   const std::string & file) :
    SrUiMixin(name),
    _applyParam(false)
{
    printf("constructed preset\n");
    
}

SrPreset::~SrPreset()
{
    printf("destroyed preset\n");
    
}

void
SrPreset::Apply() const
{
    printf("applying preset\n");
}

void
SrPreset::Save()
{
    
}