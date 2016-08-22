//
//  NetworkInputPattern.hpp
//  SonicRunway
//
//  Created by Shawn Silverman on 8/5/16.
//
//

#ifndef NetworkInputPattern_hpp
#define NetworkInputPattern_hpp

#include "Pattern.hpp"
#include "ofxTCPClient.h"

//
// A pattern that takes screens, for all the LEDs, from the network.
//
class SrNetworkInputPattern : public SrPattern {
public:
    SrNetworkInputPattern(const std::string & name,
                          SrModel * model,
                          SrAudio * audio,
                          SrGlobalParameters * globalParameters);
    virtual ~SrNetworkInputPattern();
    
    virtual bool IsAudioReactive() const;
    
protected:
    virtual void _Update();
    virtual void _Draw() const;
    
private:
    ofImage _image; // LightsPerGate x NumGates
    ofxTCPClient _tcpClient;
    const string _host;
    const int _port; // Try to connect to this port to receive the image
    const uint64_t _minConnectAttemptPeriod; // In milliseconds
    uint64_t _lastConnectAttemptTime; // Connect at most once a second
    char *_buffer;
    int _bufferSize;
    
    // Parameters
    ofParameter<bool> _flipH;
    ofParameter<bool> _flipV;

    // Functions
    
    // Reads the all the specified number of bytes from the TCP connection.
    bool readFully(char * buf, const int32_t count);

    // Skips count bytes using a buffer size of bufSize.
    void skipFully(char *buf, const int bufSize, int32_t count);

    // Turns 4 bytes in big-endian order into an int.
    inline int32_t pack4Chars(const char *buf);
};

#endif /* NetworkInputPattern_hpp */
