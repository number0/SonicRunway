//
//  PhrasePattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/9/16.
//
//

#include "PhrasePattern.hpp"
#include "Audio.hpp"
#include "BeatHistory.hpp"

SrPhrasePattern::SrPhrasePattern(const std::string & name,
                                 SrModel * model,
                                 SrAudio * audio,
                                 SrGlobalParameters * globalParameters) :
    SrScrollingPattern(name, model, audio, globalParameters)
{
    
}

SrPhrasePattern::~SrPhrasePattern()
{
    
}

void
SrPhrasePattern::_Update()
{
    SrScrollingPattern::_Update();
}

void
SrPhrasePattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    int beatIndex = GetAudio()->GetBeatHistory().GetBeatIndex()[0];
    int prevIndex = beatIndex - 1;
    
    float t = (float) beatIndex / 5;
    float prevT = (float) prevIndex / 5;
    
    int ledIdx = GetModel()->GetLightsPerGate() * t;
    int prevLedIdx = GetModel()->GetLightsPerGate() * prevT;
    
    ofFloatColor c;
    c.setHsb(0.0, 1.0, 1.0);
    
    for(int i = prevLedIdx; i < ledIdx; i++ ) {
        // Make sure we don't go beyond the array.  Just in case.
        if (i < 0 or i >= GetModel()->GetLightsPerGate()) {
            continue;
        }
        
        (*buffer)[i] = c;
    }
}