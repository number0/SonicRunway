//
//  OscSync.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 8/20/16.
//
//

#ifndef SR_OSC_SYNC_HPP
#define SR_OSC_SYNC_HPP

#include <stdio.h>

#include "ofxOscSender.h"
#include "ofxOscReceiver.h"
#include "ofParameter.h"
#include "ofParameterGroup.h"

class SrModel;
class SrSwitcher;

class SrOscSync {
public:
    SrOscSync(SrModel * model, SrSwitcher * switcher, int localPort,
              const std::string & remoteHost, int remotePort);
    ~SrOscSync();
    
    void Update();
    
private:
    void _OnParameterChanged( ofAbstractParameter & parameter );
    void _SyncParameterToMessage(ofAbstractParameter *p, ofxOscMessage & msg);
    
private:
    SrModel * _model;
    SrSwitcher * _switcher;
    
    ofxOscSender _sender;
    ofxOscReceiver _receiver;
    ofParameterGroup _syncGroup;
    bool _updatingParameter;
};

#endif 
