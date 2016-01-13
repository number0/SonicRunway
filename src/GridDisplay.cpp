//
//  GridDisplay.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 1/11/16.
//
//

#include "GridDisplay.hpp"
#include "Settings.hpp"
#include "LightArray.hpp"

#include "ofMain.h"

void
SrGridDisplay::Setup(SrLightArray *lightArray,
                     SrSettings *settings,
                     float x, float y,
                     float width, float height)
{
    _lightArray = lightArray;
    _settings = settings;
    _x = x;
    _y = y;
    _width = width;
    _height = height;
}

void
SrGridDisplay::Update()
{
    
}

void
SrGridDisplay::Draw()
{
    ofSetColor(10, 10, 10);
    ofDrawRectangle(_x, _y, _width, _height);
    
    float margin = 10.0;
    float radius = 5.0;
    
    int nStations = _settings->GetNumStations();
    int nLightsPerStation = _settings->GetLightsPerStation();
    
    float widthPerStation = (_width - margin * 2.0) / nStations;
    float heightPerLight = (_height - margin * 2.0) / nLightsPerStation;
    
    for (int stationIdx = 0; stationIdx < nStations; stationIdx++) {
        for (int lightIdx = 0;
             lightIdx < nLightsPerStation; lightIdx++) {
           
            float x = margin + widthPerStation * stationIdx;
            float y = margin + heightPerLight * lightIdx;
           
            ofSetColor(_lightArray->GetColor(stationIdx, lightIdx));
            ofFill();
            ofDrawCircle(_x + x, _y + y, radius);
        }
    }
        
}

void
SrGridDisplay::Exit()
{
    
}