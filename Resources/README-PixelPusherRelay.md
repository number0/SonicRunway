# Summary

The PixelPusherRelay program emulates a PixelPusher and sends frames to clients
over TCP.

# Running the program

The program is an executable JAR file and has two required options:

1. --gates: Specifies the total number of gates, including those first
   "virtual" gates.
2. --pixels_per_gate: Specifies the number of pixels in one gate.

To run, for example using 33 gates and 277 pixels per gate:

```
java -jar pixelpusherrelay-1.1.0-jar-with-dependencies.jar --gates=33 --pixels_per_gate=277
```

# Troubleshooting

If no image is displayed, either it's a network problem, where the two programs
can't connect, or the total number of pixels doesn't match the physical setup
exactly. Images are ignored completely if they don't match the setup.
