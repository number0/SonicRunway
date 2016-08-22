//
//  OscSync.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 8/20/16.
//
//

#include "OscSync.hpp"
#include "Model.hpp"
#include "Audio.hpp"
#include "Util.hpp"
#include "Debug.hpp"
#include "Switcher.hpp"
#include "Preset.hpp"
#include "Pattern.hpp"

static const std::string _presetPrefix = std::string("/presets/preset.multitoggle/");
static const int _presetGridWidth = 5;
static const int _presetGridHeight = 6;

SrOscSync::SrOscSync(SrModel * model, SrAudio * audio,
                     SrSwitcher * switcher,
                     int localPort,
                     const std::string& remoteHost, int remotePort) :
    _model(model),
    _audio(audio),
    _switcher(switcher),
    _updatingParameter(false),
    _syncGroup(model->GetParameterGroup()),
    _localPort(localPort),
    _remoteHost(remoteHost),
    _remotePort(remotePort)
{
    ofAddListener(_syncGroup.parameterChangedE(), this,
                  &SrOscSync::_OnParameterChanged);
    _sender.setup(remoteHost,remotePort);
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
        if (m.getRemoteIp() != _remoteHost) {
            _remoteHost = m.getRemoteIp();
            _sender.setup(_remoteHost, _remotePort);
        }
        
        // If the path corresponds to one of our parameters, sync the value
        ofAbstractParameter *parameter =
        SrUtil_FindParameter(_model->GetParameterGroup(), m.getAddress());
        if (parameter) {
            _SyncParameterToMessage(parameter, m);
            continue;
        }
        
        // If it's from the special grid of 'presets', apply the preset
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
    
    _BroadcastAudioValues();
}

void
SrOscSync::_SendFloatMessage(const std::string & path, float value)
{
    ofxOscMessage m;
    m.setAddress(path);
    m.addFloatArg(value);
    _sender.sendMessage(m);
}

void
SrOscSync::_SendStringMessage(const std::string & path, const std::string & value)
{
    ofxOscMessage m;
    m.setAddress(path);
    m.addStringArg(value);
    _sender.sendMessage(m);
}

void
SrOscSync::_BroadcastAudioValues()
{
    _SendFloatMessage("/Runway/Audio/Lows", _audio->GetLows()[0]);
    _SendFloatMessage("/Runway/Audio/Mids", _audio->GetMids()[0]);
    _SendFloatMessage("/Runway/Audio/Highs", _audio->GetHighs()[0]);
}

void
SrOscSync::_BroadcastGlobalParameterLabel(SrPreset * preset,
                                         const std::string & name)
{
    if (not preset) {
        return;
    }
    
    std::string path("/Runway/GlobalParams/");
    path += name;
    path += "/Label";
    
    std::string value;
    std::set<SrPattern *> patterns = preset->GetPatterns();
    
    for(auto iter = patterns.begin(); iter != patterns.end(); iter++) {
        std::string thisLabel = (*iter)->GetGlobalParameterLabel(name);
        if (not thisLabel.empty()) {
            value += thisLabel;
        }
    }
    
    _SendStringMessage(path, value);
}

void
SrOscSync::BroadcastPresetInfo(SrPreset * preset)
{
    _SendStringMessage("/Runway/CurrentPresetName", preset->GetName());
    _SendFloatMessage("/Runway/IsPresetAudioReactive", (float) preset->IsAudioReactive());
    
    _BroadcastGlobalParameterLabel(preset, "Dial1");
    _BroadcastGlobalParameterLabel(preset, "Dial2");
    _BroadcastGlobalParameterLabel(preset, "Dial3");
    _BroadcastGlobalParameterLabel(preset, "Slider1");
    _BroadcastGlobalParameterLabel(preset, "Slider2");
}

void
SrOscSync::_OnParameterChanged(ofAbstractParameter & parameter)
{
    if(_updatingParameter) {
        return;
    }
    
    /*
    _sender.setup(_remoteHost, 9000);
    _sender.enableBroadcast();
    _sender.sendParameter(parameter);
     */
    
    std::string path = SrUtil_GetParameterPath(parameter);
    
    ofxOscMessage m;
    m.setAddress(path);
    
    if (parameter.type()==typeid(ofParameter<float>).name()) {
        m.addFloatArg(parameter.cast<float>());
    }
    
    _sender.sendMessage(m);
}

// Copied from ofxOscParameterSync
void
SrOscSync::_SyncParameterToMessage(ofAbstractParameter *p, ofxOscMessage & msg)
{
    _updatingParameter = true;
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
    _updatingParameter = false;
}
