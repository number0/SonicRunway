
#include "ofApp.h"

#include "App.hpp"

ofApp::ofApp() :
    _isSetupComplete(false)
{
    
}

void ofApp::setup()
{
    
    essentia::init();
    
    _app = new SrApp(this);
    
    // set the size of the window
    ofSetWindowShape(1800, 900);
    
    _isSetupComplete = true;
}

void ofApp::exit(){
    essentia::shutdown();
    
    delete _app;
    
    ofSoundStreamStop();
    ofSoundStreamClose();
}

void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    // If setup isn't complete, our audio class is not yet allocated.
    if (not _isSetupComplete) {
        return;
    }
    
    _app->AudioIn(input, bufferSize, nChannels);
}

void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    // If setup isn't complete, our audio class is not yet allocated.
    if (not _isSetupComplete) {
        return;
    }
    
    _app->AudioOut(output, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::update(){
    _app->Update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    _app->Draw();
}
