//
//  ImageSequence.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/30/16.
//
//

#ifndef SR_IMAGE_SEQUENCE_HPP
#define SR_IMAGE_SEQUENCE_HPP

#include <stdio.h>
#include "ofMain.h"

#include <string>
#include <vector>

#include "ofxThreadedImageLoader.h"

//
// SrImageSequence loads a numbered series of images into memory and
// allows them to be drawn in arbitrary order.  Ideally, we'd use quicktimes
// and ofVideoPlayer for this, but for some reason I can't seem to read
// the pixels out of the ofVideoPlayer.
//
class SrImageSequence {
public:
    SrImageSequence(const std::string path,
                    const std::string & baseFileName, size_t numFrames,
                    bool padFrameNumbers);
    ~SrImageSequence();
    
    size_t GetNumFrames() const;
    
    const ofImage & GetImage(size_t frame) const;
    
private:
    ofxThreadedImageLoader _loader;
    std::vector<ofImage> _images;

    
};

#endif /* ImageSequence_hpp */
