//
//  ScrollingPattern.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 5/5/16.
//
//

#ifndef SR_SCROLLING_PATTERN_HPP
#define SR_SCROLLING_PATTERN_HPP

#include <stdio.h>

#include "Pattern.hpp"

//
// ScScrollingPattern is an SrPattern that automatically scrolls
// down the runway at the speed of sound.  Subclasses only need to
// implement _DrawCurrentGate, and SrScrollingPattern keeps track
// of the buffer and renders the rest of the gates appropriately.
//
class SrScrollingPattern : public SrPattern {
public:
    SrScrollingPattern(const std::string & name,
                       SrModel * model, SrAudio * audio,
                       SrGlobalParameters * globalParameters);
    ~SrScrollingPattern();
    
    void SetIsMask(bool mask);
    void SetIsInvertMask(bool invert);
    
protected:
    // Implemented from SrPattern
    virtual void _Update();
    virtual void _Draw() const;
    
    virtual bool _FlipLeftRight() const;
    
    // Subclasses implement this to draw the current gate.
    virtual void _DrawCurrentGate(std::vector<ofColor> * buffer) const = 0;
        
private:
    int _index;
    
    ofImage _image;
    ofParameter<bool> _mask;
    ofParameter<bool> _invertMask;
    ofParameter<bool> _reverse;
    
    bool _maskWasOn;
};

#endif
