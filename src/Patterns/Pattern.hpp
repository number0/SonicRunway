//
//  Pattern.hpp
//  SonicRunway
//
//  Created by Rob Jensen on 3/2/16.
//
//

#ifndef SR_PATTERN_HPP
#define SR_PATTERN_HPP

#include <stdio.h>

#include "Types.hpp"
#include "SimpleBuffer.hpp"
#include "UiMixin.hpp"
#include <string>

class SrModel;
class SrAudio;

//
// Base class for patterns that draw themselves to the array
// of lights.  Subclasses should implement Update and Draw.
//
// SrPatterns are expected to update every LED for every frame.
// If you're making a pattern that is intended to stream down
// the runway at the speed of sound, and doesn't change as
// it ripples down, consider deriving from SrScrollingPattern
// instead.
//
// Pattern instances are created and owned by SrApp.
//
class SrPattern : public SrUiMixin {
public:
    SrPattern(const std::string & name,
              SrModel * model, SrAudio * audio,
              SrGlobalParameters * globalParameters);
    virtual ~SrPattern();
    
    // Called by the main app to update state.
    // Subclasses should implement _Update()
    void Update();
    
    // Called by the main app to render the pattern to the
    // light buffer.  Subclasses should implement _Draw()
    void Draw();
    
    // Set the current value of 'enabled'.  This will be
    // buffered across time.
    void SetEnabled(bool enabled);
    bool GetEnabled();
    
    // Return true if the pattern is 'on' at any gate.
    // This lets patterns skip computation if they are off everywhere.
    bool IsOnAtAnyGate() const;
    
    // Return 'true' if the pattern reacts to the audio signal.
    // Default returns true.
    virtual bool IsAudioReactive() const;

    // Return 'true' if this pattern can be crossfaded during
    // transitions
//    virtual bool CanXfade() const;
    
    // Get a buffer with per-gate opacity information.
    // Opacity is 1 if the pattern is enabled, 0 if disabled.
    // The value is ramped after the pattern is explicitly enabled/disabled.
    const SrFloatSimpleBuffer & GetOpacity() const;
    
    virtual std::string GetGlobalParameterLabel(const std::string & name) const;
    
protected:
    
    // Update the pattern in response to the current state.
    // Prepare to draw.
    virtual void _Update() = 0;
    
    // Draw the pattern.  By the time this is called, the render
    // state will be set to draw to a frame buffer that represents
    // the lights.
    //   X coordinates are [0, numGates).
    //   Y coordinates are [0, lightsPerGate).
    //
    // Blend mode is OF_BLENDMODE_ADD so the color values will
    // sum together.
    //
    // Patterns should respect per-gate values from the 'Enabled'
    // buffer, and not draw anything when/where they are disabled.
    virtual void _Draw() const = 0;
    
    SrModel * GetModel() const;
    SrAudio * GetAudio() const;
    SrGlobalParameters * GetGlobalParameters() const;
    
private:
    SrModel *_model;
    SrAudio *_audio;
    SrGlobalParameters *_globalParameters;
    SrFloatSimpleBuffer _enabledBuffer;
    ofParameter<bool> _enabledParam;
    
    SrFloatSimpleBuffer _opacityBuffer;
    ofParameter<bool> _opacity;
    
    ofxToggle _enabledToggle;
};

#endif
