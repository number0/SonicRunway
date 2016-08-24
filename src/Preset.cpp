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
#include "Pattern.hpp"

SrPreset::SrPreset(const std::string & name,
                   SrModel * model,
                   SrSwitcher * switcher) :
    _name(name),
    _model(model),
    _switcher(switcher),
    _isCurrentPreset(false)
{
    _toggle.setup(_isCurrentPreset);
    _toggle.setName(name);
    _toggle.addListener(this, &This::_OnTogglePressed);
}

SrPreset::~SrPreset()
{
    SrDebug("destroyed preset\n");
    
}

const std::string &
SrPreset::GetName() const
{
    return _name;
}

ofxToggle *
SrPreset::GetToggle()
{
    return &_toggle;
}

bool
SrPreset::IsCurrentPreset() const
{
    return (bool) _isCurrentPreset;
}

void
SrPreset::SetIsCurrentPreset(bool isCurrentPreset)
{
    _isCurrentPreset = isCurrentPreset;
}

void
SrPreset::Apply() const
{
    SrDebug("applying preset\n");
    ofParameterGroup & parameterGroup =
        _model->GetParameterGroup();
    
    for(size_t i = 0; i < _strings.size(); i++) {
        const std::string & str = _strings[i];
        
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
            printf("couldn't find parameter %s\n", pathStr.c_str());
            continue;
        }
        
        SrDebug("found parameter %s!\n", pathStr.c_str());
        param->fromString(valStr);
    }
}

SrPattern *
SrPreset::_FindPattern(const std::string & str) const
{
    std::vector<std::string> splitStr = SrUtil_SplitString(str, '/');
    if (splitStr.size() < 4) {
        return NULL;
    }
    
    // XXX making some lazy assumptions about the pattern string
    return _switcher->FindPatternByName(splitStr[3]);
}

bool
SrPreset::IsAudioReactive() const
{
    for(size_t i = 0; i < _strings.size(); i++) {
        const std::string & str = _strings[i];
        
        SrPattern * pattern = _FindPattern(_strings[i]);
        
        if (pattern and pattern->IsAudioReactive()) {
            return true;
        }
    }
    return false;
}

std::set<SrPattern *>
SrPreset::GetPatterns() const
{
    std::set<SrPattern *> ret;
    
    for(size_t i = 0; i < _strings.size(); i++) {
        const std::string & str = _strings[i];
        
        SrPattern * pattern = _FindPattern(_strings[i]);
        
        if (pattern) {
            ret.insert(pattern);
        }
    }
    
    return ret;
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

const std::vector<std::string> &
SrPreset::Pickle() const
{
    return _strings;
}

void
SrPreset::Unpickle(const std::vector<std::string> & strings)
{
    _strings = strings;
    
    if (IsAudioReactive()) {
        _toggle.setName(_name + " (A)");
    } else {
        _toggle.setName(_name + " ( )");
    }
}

void
SrPreset::_OnTogglePressed(bool & value)
{
    if (value) {
        _switcher->OnPresetTogglePressed(this);
    }
}

int
SrPreset::GetIndex() const
{
    return _switcher->GetPresetIndex(this);
}