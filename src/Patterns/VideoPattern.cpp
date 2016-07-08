//
//  VideoPattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 5/31/16.
//
//

#include "VideoPattern.hpp"

SrVideoPattern::SrVideoPattern(const std::string & name,
                               SrModel * model,
                               SrAudio * audio) :
    SrPattern(name, model, audio),
    _video("fire_small.mov", OF_LOOP_NORMAL),
    _gateIndex(0)
{
    
}

SrVideoPattern::~SrVideoPattern()
{
    
}

void
SrVideoPattern::_Update()
{
    _video.GoToNextFrame();
    _gateIndex++;
    if (_gateIndex > GetModel()->GetNumGates()) {
        _gateIndex = 0;
    }
}

void
SrVideoPattern::_Draw() const
{
    if (not GetEnabled()[0]) {
        return;
    }
    
    ofPushMatrix();
    ofTranslate(_gateIndex, 0);
    _video.Draw();
    ofTranslate(-GetModel()->GetNumGates(), 0);
    _video.Draw();
    ofPopMatrix();
}