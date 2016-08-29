//
//  DropoutPattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/9/16.
//
//

#include "DropoutPattern.hpp"
#include "Audio.hpp"
#include "GlobalParameters.hpp"
#include "Util.hpp"

SrDropoutPattern::SrDropoutPattern(const std::string & name,
                                   SrModel * model, SrAudio * audio,
                                   SrGlobalParameters * globalParameters) :
SrScrollingPattern(name, model, audio, globalParameters),
_dropoutParam(false),
_dropoutWasPressed(false),
_trailLength(10),
_framesSinceDropout(_trailLength)
{
    _dropoutParam.setName("Dropout");
    _AddUIParameter(_dropoutParam);
    SrScrollingPattern::SetIsMask(true);
    SrScrollingPattern::SetIsInvertMask(true);
}

SrDropoutPattern::~SrDropoutPattern()
{
    
}

bool
SrDropoutPattern::IsAudioReactive() const
{
    return false;
}

void
SrDropoutPattern::_Update()
{
    SrScrollingPattern::_Update();
}

void
SrDropoutPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    //if (_framesSinceDropout >= _trailLength) {
    //    return;
    //}
    ofFloatColor color;
    if ( _dropoutParam ) {
        color.set(1.0, 1.0, 1.0);
    }
    else {
        color.set(0.0, 0.0, 0.0);
    }
    
    //SrGlobalParameters * globals = GetGlobalParameters();
    //float hue = globals->GetVerySlowCycle();
    //hue = SrUtil_ClampCycle(0, 1, hue);
    //float valueT = (float) _framesSinceDropout / _trailLength;
    //float value = 1.0 - valueT;
    
    for(int i = 0; i < buffer->size(); i++) {
        (*buffer)[i] = color;
    }
}
