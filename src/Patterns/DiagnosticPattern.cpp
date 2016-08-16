//
//  DiagnosticPattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/14/16.
//
//

#include "DiagnosticPattern.hpp"

SrDiagnosticPattern::SrDiagnosticPattern(
        const std::string & name,
        SrModel * model,
        SrAudio * audio,
        SrGlobalParameters * globalParameters) :
    SrPattern(name, model, audio, globalParameters)
{
    
}

SrDiagnosticPattern::~SrDiagnosticPattern()
{
    
}

bool
SrDiagnosticPattern::IsAudioReactive() const
{
    return false;
}

void
SrDiagnosticPattern::_Update()
{
    
}

void
SrDiagnosticPattern::_Draw() const
{
    if (not IsOnAtAnyGate()) {
        return;
    }
    
    int numGates = GetModel()->GetNumGates();
    int lightsPerGate = GetModel()->GetLightsPerGate();
 
    ofFloatColor red(1.0,0,0);
    ofFloatColor green(0.0,1.0,0);
    ofFloatColor blue(0,0,1.0);
    ofFloatColor yellow(1.0,1.0,0.0);
    ofFloatColor white(1.0,1.0,1.0);
    ofFloatColor cyan(0.0,1.0,1.0);
    
    for(int x = 0; x < numGates; x++) {
        for(int y = 0; y < lightsPerGate; y++) {
            float opacity = GetOpacity()[x];
            if (opacity <= 0.0) {
                continue;
            }
            
            ofColor c;
            
            if (y == 0) {
                c = red;
            } else if (y == lightsPerGate - 1) {
                c = green;
            } else if (y == x) {
                c = white;
            } else if (y < x and y % 5 == 0) {
                c = cyan;
            } else if (y == (int) lightsPerGate / 2) {
                c = yellow;
            } else {
                c = blue;
            }
            
            c *= opacity;
            ofSetColor(c);
            
            ofDrawRectangle(x,y,1,1);
        }
    }
}