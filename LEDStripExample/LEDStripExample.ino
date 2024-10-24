#include <Adafruit_NeoPixel.h>

#define LED_PIN    13
#define LED_COUNT  55

int leftSideLEDs[2] = {0,19};
int middleLEDs[2] = {20,34};
int rightSideLEDs[2] = {35,54};

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

uint32_t magenta = strip.Color(255, 0, 255); //RGB
uint32_t green = strip.Color(0, 255, 0);
uint32_t red = strip.Color(255, 0, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t no_colour = strip.Color(0, 0, 0);

enum Sides {
  NONE,
  LEFT,
  MIDDLE,
  RIGHT,
};

//void setColors(Sides, uint32_t);

void setup() {
  // put your setup code here, to run once:
  
  strip.begin();
  strip.setBrightness(20);
  //strip.setPixelColor(5, 50, 50, 255); // (pixel index, red, green, blue)
  strip.show();
  
}
  void loop(){

    // put your main code here, to run repeatedly:

    // having blue or red on either the left or right side corresponds to having the ballon of that color.
    // having magenta on either side represents having no ballon connected
    // having magenta in the middle represents having ballons of the wrong colors grabbed or no ballons grabbed on either side or both sides
    // having green on all side represetns having the correct color ob ballons grabbed
    

    setColors(LEFT, blue);
    setColors(MIDDLE, green);
    setColors(RIGHT, red);
    delay(500);

    setColors(NONE, no_colour);
    delay(500);

    setColors(LEFT, green);
    setColors(MIDDLE, green);
    setColors(RIGHT, green);
    delay(500);

    setColors(LEFT, magenta);
    setColors(MIDDLE, magenta);
    setColors(RIGHT, blue);
    delay(500);
  }


void setColors(Sides side, uint32_t color){
  int start;
  int end;
  

  switch (side){
    case LEFT:
      start = leftSideLEDs[0];
      end = leftSideLEDs[1];
      break;
    case MIDDLE:
      start = middleLEDs[0];
      end = middleLEDs[1];
      break;
    case RIGHT:
      start = rightSideLEDs[0];
      end = rightSideLEDs[1];
      break;

    default: 
      start = 0;
      end = LED_COUNT-1;
      break;

  }
  
  for (int i = start; i <= end; i++){  
    strip.setPixelColor(i, color);
  }
  strip.show();
}
