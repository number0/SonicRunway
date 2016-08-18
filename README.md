# SonicRunway

Software control for Sonic Runway art installation, a visualization of the
speed of sound.  

See http://sonicrunway.net

Mac install instructions:

- Download and install XCode from the app store.  I'm using 7.2:
    https://developer.apple.com/xcode/download/

- Install OpenFrameworks:  http://www.openframeworks.cc/download/
  follow xcode install documentation:  http://www.openframeworks.cc/setup/xcode/

- Install ofxAubio addon:  https://github.com/aubio/ofxAubio
  Just clone the repository into the addons directory.
  Don't do the the fetch_aubio_framework.sh part

- Install aubio library (with homebrew):
   brew install aubio

- Install essentia audio analysis library (with homebrew):
    https://github.com/MTG/homebrew-essentia
    This should end up under /usr/local/

- Clone this repo under (yourOpenFramworksDir)/apps/myApps/

- Open SonicRunway.xcodeproj

- From the top bar, next to the play/stop icons, make sure the 
  'Sonic Runway Debug' build target is selected.

- Build and run! 

OVERVIEW:

The app uses OpenFrameworks, which is kinda like Processing but C++.
It wraps OpenGL and other lower level libraries with a convenient
API so you can set up a window, get audio, and draw stuff.  
There are a lot of 'extensions' available to provide other 
functionality.  

Lots of good info on OpenFrameworks here:

    http://openframeworks.cc/ofBook/chapters/foreword.html

or the source for the book:

    https://github.com/openframeworks/ofBook/

Basically, the program:
  - gets audio input
  - processes the audio input to extract fft and other cues
  - renders a bunch of patterns to a buffer
  - broadcasts the buffer to the LEDs

The 'SrBuffer' class is a generic circular buffer that can store
samples of a parameter value.  It can then be queried per station.
This provides the history necessary to make patterns that ripple
down the runway at the speed of sound.  

It's important to note that the audio processing and render/draw
functions are called from different threads and at different 
intervals.  So far, I haven't bothered making things truly 
thread safe because the various buffers are all statically 
allocated. So, we may see some visual tearing.  If this becomes 
problematic we can use ofMutex to make it thread safe.

Notes on setting up a Sonic Runway server:

- Install runway software (above)

- Install Loopback (with license)
    - enter loopback licence info
    - configure 'Loopback Audio' device:
       - Left:  MOTU Microbook II: 3 / MOTU Microbook II: Left (1)
       - Rigut: MOTU Microbook II: 4 / MOTU Microbook II: Right (2)

- Build a 'product' version of software and add it to login items

- 'Energy saver' settings:
    - Computer sleep / display sleep -> Never
    - Start up automatically after a power failure
    - UPS - don't let computer or display sleep
    - UPS, Shutdown options...:
        Shut down after using UPS battery for 15 mins
        Shut down computer when time remaining on UPS is 2 mins
        Shut down computer when UPS battery is below 5%

- Configure network settings:
   Ethernet:
      - Manual IP:  192.168.0.1
      - Subnet mask:  255.255.0.0
   Wifi:
      - sonic
      - Manual IP:  192.168.1.3


TODO:

- Expand library of patterns. (Everyone!)

- Make sure we can automatically run the software on startup. (RJ)

- Debug crash -- maybe in 'BigTrails' when rotation and jitter
  have high values?  (Paul?)

- Design TouchOSC layout - include master parameter sliders and 
  buttons for presets (?)

- Add parameters to fiddle the gain on the LEDs so we can color 
  correct them.  (RJ)

- Add a parameter to delay so first physical gate is #2

- Run speed tests to make sure frame rate keeps up.

- Change 'rainbow' pattern so it doesn't go all white

- Add a subtle pattern going the other way.

- Add pattern that makes a stripe that wanders based on RMS

- (low priority) Figure out how to build standalone app.  
  Product->Archive does this mostly,
  but dependencies on dylibs causes problems.  This means either figuring out
  how to bundle dylibs with the app, or compiling them statically.  Some info:

http://lessons.livecode.com/m/4071/l/15029-linking-an-osx-external-bundle-with-a-dylib-library

