//
//  DiagnosticPattern.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 7/14/16.
//
//

#ifndef SR_DIAGNOSTIC_PATTERN_HPP
#define SR_DIAGNOSTIC_PATTERN_HPP

#include "Pattern.hpp"

//
// A simple pattern useful for testing LED strips and
// configuration.
//
// The first light of each strip is RED
// The last light is GREEN
// The middle light is YELLOW
// The light matching the gate index is WHITE
// The rest of the lights are blue.
//
class SrDiagnosticPattern : public SrPattern {
public:
    SrDiagnosticPattern(const std::string & name,
                        SrModel * model,
                        SrAudio * audio,
                        SrGlobalParameters * globalParameters);
    virtual ~SrDiagnosticPattern();
    
    virtual bool IsAudioReactive() const;
    
protected:
    virtual void _Update();
    virtual void _Draw() const;
    
};


#endif /* DiagnosticPattern_hpp */
