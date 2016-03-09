//
//  FftPattern.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 3/4/16.
//
//

#include "FftPattern.hpp"
#include "Audio.hpp"
#include "Model.hpp"
#include "FftBuffer.hpp"

SrFftPattern::SrFftPattern(SrModel * model, SrAudio * audio,
                           SrFftBuffer * fftBuffer) :
    SrPattern(model, audio),
    _fftBuffer(fftBuffer)
{
    _pixels.allocate(GetModel()->GetNumStations(),
                    GetModel()->GetLightsPerStation(),
                    3);
    
    _colorBuffer.allocate(model->GetNumStations(), model->GetLightsPerStation(), 3);
}

SrFftPattern::~SrFftPattern()
{
    
}

void
SrFftPattern::Update(const SrTime & now)
{
    const SrModel * model = GetModel();
    
    // Copy pixels
    const ofFloatPixels & fftData = _fftBuffer->GetPerStationData();
    
    for(int x = 0; x < _colorBuffer.getWidth(); x++) {
        for (int y=0; y < _colorBuffer.getHeight(); y++) {
            float fftValue = fftData.getColor(x,y).getLightness();
            fftValue = pow(fftValue, 0.8);
            
            ofFloatColor c;
            float baseColor = 0.15;
            float hue = baseColor - 0.6 * (1.0 - fftValue);
            if (hue < 0.0) {
                hue += 1.0;
            }
            c.setHsb(hue, 1.0, fftValue * 2.0);
            
            _colorBuffer.setColor(x,y,c);
        }
    }
}

void
SrFftPattern::Draw(const SrTime & now) const
{
    ofSetColor(255.0,255.0,255.0,255.0);
    
    float scaledWidth = _fftBuffer->GetBufferLengthInSeconds() /
                            GetModel()->ComputeDelayPerStation();
    
    ofImage image;
    // XXX move this allocation to constructor to avoid re-allocating?
    image.allocate(_colorBuffer.getWidth(), _colorBuffer.getHeight(), OF_IMAGE_COLOR);
    
    image.setFromPixels(_colorBuffer);
    
    image.draw(0,0,_colorBuffer.getWidth(),GetModel()->GetLightsPerStation());
    
    ofPushMatrix();
    ofTranslate(0,GetModel()->GetLightsPerStation());
    ofScale(1,-1);
    
    image.draw(0,0,_colorBuffer.getWidth(),GetModel()->GetLightsPerStation());
    ofPopMatrix();
}