// import libraries
#include <Adafruit_TCS34725.h>

// define constants
#define TCAADDR 0x70
#define LEFTSENSOR 2
#define RIGHTSENSOR 3

// doing the variable stuff (RGBC variables)
uint16_t r;
uint16_t g;
uint16_t b;
uint16_t c;

// addressing multiplexer :)
void tcaSelect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

// initialize sensors
Adafruit_TCS34725 lightSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
void setup() {
  Serial.begin(115200);
  lightSensor.begin();
  tcaSelect(LEFTSENSOR);
  delay(100);
}

// printing RGB variable data
void loop() {
  tcaSelect(LEFTSENSOR);
  lightSensor.getRawData(&r,  &g,  &b,  &c);
  Serial.println("LEFT SENSOR");
  Serial.print("RED: ");
  Serial.println(r, DEC);
  Serial.print("GREEN: ");
  Serial.println(g, DEC);
  Serial.print("BLUE: ");
  Serial.println(b, DEC);
  Serial.println();
  delay(500);

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
  delay(500);
}
