//
//  AnimPattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/30/16.
//
//

#include "AnimPattern.hpp"

SrAnimPattern::SrAnimPattern(const std::string & name,
                             const std::string & baseFileName,
                             size_t numFrames,
                             bool padFrameNumbers,
                             SrModel * model,
                             SrAudio * audio) :
    SrPattern(name, model, audio),
    _imageSequence(baseFileName, numFrames, padFrameNumbers),
    _currentFrame(0),
    _gateIndex(0),
    _scroll(false)
{
    _scroll.setName("Scroll");
    _AddUIParameter(_scroll);
}

SrAnimPattern::~SrAnimPattern()
{
    
}

void
SrAnimPattern::_Update()
{
    _currentFrame++;
    if (_currentFrame >= _imageSequence.GetNumFrames()) {
        _currentFrame = 0;
    }
    
    if (_scroll) {
        _gateIndex++;
        if (_gateIndex > GetModel()->GetNumGates()) {
            _gateIndex = 0;
        }
    }
}

void
SrAnimPattern::_Draw() const
{
    if (not GetEnabled()[0]) {
        return;
    }
    
    const ofImage & img = _imageSequence.GetImage(_currentFrame);
    printf("img size %f %f\n", img.getWidth(), img.getHeight());
    
    img.draw(_gateIndex, 0, img.getWidth(), GetModel()->GetLightsPerGate());
    img.draw(_gateIndex - GetModel()->GetNumGates(), 0);
}