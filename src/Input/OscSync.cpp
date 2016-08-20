//
//  OscSync.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 8/20/16.
//
//

#include "OscSync.hpp"
#include "Model.hpp"
#include "Util.hpp"
#include "Debug.hpp"
#include "Switcher.hpp"

static const std::string _presetPrefix = std::string("/presets/preset.multitoggle/");
static const int _presetGridWidth = 5;
static const int _presetGridHeight = 6;

SrOscSync::SrOscSync(SrModel * model, SrSwitcher * switcher,
                     int localPort,
                     const std::string& host, int remotePort) :
    _model(model),
    _switcher(switcher),
    _updatingParameter(false),
    _syncGroup(model->GetParameterGroup())
{
    ofAddListener(_syncGroup.parameterChangedE(), this,
                  &SrOscSync::_OnParameterChanged);
    _sender.setup(host,remotePort);
    _receiver.setup(localPort);
    
}

SrOscSync::~SrOscSync()
{
    ofRemoveListener(_syncGroup.parameterChangedE(), this,
                     &SrOscSync::_OnParameterChanged);
}

static size_t
_GetPresetIndex(ofxOscMessage & msg)
{
    std::vector<std::string> address = ofSplitString(msg.getAddress(),"/",true);
    if (address.size() != 4) {
        SrError("OSC Preset message not the right size");
        return;
    }
    
    int x = atoi(address[3].c_str());
    int y = atoi(address[2].c_str());
    
    x = x - 1;
    y = _presetGridHeight - y;
    
    int idx = y * _presetGridWidth + x;
    
    return idx;
}

void
SrOscSync::Update()
{
    ofxOscMessage m;
    
    while (_receiver.getNextMessage(m)) {
        ofAbstractParameter *parameter =
        SrUtil_FindParameter(_model->GetParameterGroup(), m.getAddress());
        if (parameter) {
            _SyncParameterToMessage(parameter, m);
            continue;
        }
        
        if (m.getAddress().compare(0, _presetPrefix.size(), _presetPrefix) == 0) {
            if (m.getArgType(0) != OFXOSC_TYPE_FLOAT) {
                SrError("OSC preset not a float");
                continue;
            }
            
            float value = m.getArgAsFloat(0);
            if (value == 1.0) {
                size_t presetIndex = _GetPresetIndex(m);
                _switcher->ApplyPresetAtIndex(presetIndex);
            }
        }
        
        // XXX Handle other params here..
    }
}

void
SrOscSync::_OnParameterChanged(ofAbstractParameter & parameter)
{
    if(_updatingParameter) {
        return;
    }
    
    _sender.sendParameter(parameter);
}

// Copied from ofxOscParameterSync
void
SrOscSync::_SyncParameterToMessage(ofAbstractParameter *p, ofxOscMessage & msg)
{
    std::vector<std::string> address = ofSplitString(msg.getAddress(),"/",true);
    for(unsigned int i=0;i<address.size();i++){
        if(p) {
            if(address[i]==p->getEscapedName()){
                if(p->type()==typeid(ofParameterGroup).name()){
                    if(address.size()>=i+1){
                        p = &static_cast<ofParameterGroup*>(p)->get(address[i+1]);
                    }
                }else if(p->type()==typeid(ofParameter<int>).name() && msg.getArgType(0)==OFXOSC_TYPE_INT32){
                    p->cast<int>() = msg.getArgAsInt32(0);
                }else if(p->type()==typeid(ofParameter<float>).name() && msg.getArgType(0)==OFXOSC_TYPE_FLOAT){
                    p->cast<float>() = msg.getArgAsFloat(0);
                }else if(p->type()==typeid(ofParameter<double>).name() && msg.getArgType(0)==OFXOSC_TYPE_DOUBLE){
                    p->cast<double>() = msg.getArgAsDouble(0);
                }else if(p->type()==typeid(ofParameter<bool>).name() && (msg.getArgType(0)==OFXOSC_TYPE_TRUE
                                                                         || msg.getArgType(0)==OFXOSC_TYPE_FALSE
                                                                         || msg.getArgType(0)==OFXOSC_TYPE_INT32
                                                                         || msg.getArgType(0)==OFXOSC_TYPE_INT64
                                                                         || msg.getArgType(0)==OFXOSC_TYPE_FLOAT
                                                                         || msg.getArgType(0)==OFXOSC_TYPE_DOUBLE
                                                                         || msg.getArgType(0)==OFXOSC_TYPE_STRING
                                                                         || msg.getArgType(0)==OFXOSC_TYPE_SYMBOL)){
                    p->cast<bool>() = msg.getArgAsBool(0);
                }else if(msg.getArgType(0)==OFXOSC_TYPE_STRING){
                    p->fromString(msg.getArgAsString(0));
                }
            }
        }
    }
}
