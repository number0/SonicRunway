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
    _enabledParam(false)
{
    _artnet.setup("192.168.0.1");
    _artnet.verbose = false;
    
    _enabledToggle.setup(_enabledParam);
    _enabledParam.setName("Enabled");
    _AddUIParameter(_enabledParam);
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
    
    std::vector<unsigned char> data(512 * 3);
    
    const ofFloatPixels & pixels = _model->GetFloatPixels();
    if (not pixels.isAllocated()) {
        SrError("pixels not allocated\n");
        return;
    }
    
    for (int i=0; i < _model->GetLightsPerGate(); i++) {
        ofFloatColor color = pixels.getColor(0, i);
      
        // TMP
        //color = ofFloatColor::white;
        
        // XXX hacking gamma correction by squaring the
        // brightness.  Clearly there is a more accurate way...
        color.setHsb(color.getHue(), color.getSaturation(),
                     color.getBrightness() * color.getBrightness());
        data[i*3] = color[0] * 255;
        data[i*3 + 1] = color[1] * 255;
        data[i*3 + 2] = color[2] * 255;
    }
    
    //_artnet.sendDmx("192.168.0.50", &data[0], 512);
    _artnet.sendDmx("192.168.3.202", &data[0], 512);
    
    //_artnet.sendDmx("192.168.0.51", &data[0], 512);
    //_artnet.sendDmx("192.168.3.202", 0 /* subnet */, 0 /* universe */, &data[0], 512);
    
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
