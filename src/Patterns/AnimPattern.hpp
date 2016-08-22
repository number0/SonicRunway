//
//  AnimPattern.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/30/16.
//
//

#ifndef SR_ANIM_PATTERN_HPP
#define SR_ANIM_PATTERN_HPP

#include "Pattern.hpp"
#include "ImageSequence.hpp"

//
// A pattern that plays an image sequence in a loop.
//
class SrAnimPattern : public SrPattern {
public:
    SrAnimPattern(const std::string & name,
                  const std::string & path,
                  const std::string & baseFileName,
                  size_t numFrames,
                  bool padFrameNumbers,
                  SrModel * model,
                  SrAudio * audio,
                  SrGlobalParameters * globalParameters);
    virtual ~SrAnimPattern();
    
    virtual bool IsAudioReactive() const;
    
protected:
    virtual void _Update();
    virtual void _Draw() const;
    
private:
    SrImageSequence _imageSequence;
    size_t _currentFrame;
    int _gateIndex;
    
    ofParameter<bool> _scroll;
};

#endif /* AnimPattern_hpp */