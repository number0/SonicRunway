# Summary

The PixelPusherRelay program emulates a PixelPusher and sends frames to clients over TCP.

# Running the program

The program is an executable JAR file and has two required options:

1. --gates: Specifies the total number of gates, including those first "virtual" gates.
2. --pixels_per_gate: Specifies the number of pixels in one gate.

To run, for example using 34 gates and 277 pixels per gate:

```
java -jar pixelpusherrelay-1.0.2-jar-with-dependencies.jar --gates=34 --pixels_per_gate=277
```
