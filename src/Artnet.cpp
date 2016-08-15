//
//  Artnet.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 1/14/16.
//
//

#include "Artnet.hpp"
#include "Model.hpp"
#include "Debug.hpp"

SrArtnet::SrArtnet(const std::string & name,
                   SrModel * model) :
    SrUiMixin(name),
    _model(model),
    _enabledParam(true)
{
    _artnet.setup("192.168.0.1");
    _artnet.verbose = false;
    
    _enabledToggle.setup(_enabledParam);
    _enabledParam.setName("Enabled");
    _AddUIParameter(_enabledParam);
    
    // XXX would be better to read this from a file, so we can hack the
    // file to change IP addresses without having to recompile.
    //_ipAddresses.resize(_model->GetNumGates());
   
    
    _ipAddresses.resize(_model->GetNumGates());
    
    // There is no station 0.  adding address of spare 1
    _ipAddresses[0] = std::string("192.168.3.24");
    
    /*  ACTUAL ADDRESSES OF THE BOXES
     */
    _ipAddresses[1] = std::string("192.168.3.32");
    _ipAddresses[2] = std::string("192.168.3.11");
    _ipAddresses[3] = std::string("192.168.3.12");
    _ipAddresses[4] = std::string("192.168.3.28");
    _ipAddresses[5] = std::string("192.168.3.16");
    _ipAddresses[6] = std::string("192.168.3.202");
    _ipAddresses[7] = std::string("192.168.3.17");
    _ipAddresses[8] = std::string("192.168.3.29");
    _ipAddresses[9] = std::string("192.168.3.18");
    _ipAddresses[10] = std::string("192.168.3.27");
    _ipAddresses[11] = std::string("192.168.3.26");
    _ipAddresses[12] = std::string("192.168.3.14");
    _ipAddresses[13] = std::string("192.168.3.25");
    _ipAddresses[14] = std::string("192.168.3.10");
    _ipAddresses[15] = std::string("192.168.3.22");
    _ipAddresses[16] = std::string("192.168.3.21");
    _ipAddresses[17] = std::string("192.168.3.20");
    _ipAddresses[18] = std::string("192.168.3.15");
    _ipAddresses[19] = std::string("192.168.3.31");
    _ipAddresses[20] = std::string("192.168.3.35");
    _ipAddresses[21] = std::string("192.168.3.36");
    _ipAddresses[22] = std::string("192.168.3.37");
    _ipAddresses[23] = std::string("192.168.3.38");
    _ipAddresses[24] = std::string("192.168.3.19");
    _ipAddresses[25] = std::string("192.168.3.33");
    _ipAddresses[26] = std::string("192.168.3.23");
    _ipAddresses[27] = std::string("192.168.3.34");
    _ipAddresses[28] = std::string("192.168.3.39");
    _ipAddresses[29] = std::string("192.168.3.40");
    _ipAddresses[30] = std::string("192.168.3.41");
    _ipAddresses[31] = std::string("192.168.3.42");
    _ipAddresses[32] = std::string("192.168.3.44");
}

void
SrArtnet::UpdateLights()
{
    // XXX disabling this check because the Lumigeek boards don't
    // correctly respond to artnet discovery.  Just go ahead and blast
    // the signal anyway :)
    /*
    if (_artnet.status != NODES_FOUND) {
        SrWarn("status != NODES_FOUND\n");
        return;
    }
     */
    
    if (not (bool) _enabledParam) {
        return;
    }
    
    for(size_t i=0; i < _model->GetNumGates(); i++) {
        _BroadcastToGate(i);
    }
}

void
SrArtnet::_BroadcastToGate(size_t gateIdx)
{
    if (gateIdx >= _ipAddresses.size()) {
        SrError("IP address %zu out of range\n", gateIdx);
        return;
    }
   
    const std::string & ipAddress = _ipAddresses[gateIdx];
    if (ipAddress.empty()) {
        // XXX fail silently so we don't spew.
        return;
    }
    
    std::vector<unsigned char> data(512 * 3);
    
    const ofFloatPixels & pixels = _model->GetFloatPixels();
    if (not pixels.isAllocated()) {
        SrError("pixels not allocated\n");
        return;
    }
    
    // XXX probably a memcpy way to do this faster..
    for (int i=0; i < _model->GetLightsPerGate(); i++) {
        ofFloatColor color = pixels.getColor(gateIdx, i);
      
        // TMP
        //color = ofFloatColor::white;
        
        // XXX hacking gamma correction by squaring the
        // brightness.  Clearly there is a more accurate way...
        color.setHsb(color.getHue(), color.getSaturation(),
                     color.getBrightness() * color.getBrightness());
        
        // XXX switch red and green b/c of the particular LEDs we have.
        data[i*3] = color[1] * 255;
        data[i*3 + 1] = color[0] * 255;
        data[i*3 + 2] = color[2] * 255;
    }
    
    _artnet.sendDmx(ipAddress, &data[0], 512);
}

SrArtnet::~SrArtnet()
{
    // XXX cleanup fbo??
    // Stop artnet?
    
    /*
    _artnet.stop();
    _artnet.close();
     */
}
