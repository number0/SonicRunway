# Summary

The TouchOSC Service program provides an mDNS source and HTTP server for
TouchOSC connections and layout files.

# Running the program

The program is an executable JAR file and takes one argument, the .touchosc
file to serve. 

To run and serve, for example, the file, *Sonic Runway iPad H.touchosc*:

```
java -jar touchoscservice1.1.0-jar-with-dependencies.jar "Sonic Runway iPad H.touchosc"
```

To print out all the OSC messages sent to the machine, add the `--print_osc_messages` flag.

**Note:** This uses Apple's mDNS implementation under the hood, so probably
only works on OS X. Also, this program depends on a native library,
libjdns_sd.jnilib that must be in the current working directory when running
the program.

# What it does

The program does two things:

1. Advertises itself as a TouchOSC receiver. This makes it so that you don't
   need to know the IP address. From the *OSC* sub-menu in the *Connections*
   section, simply wait for the computer of interest to appear and tap the
   name. The Host IP address will be automatically filled in.
2. Advertises itself as an instance of TouchOSC Editor. This makes it so that
   any device with TouchOSC installed can download the layout. From the
   *Layout* sub-menu, select *Add* and then, simply wait for the computer of
   interest to appear and tap the name. The layout will be downloaded.
