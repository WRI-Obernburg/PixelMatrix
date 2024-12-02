# Hardware

## Hardware Requirements

- 144 neopixels
- 1 esp8266
- 1 5V power supply / 2A
- 1 3D printed case
- 1 Backplate
- 1 LED plate
- 1 Frontplate

The neopixel string has to be connected to the rx pin of the esp8266.
GND from the neopixel string has to be connected to the GND of the esp8266 and the power supply.
The 5V from the neopixel string has to be connected to the 5V of the power supply.

Depending on the exact model of the neopixels you might need a level shifter to convert the 3.3V signal from the esp8266 to 5V.