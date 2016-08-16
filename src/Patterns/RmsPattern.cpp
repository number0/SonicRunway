//
//  RmsPattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/14/16.
//
//

#include "RmsPattern.hpp"
#include "Audio.hpp"

SrRmsPattern::SrRmsPattern(const std::string & name,
                           SrModel * model, SrAudio * audio,
                           SrGlobalParameters * globalParameters) :
    SrScrollingPattern(name, model, audio, globalParameters)
{
    
}

SrRmsPattern::~SrRmsPattern()
{
    
}

void
SrRmsPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    float low = GetAudio()->GetLows()[0];
    float mid = GetAudio()->GetMids()[0];
    float high = GetAudio()->GetHighs()[0];
    
    ofFloatColor lowColor(1.0, 0.0, 0.0);
    ofFloatColor midColor(0.0, 1.0, 0.0);
    ofFloatColor highColor(0.0, 0.0, 1.0);
    
    // Square the values to accentuate the peaks
    low *= low;
    mid *= mid;
    high *= high;
    
    int numSegments = 3;
    size_t segment0Start = 0;
    size_t segment1Start = buffer->size() / numSegments;
    size_t segment2Start = buffer->size() / numSegments * 2;
    
    size_t numLowLeds = low * buffer->size() / numSegments;
    size_t numMidLeds = mid * buffer->size() / numSegments;
    size_t numHighLeds = high * buffer->size() / numSegments;
    
    size_t fullCircleClamp = 4.0 / 3.0 * buffer->size();
    
    for(size_t i=0; i < numLowLeds; i++) {
        size_t idx = i + segment0Start;
        idx %= buffer->size();
        (*buffer)[idx] = lowColor;
    }
    
    for(size_t i=0; i < numMidLeds; i++) {
        int idx = i + segment1Start;
        idx %= buffer->size();
        (*buffer)[idx] = midColor;
    }
    
    for(size_t i=0; i < numHighLeds; i++) {
        int idx = i + segment2Start;
        idx %= buffer->size();
        (*buffer)[idx] = highColor;
    }
    
}