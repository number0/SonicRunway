//
//  NetworkInputPattern.cpp
//  SonicRunway
//
//  Created by Shawn Silverman on 8/5/16.
//
//

#include "NetworkInputPattern.hpp"

SrNetworkInputPattern::SrNetworkInputPattern(const std::string & name,
                                             SrModel * model,
                                             SrAudio * audio,
                                             SrGlobalParameters * globalParameters) :
SrPattern(name, model, audio, globalParameters),
_host("127.0.0.1"),
_port(11123),
_minConnectAttemptPeriod(1000),
_lastConnectAttemptTime(0)
{
    // Image setup
    int width = model->GetLightsPerGate();
    int height = model->GetNumGates();
    _image.allocate(width, height, OF_IMAGE_COLOR);
    _image.setColor(ofColor::black);
    
    // The buffer should be a minimum size of 4
    _bufferSize = width * height * 3;
    if (_bufferSize < 4) {
        _bufferSize = 4;
    }
    _buffer = new char[_bufferSize];
}

SrNetworkInputPattern::~SrNetworkInputPattern()
{
    delete _buffer;
}

bool
SrNetworkInputPattern::IsAudioReactive() const
{
    return false;
}

void
SrNetworkInputPattern::_Update()
{
    _image.setColor(ofColor::black);

    if (not IsOnAtAnyGate()) { // Is not enabled?
        if (_tcpClient.isConnected()) {
            _tcpClient.close();
        }
        return;
    }

    // Don't attempt to connect more than once a second
    uint64_t time = ofGetElapsedTimeMillis();
    if (!_tcpClient.isConnected() && time - _lastConnectAttemptTime >= _minConnectAttemptPeriod) {
        _lastConnectAttemptTime = time;
        if (!_tcpClient.setup(_host, _port)) {
            return;
        }
    }
    if (!_tcpClient.isConnected()) {
        return;
    }

    // Trigger a screen send
    _buffer[0] = 0;
    if (!_tcpClient.sendRawBytes(_buffer, 1)) {
        _tcpClient.close();
        return;
    }
        
    // Read the size
    if (!readFully(_buffer, 4)) {
        _tcpClient.close();
        return;
    }
    int32_t pixels = pack4Chars(_buffer);
    if (pixels != _image.getWidth() * _image.getHeight()) {
        skipFully(_buffer, _bufferSize, pixels * 3);
        return;
    }
    
    // Read the pixels
    if (!readFully(_buffer, pixels * 3)) {
        _tcpClient.close();
        return;
    }
    
    _image.setFromPixels((unsigned char *)_buffer, _image.getWidth(), _image.getHeight(), OF_IMAGE_COLOR);
}

bool SrNetworkInputPattern::readFully(char *buf, const int32_t count) {
    int toRead = count;
    while (toRead > 0) {
        int read = _tcpClient.receiveRawBytes(buf, toRead);
        if (!_tcpClient.isConnected()) {
            return false;
        }
        if (read > 0) {
            buf += read;
            toRead -= read;
        }
    }
    return true;
}

void SrNetworkInputPattern::skipFully(char *buf, const int bufSize, int32_t count) {
    while (count > bufSize) {
        if (!readFully(buf, bufSize)) {
            return;
        }
        count -= bufSize;
    }
    if (count > 0) {
        readFully(buf, count);
    }
}

inline int32_t SrNetworkInputPattern::pack4Chars(const char *buf) {
    return ((int)(((signed char)buf[0]) << 24)  // This one is signed
            |  (int)(((unsigned char)buf[1]) << 16)
            |  (int)(((unsigned char)buf[2]) << 8)
            |  (int)((unsigned char)buf[3]));
}

void
SrNetworkInputPattern::_Draw() const
{
    float opacity = GetOpacity()[0];
    if (opacity <= 0) { // Is not enabled?
        return;
    }
    
    // Set a greyscale color according to 'opacity' to multiply the image
    ofSetColor(ofFloatColor(opacity, opacity, opacity));
    
    ofPushMatrix();
    ofRotate(90);
    ofTranslate(0, -_image.getHeight());
    _image.draw(0, 0);
    ofPopMatrix();
}
