//
//  ScrollingPattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 5/5/16.
//
//

#include "ScrollingPattern.hpp"

SrScrollingPattern::SrScrollingPattern(const std::string & name,
                                       SrModel * model, SrAudio * audio,
                                       SrGlobalParameters * globalParameters) :
    SrPattern(name, model, audio, globalParameters),
    _index(0),
    _mask(false),
    _invertMask(false),
    _maskWasOn(false),
    _reverse(false)
{
    _mask.setName("IsMask");
    _AddUIParameter(_mask);
    
    _invertMask.setName("InvertMask");
    _AddUIParameter(_invertMask);
    
    _reverse.setName("Reverse");
    _AddUIParameter(_reverse);
    
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
    if(_maskWasOn && (!_mask)) {
        _image.setColor(ofColor::black);
    }
    _maskWasOn = _mask;
    
    if ( _reverse ){
        _index++;
        if(_index > _image.getWidth() - 1){
            _index = 0;
        }
    }
    else {
        _index--;
        if (_index < 0) {
            _index = (int) _image.getWidth() - 1;
        }
     
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

        if(_mask) {
            float b = color.getBrightness()/255.0;
            if (_invertMask) {
                b = 1.0f-b;
            } else {
                b = pow(b, 0.2);
            }
            // if we're a mask, "fade out" means go to 1.0.
            b = ofMap(b, 0.0f, 1.0f, (1.0f-opacity), 1.0f);
            color = ofColor(b*255.0,b*255.0,b*255.0);
        } else {
            color *= opacity;
        }
        
        _image.setColor(_index, i, color);
    }
    
    _image.update();
}

bool
SrScrollingPattern::_FlipLeftRight() const
{
    return false;
}

void
SrScrollingPattern::_Draw() const
{
    if (not IsOnAtAnyGate()) {
        return;
    }
    
    
    if(_mask) {
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    }
    
    /*
     NOTE: below is mtf's masking that I can't get to
     xfade. -jdn
     */
    
    /*
    if(_mask && !_invertMask) {
        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    
    if(_invertMask) {
        ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
    }
    */
    
    ofPushMatrix();
    
    if (_FlipLeftRight()) {
        ofScale(1,-1);
        ofTranslate(0, -GetModel()->GetLightsPerGate());
    }
    
    ofSetColor(ofColor::white);
    _image.draw(-_index,0);
    _image.draw(-_index + GetModel()->GetNumGates(),0);
    
    ofPopMatrix();
    
    if (_mask || _invertMask) {
        ofEnableBlendMode(OF_BLENDMODE_ADD);
    }
}