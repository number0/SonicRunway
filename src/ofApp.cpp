
#include "ofApp.h"

#include "App.hpp"

//--------------------------------------------------------------
void ofApp::setup(){
    
    essentia::init();
    
    _app = new SrApp(this);
    
    // set the size of the window
    ofSetWindowShape(1800, 900);
}

void ofApp::exit(){
    essentia::shutdown();
    
    delete _app;
    
    ofSoundStreamStop();
    ofSoundStreamClose();
}

void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    _app->AudioIn(input, bufferSize, nChannels);
}

void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
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
