# Balloon Detection Subsystem

## Overview

Our objective with the led strips is to detect the ballon color that is grabbed by the vacuum and displayed on the Led strip, on the respective side of the ballon

## Required Hardware

* Arduino Uno
* Led Strip (55 pixels)
* Power supply (5V, 1.5 A)
* I2C Multiplexer
* 2 TCS34725 (Color sensors)
* USB type A to B cable

## Required Software

* Arduino IDE >= 2.0

### Required Libraries

* Wire Library
* Adafruit Neopixel Library
* Adafruit TCA34725 Library

## LED Strip Code Example

```cpp
#include <Adafruit_NeoPixel.h>

#define LED_PIN    13
#define LED_COUNT  55

int leftSideLEDs[2] = {0,19};
int middleLEDs[2] = {20,34};
int rightSideLEDs[2] = {35,54};

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

uint32_t magenta = strip.Color(255, 0, 255);
uint32_t green = strip.Color(0, 255, 0);
uint32_t red = strip.Color(255, 0, 0);
uint32_t blue = strip.Color(0, 0, 255);
void setup() {
  // put your setup code here, to run once:
  
  strip.begin();
  strip.setBrightness(20);
  strip.setPixelColor(5, 50, 50, 255); // (pixel index, red, green, blue)
  strip.show();
  
}

void loop() {

  // put your main code here, to run repeatedly:

  // having blue or red on either the left or right side corresponds to having the ballon of that color.
  // having magenta on either side represents having no ballon connected
  // having magenta in the middle represents having ballons of the wrong colors grabbed or no ballons grabbed on either side or both sides
  // having green on all side represetns having the correct color ob ballons grabbed
  setColors(0, blue);
  setColors(1, green);
  setColors(2, red);
  delay(500);
  setColors(0, green);
  setColors(1, green);
  setColors(2, green);
  delay(500);
  setColors(0, magenta);
  setColors(1, magenta);
  setColors(2, blue);
  delay(500);
}

void setColors(int side, uint32_t color){
  int start = 0;
  int end = 0;
  switch (side){
    case 0:
      start = leftSideLEDs[0];
      end = leftSideLEDs[1];
      break;
    case 1:
      start = middleLEDs[0];
      end = middleLEDs[1];
      break;
    case 2:
      start = rightSideLEDs[0];
      end = rightSideLEDs[1];
      break;
  }
  
  for (int i = start; i <= end; i++){
    strip.setPixelColor(i, color);
  }
  strip.show();
}
```

## Color Sensor Code Example

...

## Links and Resources

...
