//
//  VideoPattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 5/31/16.
//
//

#include "VideoPattern.hpp"

SrVideoPattern::SrVideoPattern(const std::string & name,
                               const std::string & videoFileName,
                               SrModel * model,
                               SrAudio * audio) :
    SrPattern(name, model, audio),
    _video(videoFileName, OF_LOOP_NORMAL),
    _gateIndex(0),
    _scroll(false)
{
    _scroll.setName("Scroll");
    _AddUIParameter(_scroll);
}

SrVideoPattern::~SrVideoPattern()
{
    
}

void
SrVideoPattern::_Update()
{
    _video.GoToNextFrame();
    if (_scroll) {
        _gateIndex++;
        if (_gateIndex > GetModel()->GetNumGates()) {
            _gateIndex = 0;
        }
    }
}

void
SrVideoPattern::_Draw() const
{
    float opacity = GetOpacity()[0];
    if (opacity <= 0.0) {
        return;
    }
    
    // Set a greyscale color according to 'opacity' to multiply the image
    ofSetColor(ofFloatColor(opacity, opacity, opacity));
    
    ofPushMatrix();
    ofTranslate(_gateIndex, 0);
    _video.Draw();
    ofTranslate(-GetModel()->GetNumGates(), 0);
    _video.Draw();
    ofPopMatrix();
}