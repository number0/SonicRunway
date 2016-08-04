//
//  Preset.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/9/16.
//
//

#include "Preset.hpp"
#include "Model.hpp"
#include "Util.hpp"
#include "Debug.hpp"
#include "Switcher.hpp"

SrPreset::SrPreset(const std::string & name,
                   SrModel * model,
                   SrSwitcher * switcher) :
    SrUiMixin(name),
    _model(model),
    _switcher(switcher)
{
    SrDebug("constructed preset\n");
    
    _applyButton.setup("Apply", 40, 40);
    _AddUI(&_applyButton);
    
    _storeButton.setup("Store", 40, 40);
    _AddUI(&_storeButton);
    
    _applyButton.addListener(this, &This::_OnApplyPressed);
    _storeButton.addListener(this, &This::_OnStorePressed);
}

SrPreset::~SrPreset()
{
    SrDebug("destroyed preset\n");
    
}

void
SrPreset::Apply() const
{
    SrDebug("applying preset\n");
    ofParameterGroup & parameterGroup =
        _model->GetParameterGroup();
    
    for(size_t i = 0; i < _strings.size(); i++) {
        const std::string str = _strings[i];
        
        std::vector<std::string> strVec =
            SrUtil_SplitString(str, ':');
        if (strVec.size() != 2) {
            SrError("error parsing line: %s\n", str.c_str());
            continue;
        }
        
        const std::string & pathStr = strVec[0];
        const std::string & valStr = strVec[1];
        
        ofAbstractParameter * param =
            SrUtil_FindParameter(parameterGroup, pathStr);
        
        if (not param) {
            SrWarn("couldn't find parameter %s\n", pathStr.c_str());
            continue;
        }
        
        SrDebug("found parameter %s!\n", pathStr.c_str());
        param->fromString(valStr);
    }
}

void
SrPreset::Store()
{
    SrDebug("storing preset\n");
    
    _strings.clear();
    
    std::string path;
    _WriteParamRecurse(_model->GetParameterGroup(),
                       _model->GetParameterGroup(), path);
}

void
SrPreset::_WriteParamRecurse(const ofAbstractParameter & param,
                             ofParameterGroup & rootGroup,
                             const std::string & parentPath)
{
    const ofParameterGroup * group =
        dynamic_cast<const ofParameterGroup *>(&param);
    
    if (group) {
        for(auto iter = group->begin(); iter != group->end(); iter++) {
            auto childParam = *iter;
            
            std::string path = parentPath;
            path += "/" + group->getName();
            
            _WriteParamRecurse(*childParam, rootGroup, path);
        }
    } else {
        if (SrUtil_IsPathToEnabledPattern(parentPath, rootGroup)) {
            char *str;
            asprintf(&str, "%s/%s: %s\n",
                     parentPath.c_str(),
                     param.getName().c_str(),
                     param.toString().c_str());
            
            _strings.push_back(std::string(str));
            
            free(str);
        }
    }
}

void
SrPreset::_OnApplyPressed()
{
    _switcher->OnPresetApplyClicked(this);
}

void
SrPreset::_OnStorePressed()
{
    Store();
}

const std::vector<std::string> &
SrPreset::Pickle() const
{
    return _strings;
}

void
SrPreset::Unpickle(const std::vector<std::string> & strings)
{
    _strings = strings;
}