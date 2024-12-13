#include <Adafruit_NeoPixel.h>

#define LEFT_STRIP_PIN    5
#define RIGHT_STRIP_PIN   6
#define LED_COUNT  21

Adafruit_NeoPixel strip_left(LED_COUNT, LEFT_STRIP_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_right(LED_COUNT, RIGHT_STRIP_PIN, NEO_GRB + NEO_KHZ800);

uint32_t magenta = strip_left.Color(255, 0, 255); //RGB
uint32_t green = strip_left.Color(0, 255, 0);
uint32_t red = strip_left.Color(255, 0, 0);
uint32_t blue = strip_left.Color(0, 0, 255);
uint32_t no_colour = strip_left.Color(0, 0, 0);

enum Sides {
  NONE,
  LEFT,
  MIDDLE,
  RIGHT,
};

void setColors(Sides, uint32_t);

void setup() {
  // put your setup code here, to run once:
  
  strip_left.begin();
  strip_left.setBrightness(20);
  // strip_left.setPixelColor(0, 50, 50, 255); // (pixel index, red, green, blue)
  strip_left.show();

  strip_right.begin();
  strip_right.setBrightness(20);
  // strip_right.setPixelColor(0, 50, 50, 255); // (pixel index, red, green, blue)
  strip_right.show();
}
  void loop(){

    // put your main code here, to run repeatedly:

    // having blue or red on either the left or right side corresponds to having the ballon of that color.
    // having magenta on either side represents having no ballon connected
    // having magenta in the middle represents having ballons of the wrong colors grabbed or no ballons grabbed on either side or both sides
    // having green on all side represetns having the correct color ob ballons grabbed
    
    setColors(LEFT, green);
    setColors(RIGHT, green);
    delay(500);

    setColors(LEFT, red);
    setColors(RIGHT, red);
    delay(500);

    setColors(LEFT, no_colour);
    setColors(RIGHT, no_colour);
    delay(500);

    setColors(LEFT, magenta);
    setColors(RIGHT, magenta);
    delay(500);

    setColors(LEFT, blue);
    setColors(RIGHT, blue);
    delay(500);
  }


void setColors(Sides side, uint32_t color){
  switch (side){
    case LEFT:
      strip_left.fill(color);
      strip_left.show();
      break;
    case RIGHT:
      strip_right.fill(color);
      strip_right.show();
      break;

    default:
      break;
  }
}
