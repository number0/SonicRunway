//
//  Video.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 6/5/16.
//
//

#include "Video.hpp"
#include "Util.hpp"
#include "Debug.hpp"

SrVideo::SrVideo(const std::string & relativeFileName,
                 ofLoopType loop)
{
    _player.load(SrUtil_GetAbsolutePathForResource(relativeFileName));
    
    _player.play();
    _player.setPosition(0.0);
    _player.update();
    _player.setLoopState(loop);
    
    if (not _player.isLoaded()) {
        SrError("failed to load %s\n", relativeFileName.c_str());
        return;
    }
}

SrVideo::~SrVideo()
{
    
}

int
SrVideo::GetNumFrames() const
{
    return _player.getTotalNumFrames();
}

int
SrVideo::GoToNextFrame()
{
    _player.nextFrame();
    if (_player.getCurrentFrame() >= _player.getTotalNumFrames()) {
        Reset();
    } else {
        _player.update();
    }
    
    return _player.getCurrentFrame();
}

void
SrVideo::Reset()
{
    _player.setPosition(0.0);
    _player.update();
}

void
SrVideo::Draw() const
{
    _player.draw(0,0);
}
