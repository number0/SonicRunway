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
                             SrAudio * audio,
                             SrGlobalParameters * globalParameters) :
    SrPattern(name, model, audio, globalParameters),
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
    // For this pattern, we just use the opacity at the 0th gate.
    float opacity = GetOpacity()[0];
    if (opacity <= 0.0) {
        return;
    }
    
    const ofImage & img = _imageSequence.GetImage(_currentFrame);
    
    ofSetColor(ofFloatColor(opacity, opacity, opacity));
    
    img.draw(_gateIndex, 0, img.getWidth(), GetModel()->GetLightsPerGate());
    img.draw(_gateIndex - GetModel()->GetNumGates(), 0);
}