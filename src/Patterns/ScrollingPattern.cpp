//
//  ScrollingPattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 5/5/16.
//
//

#include "ScrollingPattern.hpp"

SrScrollingPattern::SrScrollingPattern(const std::string & name,
                                   SrModel * model, SrAudio * audio) :
    SrPattern(name, model, audio),
    _index(0)
{
    int width = model->GetNumGates() * model->GetFramesPerGate();
    int height = model->GetLightsPerGate();
    int numChannels = 4;
    
    _image.allocate(width, height, OF_IMAGE_COLOR_ALPHA);
    _image.setColor(ofColor::black);
}

SrScrollingPattern::~SrScrollingPattern()
{
    
}

void
SrScrollingPattern::_Update()
{
    _index--;
    if (_index < 0) {
        _index = (int) _image.getWidth() - 1;
    }
    
    if (not IsOnAtAnyGate()) {
        return;
    }
    
    // Draw the current gate into a buffer
    std::vector<ofColor> currentColors(GetModel()->GetLightsPerGate());
    for(int i = 0; i < GetModel()->GetLightsPerGate(); i++) {
        currentColors[i] = ofColor::black;
    }
    
    _DrawCurrentGate(&currentColors);
    
    // Copy the colors to our pixel cache
    for(int i = 0; i < GetModel()->GetLightsPerGate(); i++) {
        ofColor color = currentColors[i];
       
        float opacity = GetOpacity()[0];
        
        color *= opacity;
        
        _image.setColor(_index, i, color);
    }
    
    _image.update();
}

void
SrScrollingPattern::_Draw() const
{
    if (not IsOnAtAnyGate()) {
        return;
    }
    
    ofSetColor(ofColor::white);
    _image.draw(-_index,0);
    _image.draw(-_index + GetModel()->GetNumGates(),0);
}
