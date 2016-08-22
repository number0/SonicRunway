//
//  AnimPattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/30/16.
//
//

#include "AnimPattern.hpp"

SrAnimPattern::SrAnimPattern(const std::string & name,
                             const std::string & path,
                             const std::string & baseFileName,
                             size_t numFrames,
                             bool padFrameNumbers,
                             SrModel * model,
                             SrAudio * audio,
                             SrGlobalParameters * globalParameters) :
    SrPattern(name, model, audio, globalParameters),
    _imageSequence(path, baseFileName, numFrames, padFrameNumbers),
    _currentFrame(0),
    _gateIndex(0),
    _scroll(false)
{
    _scroll.setName("Scroll");
    _AddUIParameter(_scroll);
    
    int width = model->GetNumGates(); // * model->GetFramesPerGate();
    int height = model->GetLightsPerGate();
    /*
    _curImg.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
    _curImg.setColor(ofColor::black);
     */
}

SrAnimPattern::~SrAnimPattern()
{
    
}

bool
SrAnimPattern::IsAudioReactive() const
{
    return false;
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

    /*
    const ofImage & img = _imageSequence.GetImage(_currentFrame);

    //_curImg.clear();
    //_curImg.clone(img);

    int newWidth = _curImg.getWidth();
    int newHeight = _curImg.getHeight();
    
    const ofFloatPixels & pixels = GetModel()->GetFloatPixels();
    int numGates = GetModel()->GetNumGates();
    int lightsPerGate = GetModel()->GetLightsPerGate();
    
    ofFloatColor blackPixel = ofFloatColor(1.0f,1.0f,1.0f,1.0f);
    
    for (int x=0; x < newWidth; x++) {
        for (int y=0; y < newHeight; y++) {
            int gateIdx = int(ofMap(x, 0, newWidth, 0, numGates));
            int lightIdx = int(ofMap(y, 0, newHeight, 0, lightsPerGate));
            ofFloatColor color = pixels.getColor(gateIdx, lightIdx);
            //_curImg.setColor(x, y, blackPixel);
            if(color.getBrightness() < 0.5f) {
                std::cout << "setting pixel " << x << " " << y << " to " << blackPixel << std::endl;
                std::cout << "brightness of " << color << " was " << color.getBrightness() << std::endl;
                _curImg.setColor(x, y, blackPixel);
            }
        }
    }
    
    _curImg.update();
    */
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