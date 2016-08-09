//
//  GlobalParameters.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 8/8/16.
//
//

#ifndef SR_GLOBAL_PARAMETERS_HPP
#define SR_GLOBAL_PARAMETERS_HPP

class SrModel;

#include "UiMixin.hpp"

//
// SrGlobalParameters -- a few global knobs and dials that are intended
// to control values across many patterns at once.  These will be surfaced
// as values to play with through touchOSC.  They may also vary programmatically
// to automate patterns changing slowly across time.
//
// Patterns should use these use these values to modify the style of their
// display, such as color, speed, decay, etc..
//
// Note:  parameters are called 'Dial' even though they show up as sliders
// in the default UI.  We intend to represent them as dials through TouchOSC
//
// So far:
//   - Dial1 : generally used to shift hues in a relative way.  Range [0,1]
//
class SrGlobalParameters : public SrUiMixin {
    typedef SrGlobalParameters This;
    
public:
    SrGlobalParameters(const std::string & name,
                       SrModel * model);
    virtual ~SrGlobalParameters();
    
    void Update();
    
    float GetDial1() const;
    float GetDial2() const;
    float GetSlider1() const;
    float GetSlider2() const;
    
private:
    SrModel * _model;
    
    ofParameter<bool> _cycleAutomatically;
    ofParameter<float> _secondsPerCycle;
    
    ofParameter<float> _dial1Parameter;
    ofParameter<float> _dial2Parameter;
    ofParameter<float> _slider1Parameter;
    ofParameter<float> _slider2Parameter;
    
    
};


#endif
