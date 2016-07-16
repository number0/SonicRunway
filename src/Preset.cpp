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

SrPreset::SrPreset(const std::string & name,
                   SrModel * model,
                   const std::string & fileName) :
    SrUiMixin(name),
    _model(model),
    _fileName(fileName)
{
    SrDebug("constructed preset\n");
    
    _applyButton.setup("Apply", 40, 40);
    _AddUI(&_applyButton);
    
    _saveButton.setup("Save", 40, 40);
    _AddUI(&_saveButton);
    
    _applyButton.addListener(this, &This::_OnApplyPressed);
    _saveButton.addListener(this, &This::_OnSavePressed);
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
    
    
    std::ifstream ifs(_fileName, std::ifstream::in);
    if (not ifs) {
        SrError("error opening file %s\n", _fileName.c_str());
        return;
    }
    
    std::string str;
    while (std::getline(ifs, str)) {
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
SrPreset::Save()
{
    SrDebug("saving preset\n");
    FILE *fp = fopen(_fileName.c_str(), "w");
    if (not fp) {
        SrError("error opening file %s\n", _fileName.c_str());
        return;
    }
    
    std::string path;
    _WriteParamRecurse(_model->GetParameterGroup(), path, fp);
   
    fclose(fp);
}

void
SrPreset::_WriteParamRecurse(const ofAbstractParameter & param,
                             const std::string & parentPath,
                             FILE * fp)
                
{
    const ofParameterGroup * group =
        dynamic_cast<const ofParameterGroup *>(&param);
    
    if (group) {
        for(auto iter = group->begin(); iter != group->end(); iter++) {
            auto childParam = *iter;
            
            std::string path = parentPath;
            path += "/" + group->getName();
            
            _WriteParamRecurse(*childParam, path, fp);
        }
    } else {
        fprintf(fp, "%s/%s: %s\n", parentPath.c_str(),
                param.getName().c_str(),
                param.toString().c_str());
    }
}

void
SrPreset::_OnApplyPressed()
{
    Apply();
}

void
SrPreset::_OnSavePressed()
{
    Save();
}
