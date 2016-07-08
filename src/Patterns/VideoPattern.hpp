//
//  VideoPattern.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 5/31/16.
//
//

#ifndef VideoPattern_hpp
#define VideoPattern_hpp

#include "Pattern.hpp"
#include "Video.hpp"

class SrVideoPattern : public SrPattern {
public:
    SrVideoPattern(const std::string & name, SrModel * model,
                   SrAudio * audio);
    virtual ~SrVideoPattern();
    
protected:
    virtual void _Update();
    virtual void _Draw() const;
    
private:
    SrVideo _video;
    int _gateIndex;
};

#endif /* VideoPattern_hpp */
