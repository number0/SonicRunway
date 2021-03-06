//
//  ImageSequence.cpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/30/16.
//
//

#include "ImageSequence.hpp"
#include "Util.hpp"
#include "Debug.hpp"

SrImageSequence::SrImageSequence(const std::string path,
                                 const std::string & baseFileName,
                                 size_t numFrames,
                                 bool padFrameNumbers)
{
    
    _images.resize(numFrames);
    
    //std::string prefix = SrUtil_GetAbsolutePathForResource(path);
    std::string prefix = path + "/" + baseFileName;
    
    for(size_t i = 0; i < numFrames; i++) {
        char *str;
       
        if (padFrameNumbers) {
            asprintf(&str, "%s%03zu.png", prefix.c_str(), i + 1);
        } else {
            asprintf(&str, "%s%zu.png", prefix.c_str(), i + 1);
        }
        _loader.loadFromDisk(_images[i], str); // multithreaded background loader
        //_images[i].load(str); // blocking old-school loader
        
        free(str);
    }
}

SrImageSequence::~SrImageSequence()
{
    _loader.stopThread();
}
    
size_t
SrImageSequence::GetNumFrames() const
{
    return _images.size();
}
    
const ofImage &
SrImageSequence::GetImage(size_t frame) const
{
    return _images[frame];
}
