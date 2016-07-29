//
//  Artnet.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 1/14/16.
//
//

#ifndef SR_ARTNET_HPP
#define SR_ARTNET_HPP

#include <stdio.h>
#include "ofxArtnet.hpp"
#include "ofMain.h"
#include "UiMixin.hpp"

class SrModel;

//
// Artnet output - translates the current buffer into Artnet/DMX signals
// to update the LED colors.
//
class SrArtnet : public SrUiMixin {
public:
    SrArtnet(const std::string & name, SrModel * model);
    ~SrArtnet();
    
    void UpdateLights();
    
private:
    void _BroadcastToGate(size_t gateIdx);
    
private:
    SrModel * _model;
    ofParameter<bool> _enabledParam;
    ofxToggle _enabledToggle;
    
    ofxArtnet _artnet;
    ofImage _testImage;
    ofFbo _fbo;
    
    std::vector<std::string> _ipAddresses;
};

#endif
