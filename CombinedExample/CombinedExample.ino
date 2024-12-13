#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <Adafruit_NeoPixel.h>

#define TCAADDR          0x70
#define LEFTSENSOR       7
#define RIGHTSENSOR      6
#define LEFT_STRIP_PIN   5
#define RIGHT_STRIP_PIN  6
#define LED_COUNT        21
#define RED_RIGHT_PIN    8
#define BLUE_RIGHT_PIN   9
#define RED_LEFT_PIN     10
#define BLUE_LEFT_PIN    11
#define RIGHT_ENABLE_PIN 2
#define LEFT_ENABLE_PIN  3


struct Color_Type {
  enum type { RED, BLUE, NONE };
  static constexpr unsigned num_types = 3;
  static constexpr type types[] = {RED, BLUE, NONE};
  static constexpr const char* names[] = {"red", "blue", "none"};
  static constexpr type default_type = NONE;
};

// The threshold for the ratio between red and blue that signifies whether a red balloon or blue balloon are seen
const float midThresh = 1.50;
const float midThreshWidth = 0.75;
// This value is used to tell which sensor is currently being measured
bool rightSensor = false;
const int delayTime = 100;

// The LED ranges that define the separate parts of the LED strip
// int leftSideLEDs[2] = {0,3}; // {0,19}
// int middleLEDs[2] = {4,7};// {20,34}
// int rightSideLEDs[2] = {8,11};// {35,54}

// Initializing neostrips
Adafruit_NeoPixel strip_left(LED_COUNT, LEFT_STRIP_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip_right(LED_COUNT, RIGHT_STRIP_PIN, NEO_GRB + NEO_KHZ800);

// Initializing sensors
Adafruit_TCS34725 sensor_left = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_1X);
Adafruit_TCS34725 sensor_right = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_1X);

// Strip Constants
const uint32_t magenta = strip_left.Color(255, 0, 255); 
const uint32_t green = strip_left.Color(0, 255, 0);
const uint32_t red = strip_left.Color(255, 0, 0);
const uint32_t blue = strip_left.Color(0, 0, 255);
const uint32_t no_colour = strip_left.Color(0, 0, 0);

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

void setup() {
  //Initializing data pins
  pinMode(RED_RIGHT_PIN, OUTPUT);
  pinMode(RED_LEFT_PIN, OUTPUT);
  pinMode(BLUE_RIGHT_PIN, OUTPUT);
  pinMode(BLUE_LEFT_PIN, OUTPUT);
  pinMode(RIGHT_ENABLE_PIN, INPUT_PULLUP);
  pinMode(LEFT_ENABLE_PIN, INPUT_PULLUP);

  // Starting everything up (comms and sensors)
  Serial.begin(115200);

  strip_left.begin();
  strip_left.setBrightness(10);
  strip_left.show();

  tcaSelect(LEFTSENSOR);
  sensor_left.begin();
  delay(delayTime);
  
  strip_right.begin();
  strip_right.setBrightness(10);
  strip_right.show();

  tcaSelect(RIGHTSENSOR);
  sensor_right.begin();
  delay(delayTime);
}

void loop(){
  tcaSelect(LEFTSENSOR);
  rightSensor = false;
  sensor_left.getRawData(&r,  &g,  &b,  &c);
  Serial.println("LEFT SENSOR");
  // Serial.print("RED: ");
  // Serial.println(r, DEC);
  // Serial.print("GREEN: ");
  // Serial.println(g, DEC);
  // Serial.print("BLUE: ");
  // Serial.println(b, DEC);
  // Serial.println();
  delay(delayTime);
  setSideColors();



  tcaSelect(RIGHTSENSOR);
  rightSensor = true;
  sensor_right.getRawData(&r,  &g,  &b,  &c);
  Serial.println("RIGHT SENSOR");
  // Serial.print("RED: ");
  // Serial.println(r, DEC);
  // Serial.print("GREEN: ");
  // Serial.println(g, DEC);
  // Serial.print("BLUE: ");
  // Serial.println(b, DEC);
  // Serial.println();
  delay(delayTime);
  setSideColors();
}

void setSideColors(){
  int color = checkColor(r,g,b);

  if (color == Color_Type::RED && rightSensor == false){
    // Serial.println("RED");
    setColors(LEFT, red);
    digitalWrite(RED_LEFT_PIN, HIGH);
    digitalWrite(BLUE_LEFT_PIN, LOW);
  }
  if (color == Color_Type::RED && rightSensor == true){
    // Serial.println("RED");
    setColors(RIGHT, red);
    digitalWrite(RED_RIGHT_PIN, HIGH);
    digitalWrite(BLUE_RIGHT_PIN, LOW);
  }

  if (color == Color_Type::BLUE && rightSensor == false){
    // Serial.println("BLUE");
    setColors(LEFT, blue);
    digitalWrite(BLUE_LEFT_PIN, HIGH);
    digitalWrite(RED_LEFT_PIN, LOW);
  }
  if (color == Color_Type::BLUE && rightSensor == true){
    // Serial.println("BLUE");
    setColors(RIGHT, blue);
    digitalWrite(RED_RIGHT_PIN, LOW);
    digitalWrite(BLUE_RIGHT_PIN, HIGH);
  }

  if (color == Color_Type::NONE && rightSensor == false){
    // Serial.println("NONE");
    setColors(LEFT, green);
    digitalWrite(BLUE_LEFT_PIN, LOW);
    digitalWrite(RED_LEFT_PIN, LOW);
  }

  if (color == Color_Type::NONE && rightSensor == true){
    // Serial.println("NONE");
    setColors(RIGHT, green);
    digitalWrite(RED_RIGHT_PIN, LOW);
    digitalWrite(BLUE_RIGHT_PIN, LOW);
   
  }
}

int checkColor(uint32_t red, uint32_t green, uint32_t blue){
  float tempblue = (float) blue;
  float tempred = (float) red;
  float ratio = tempred/tempblue;

  Serial.print("R: ");
  Serial.println(tempred);
  Serial.print("B: ");
  Serial.println(tempblue);
  Serial.print("Ratio:");
  Serial.println(ratio);

  if ((rightSensor == true && digitalRead(RIGHT_ENABLE_PIN) == HIGH) || (rightSensor == false && digitalRead(LEFT_ENABLE_PIN) == HIGH)) {
    if (ratio > midThresh + midThreshWidth) {
      return Color_Type::RED;
    } else if (ratio <  midThresh - midThreshWidth) {
      return Color_Type::BLUE;
    } else {
      return Color_Type::NONE;
    }
  }
  else{
    return Color_Type::NONE;
  }
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
