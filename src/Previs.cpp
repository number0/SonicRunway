//
//  Previs.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 3/3/16.
//
//

#include "Previs.hpp"
#include "Model.hpp"
#include "Audio.hpp"

SrPrevis::SrPrevis(SrModel * model, SrAudio * audio) :
    UiMixin("Previs"),
    _model(model),
    _audio(audio),
    _lightRadius(0.2),
    _reverseAngleParam(true)
{
    _camera.setFov(35);
    /*
    _camera.lookAt(ofVec3f(0,0,60),ofVec3f(0,1,0));
    _camera.setPosition(0,5.8,-18);
     */
    
    _camera.lookAt(ofVec3f(0,0,-60),ofVec3f(0,1,0));
    _camera.setPosition(0,5.8,1018);
    
    _reverseAngleToggle.setup(_reverseAngleParam);
    _AddUI(&_reverseAngleToggle);
}

SrPrevis::~SrPrevis()
{
    
}

void
SrPrevis::Update()
{
    if (_reverseAngleToggle) {
        _camera.lookAt(ofVec3f(0,0,-60),ofVec3f(0,1,0));
        _camera.setPosition(0,5.8,1018);
    } else {
        _camera.lookAt(ofVec3f(0,0,60),ofVec3f(0,1,0));
        _camera.setPosition(0,5.8,-18);
    }
}

void
SrPrevis::Draw(float x, float y, float width, float height)
{
    ofPushStyle();
    
    // Background
    ofSetColor(10,10,10,255);
    ofFill();
    ofDrawRectangle(x,y,width,height);
    
    // Start 3d stuff here
    _camera.begin(ofRectangle(x,y,width,height));
   
    // Ground plane
    ofPushMatrix();
    ofRotateX(90);
    ofSetColor(30,30,30,255);
    ofDrawPlane(3000,3000);
    ofPopMatrix();
    
    //_DrawSpheres(_lightRadius * 1.5, 40);
    //_DrawSpheres(_lightRadius * 1.3, 40);
    _DrawSpheres(_lightRadius * 1.0, 255);
    
    // End 3d stuff
    _camera.end();
    
    ofPopStyle();
}

void
SrPrevis::_DrawSpheres(float lightRadius, float transparency)
{
    int numSpacesBetweenStations = _model->GetNumStations() - 1;
    float distanceBetweenStations =
        _model->GetRunwayLength() / numSpacesBetweenStations;
    float circumference = _model->GetArchLength() * 4.0 / 3.0;
    float radius = circumference / (M_PI * 2.0);
    
    const ofFloatPixels & pixels = _model->GetFloatPixels();
    
    float groundToCenter = sin(M_PI / 4.0) * radius;
    
    int lastStationIdx = _model->GetLightsPerStation() - 1;
    
    for(int station = 0; station < _model->GetNumStations(); station++) {
        for(int light = 0; light < _model->GetLightsPerStation(); light++) {
            
            ofColor c = pixels.getColor(station, light);
            ofColor drawColor(c[0], c[1], c[2], transparency);
            
            ofSetColor(drawColor);
            
            float angle = (float) light / lastStationIdx * 3 * M_PI / 2;
            angle -= M_PI / 4.0;
            
            float z = station * distanceBetweenStations;
            float x = cos(angle) * radius;
            float y = sin(angle) * radius + groundToCenter;
           
            ofDrawSphere(x,y,z,lightRadius);
        }
    }
    
}

void
SrPrevis::AudioOut(float * output, int bufferSize, int nChannels) const
{
    float cameraDist = _camera.getPosition().length();
    float delay = cameraDist / _model->GetSpeedOfSound();
    
    _audio->AudioOutDelayed(output, bufferSize, nChannels, delay);
}