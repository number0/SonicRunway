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
        SrAudio * audio) :
    SrPattern(name, model, audio)
{
    
}

SrDiagnosticPattern::~SrDiagnosticPattern()
{
    
}

void
SrDiagnosticPattern::_Update()
{
    
}

void
SrDiagnosticPattern::_Draw() const
{
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
            if (not GetEnabled()[x]) {
                continue;
            }
            if (y == 0) {
                ofSetColor(red);
            } else if (y == lightsPerGate - 1) {
                ofSetColor(green);
            } else if (y == x) {
                ofSetColor(white);
            } else if (y < x and y % 5 == 0) {
                ofSetColor(cyan);
            } else if (y == (int) lightsPerGate / 2) {
                ofSetColor(yellow);
            } else {
                ofSetColor(blue);
            }
            ofDrawRectangle(x,y,1,1);
        }
    }
}