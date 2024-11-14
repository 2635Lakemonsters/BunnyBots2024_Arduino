#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <Adafruit_NeoPixel.h>

#define TCAADDR       0x70
#define LEFTSENSOR    2
#define RIGHTSENSOR   3
#define STRIP_PIN     4
#define LED_COUNT     55
#define RED_RIGHT_PIN 8
#define BLUE_RIGHT_PIN 9
#define RED_LEFT_PIN 10
#define BLUE_LEFT_PIN 11


struct Color_Type {
  enum type { RED, BLUE, NONE };
  static constexpr unsigned num_types = 3;
  static constexpr type types[] = {RED, BLUE, NONE};
  static constexpr const char* names[] = {"red", "blue", "none"};
  static constexpr type default_type = NONE;
};

// The threshold for the ratio between red and blue that signifies whether a red balloon or blue balloon are seen
const float midThresh = 2;
const float midThreshWidth = 1.3;
// This value is used to tell which sensor is currently being measured
bool rightSensor = false;

// The LED ranges that define the separate parts of the LED strip
int leftSideLEDs[2] = {0,1}; // {0,19}
int middleLEDs[2] = {2,3};// {20,34}
int rightSideLEDs[2] = {4,5};// {35,54}

// Strip Setup Constants
Adafruit_NeoPixel strip(LED_COUNT, STRIP_PIN, NEO_GRB + NEO_KHZ800);
const uint32_t magenta = strip.Color(255, 0, 255); 
const uint32_t green = strip.Color(0, 255, 0);
const uint32_t red = strip.Color(255, 0, 0);
const uint32_t blue = strip.Color(0, 0, 255);
const uint32_t no_colour = strip.Color(0, 0, 0);

// Color sensor variables
uint16_t r;
uint16_t g;
uint16_t b;
uint16_t c;

 // Sections of the LED strip
enum Sides {
  NONE,
  LEFT,
  MIDDLE,
  RIGHT,
};

// switches the selected address of the multiplexer
void tcaSelect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

// Defining helper functions
void setColors(Sides, uint32_t);
int checkColor(uint32_t, uint32_t, uint32_t);

// Initializing sensors
Adafruit_TCS34725 lightSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_1X);

void setup() {
  //Initializing data pins
  pinMode(RED_RIGHT_PIN, OUTPUT);

  // Starting everything up (comms and sensors)
  Serial.begin(115200);
  strip.begin();
  strip.setBrightness(20);
  strip.show();
  lightSensor.begin();
  tcaSelect(LEFTSENSOR);
  delay(100);
}

void loop(){
  //tcaSelect(LEFTSENSOR);
  //rightSensor = false;
  //lightSensor.getRawData(&r,  &g,  &b,  &c);
  //Serial.println("LEFT SENSOR");
  // Serial.print("RED: ");
  // Serial.println(r, DEC);
  // Serial.print("GREEN: ");
  // Serial.println(g, DEC);
  // Serial.print("BLUE: ");
  // Serial.println(b, DEC);
  // Serial.println();
  //delay(100);
  //setSideColors();
  tcaSelect(RIGHTSENSOR);
  rightSensor = true;
  lightSensor.getRawData(&r,  &g,  &b,  &c);
  Serial.println("RIGHT SENSOR");
  // Serial.print("RED: ");
  // Serial.println(r, DEC);
  // Serial.print("GREEN: ");
  // Serial.println(g, DEC);
  // Serial.print("BLUE: ");
  // Serial.println(b, DEC);
  // Serial.println();
  delay(100);
  setSideColors();
}

void setSideColors(){
  int color = checkColor(r,g,b);


  


  if (color == Color_Type::RED && rightSensor == false){
    // Serial.println("RED");
    setColors(LEFT, red);
  }

  if (color == Color_Type::RED && rightSensor == true){
    // Serial.println("RED");
    setColors(RIGHT, red);
    digitalWrite(RED_RIGHT_PIN, HIGH);
  }

  if (color == Color_Type::BLUE && rightSensor == false){
    // Serial.println("BLUE");
    setColors(LEFT, blue);
  }

  if (color == Color_Type::BLUE && rightSensor == true){
    // Serial.println("BLUE");
    setColors(RIGHT, blue);
    digitalWrite(RED_RIGHT_PIN, LOW);
  }

  if (color == Color_Type::NONE && rightSensor == false){
    // Serial.println("NONE");
    setColors(LEFT, green);
    
  }

  if (color == Color_Type::NONE && rightSensor == true){
    // Serial.println("NONE");
    setColors(RIGHT, green);
    digitalWrite(RED_RIGHT_PIN, LOW);
  }
}

int checkColor(uint32_t red, uint32_t green, uint32_t blue){
  float tempblue = (float) blue;
  float tempred = (float) red;
  float ratio = tempred/tempblue;
  // Serial.println(tempred);
  // Serial.println(tempblue);
  Serial.print("Ratio:");
  Serial.println(ratio);

  if (ratio > midThresh + midThreshWidth) {
    return Color_Type::RED;
  } else if (ratio <  midThresh - midThreshWidth) {
    return Color_Type::BLUE;
  } else {
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
