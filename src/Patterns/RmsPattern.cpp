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
                           SrModel * model, SrAudio * audio) :
    SrScrollingPattern(name, model, audio)
{
    
}

SrRmsPattern::~SrRmsPattern()
{
    
}

void
SrRmsPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    float rmsValue = GetAudio()->GetLowRMS()[0];
    float t = rmsValue / (GetModel()->GetNumGates() - 1);
    t *= 500;
    t *= t;
    int rmsIdx = t * buffer->size();
    
    // double check range
    rmsIdx = rmsIdx % buffer->size();
   
    ofFloatColor c(1.0, 1.0, 1.0);
    for(int i = 0; i < rmsIdx; i++) {
        (*buffer)[i] = c;
    }
}