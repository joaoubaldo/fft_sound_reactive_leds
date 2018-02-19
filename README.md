# Overview
Sound reactive rgb leds based on FFT processing.
There are different types of animations:
- Beating  
- Shifting

(see config.h for explanation)

## Requirements
Tested with:
- Arduino Nano 328p
- Polulu Led strip (1 data pin for all leds)
- Microphone module with gain and DC offset

## Configuration
Read and change config.h accordingly

## Build requirements
- PlatformIO

## Building
    platformio run

## Uploading
    platformio run -t upload
