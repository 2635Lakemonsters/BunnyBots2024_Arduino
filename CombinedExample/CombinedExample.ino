#include <Wire.h>

#include <Adafruit_TCS34725.h>

#include <Adafruit_NeoPixel.h>

// define constants
#define TCAADDR 0x70
#define LEFTSENSOR 2
#define RIGHTSENSOR 3
#define LED_PIN    13
#define LED_COUNT  55

struct Color_Type {
  enum type { RED, BLUE, NONE };
  static constexpr unsigned num_types = 3;
  static constexpr type types[] = {RED, BLUE, NONE};
  static constexpr const char* names[] = {"red", "blue", "none"};
  static constexpr type default_type = NONE;
};


const float redTolerance = 1.80;
const float blueTolerance = 0.7;

int leftSideLEDs[2] = {0,19};
int middleLEDs[2] = {20,34};
int rightSideLEDs[2] = {35,54};

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

uint32_t magenta = strip.Color(255, 0, 255); //RGB
uint32_t green = strip.Color(0, 255, 0);
uint32_t red = strip.Color(255, 0, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t no_colour = strip.Color(0, 0, 0);

uint16_t r;
uint16_t g;
uint16_t b;
uint16_t c;

enum Sides {
  NONE,
  LEFT,
  MIDDLE,
  RIGHT,
};

void tcaSelect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

// initialize sensors


void setColors(Sides, uint32_t);
int checkColor(uint32_t, uint32_t, uint32_t);


Adafruit_TCS34725 lightSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
void setup() {
  // put your setup code here, to run once:
  
  strip.begin();
  strip.setBrightness(20);
  //strip.setPixelColor(5, 50, 50, 255); // (pixel index, red, green, blue)
  strip.show();

  Serial.begin(115200);
  lightSensor.begin();
  tcaSelect(LEFTSENSOR);
  delay(100);
  
}

  void loop(){
  // tcaSelect(LEFTSENSOR);
  // lightSensor.getRawData(&r,  &g,  &b,  &c);
  // Serial.println("LEFT SENSOR");
  // Serial.print("RED: ");
  // Serial.println(r, DEC);
  // Serial.print("GREEN: ");
  // Serial.println(g, DEC);
  // Serial.print("BLUE: ");
  // Serial.println(b, DEC);
  // Serial.println();
  // delay(100);

  tcaSelect(RIGHTSENSOR);
  lightSensor.getRawData(&r,  &g,  &b,  &c);
  Serial.println("RIGHT SENSOR");
  Serial.print("RED: ");
  Serial.println(r, DEC);
  Serial.print("GREEN: ");
  Serial.println(g, DEC);
  Serial.print("BLUE: ");
  Serial.println(b, DEC);
  Serial.println();
  
  if (checkColor(r,g,b) == Color_Type::RED){
    Serial.println("RED");
    setColors(LEFT, red);
    setColors(RIGHT, red);
  }
  if (checkColor(r,g,b) == Color_Type::BLUE){
    Serial.println("BLUE");
    setColors(LEFT, blue);
    setColors(RIGHT, blue);
  }
  if (checkColor(r,g,b) == Color_Type::NONE){
    Serial.println("NONE");
    setColors(NONE, green);
  }

}

int checkColor(uint32_t red, uint32_t green, uint32_t blue){
  double tempblue = (double) blue;
  double tempred = (double) red;
  Serial.println(tempred);
  Serial.println(tempblue);
  Serial.println(tempred / tempblue);
  if (tempred / tempblue > redTolerance)
  {
    
    return Color_Type::RED;
  }
  else if (tempblue/ tempred > blueTolerance) 
  {
    return Color_Type::BLUE;
  }
  else
  {
    return Color_Type::NONE;
  }
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

// const double btmp{static_cast<double>(blue)};
