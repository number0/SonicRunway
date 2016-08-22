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
    SrScrollingPattern(name, model, audio, globalParameters),
    _random(false)
{
    _random.setName("Random");
    _AddUIParameter(_random);
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
    
    if ( _random ) {
        int globalBeat = GetAudio()->GetBeatHistory().GetGlobalBeat()[0];
        int globalMeasure = (int)globalBeat / 8;
        
        int beatDiv = 16;
        int measureDiv = 8;
        
        srand(globalBeat);
        int beat = rand();
        
        int divs[] = {3,4,5,6,8,16,20};
        int choice = rand() % 7;
        
        beatDiv = divs[choice];
        
        beat = beat % beatDiv;
        
        //int r2 = rand()%6;
        //beatDiv = 2 + (r2 * 2);
        
        float beatNorm = (float) beat / beatDiv;
        float interval = 1.0 / beatDiv;
        
        srand(globalMeasure);
        int measure = rand() % 8;
        
        for(int lightIndex = 0; lightIndex < buffer->size(); lightIndex++) {
            
            float normalizedIndex = (float)lightIndex / (buffer->size()-1);
            ofFloatColor c;
            
            if ( normalizedIndex > beatNorm &&
                 normalizedIndex < beatNorm + interval) {
                
                c.setHsb(0.0, 1.0, 1.0);
                
            }
            else {
                c.setHsb(0.0, 0.0, 0.0);
            }
            
            (*buffer)[lightIndex] = c;
            
        }

        
    }
    
    else {
        
        float t = (float) beatIndex / 4;
        float prevT = (float) prevIndex / 4;
    
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
}