//
//  BigTrailsPattern.cpp
//  SonicRunway
//
//  Created by Paul Roales on 7/17/16.
//
//

#include "BigTrailsPattern.hpp"
#include "Audio.hpp"
#include "GlobalParameters.hpp"

SrBigTrailsPattern::SrBigTrailsPattern(const std::string & name,
                                 SrModel * model, SrAudio * audio,
                                       SrGlobalParameters * globalParameters) :
    SrScrollingPattern(name, model, audio, globalParameters),
    _average(0.0)
{

}

SrBigTrailsPattern::~SrBigTrailsPattern()
{
    // Uses GlobalParams like so:
    // Dial1 - Hue 1
    // Dial2 - Jitter (Dial2 * 100)
    // Dial3 - Rotation (Dial3 * 100 - 50)
    // Slider1 - Saturation
    // Slider2 - Brightness
}

void
SrBigTrailsPattern::_Update()
{
    SrScrollingPattern::_Update();
    
    float low = GetAudio()->GetLows()[0];
    float mid = GetAudio()->GetMids()[0];
    float high = GetAudio()->GetHighs()[0];
    
    float jitter;
    if (GetGlobalParameters()->GetCycleAutomatically()) {
        jitter = 20;
    } else {
        jitter = GetGlobalParameters()->GetDial2() * 100;
    }
    
    low *= jitter;
    mid *= jitter;
    high *= jitter;
    
    // Rolling average replacing 5% each time.
    _average = (_average * .95) + (.05 * (low + mid + high));
}

void
SrBigTrailsPattern::_DrawCurrentGate(std::vector<ofColor> * buffer) const
{
    int bufferSize = buffer->size();

    float hue;
    float saturation;
    float brigthness;
    float rotation;
    float jitter;
    if (GetGlobalParameters()->GetCycleAutomatically()) {
        hue = GetGlobalParameters()->GetVerySlowCycle();
        saturation = 0.9;
        brigthness = 0.7;
        rotation = 12;
        jitter = 20;
    } else {
        hue = GetGlobalParameters()->GetDial1();
        jitter = GetGlobalParameters()->GetDial2() * 100;
        rotation = GetGlobalParameters()->GetDial3() * 100 - 50;
        saturation = GetGlobalParameters()->GetSlider1();
        brigthness = GetGlobalParameters()->GetSlider2();
    }
    
    float low = GetAudio()->GetLows()[0];
    float mid = GetAudio()->GetMids()[0];
    float high = GetAudio()->GetHighs()[0];
    
    // Square the values to accentuate the peaks
    low *= jitter;
    mid *= jitter;
    high *= jitter;
    
    int manualAdjustment = rotation;
    int index1 = bufferSize * 0.19 + low - _average + manualAdjustment;
    int index2 = bufferSize * 0.5 + mid - _average + manualAdjustment;
    int index3 = bufferSize * 0.81 + high - _average + manualAdjustment;
    
    ofFloatColor color;
    color.setHsb(hue, saturation, brigthness);
    
    int width = bufferSize * .05;
    for (int i = 0; i < width; ++i) {
        int index1Minus = std::min(index1 + i, bufferSize - 1);
        int index2Minus = std::min(index2 + i, bufferSize - 1);
        int index3Minus = std::min(index3 + i, bufferSize - 1);
        
        int index1Plus = std::max(index1 - i, 1);
        int index2Plus = std::max(index2 - i, 1);
        int index3Plus = std::max(index3 - i, 1);
        
        (*buffer)[index1Minus] = color;
        (*buffer)[index2Minus] = color;
        (*buffer)[index3Minus] = color;
        (*buffer)[index1Plus] = color;
        (*buffer)[index2Plus] = color;
        (*buffer)[index3Plus] = color;
    }
    
}