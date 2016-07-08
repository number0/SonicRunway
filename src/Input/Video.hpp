//
//  Video.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 6/5/16.
//
//

#ifndef SR_VIDEO_HPP
#define SR_VIDEO_HPP

#include <stdio.h>

#include "ofMain.h"

//
// SrVideo - provides access to a video asset.
//
// Right now this is just a thin wrapper on ofVideoPlayer.  But it's nicer
// to centralize it in case we need to change how paths are handled, or
// if it becomes important to manually cache the frames somehow.
//
class SrVideo {
public:
    SrVideo(const std::string & relativeFile, ofLoopType loop);
    ~SrVideo();
    
    int GetNumFrames() const;
    
    int GoToNextFrame();
    void Reset();
    virtual void Draw() const;

private:
    ofVideoPlayer _player;
};

#endif
