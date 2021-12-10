// Define color sensor pins
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#define sensorOut 8
#include <LiquidCrystal.h>

// use ~560 ohm resistor between Red & Blue, ~1K for green (its brighter)
#define redpin 10
#define greenpin 11
#define bluepin 9
// for a common anode LED, connect the common pin to +5V
// for common cathode, connect the common to ground// set to false if using a common cathode LED
#define commonAnode true// our RGB -> eye-recognized gamma color
byte gammatable[256];
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
// Creates an LCD object. Parameters: (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd = LiquidCrystal(2, 3, 4, 5, 6, 7);

// Variables for Color Pulse Width Measurements
int redPW = 0;
int greenPW = 0;
int bluePW = 0;
int average = 0;
int divisor = 10;


void setup() {
  Serial.begin(9600);
//Serial.println("Color View Test!"); 
  if (tcs.begin()) {
//Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
  while (1); // halt!
  } // use these three pins to drive an LED
  #if defined(ARDUINO_ARCH_ESP32)
    ledcAttachPin(redpin, 1);
    ledcSetup(1, 12000, 8);
    ledcAttachPin(greenpin, 2);
    ledcSetup(2, 12000, 8);
    ledcAttachPin(bluepin, 3);
    ledcSetup(3, 12000, 8);
  #else
    pinMode(redpin, OUTPUT);
    pinMode(greenpin, OUTPUT);
    pinMode(bluepin, OUTPUT);
  #endif // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i=0; i<256; i++) {
  float x = i;
  x /= 255;
  x = pow(x, 2.5);
  x *= 255; if (commonAnode) {
  gammatable[i] = 255 - x;
  } else {
  gammatable[i] = x;
  }
  //Serial.println(gammatable[i]);
  }
  // The commented out code in loop is example of getRawData with clear value.
  // Processing example colorview.pde can work with this kind of data too, but It requires manual conversion to
  // [0-255] RGB value. You can still uncomments parts of colorview.pde and play with clear value.
  
  lcd.begin(16, 2);
  lcd.print("scrollDisplayLeft() example");
  Serial.begin(9600);
  if (tcs.begin()) {
  Serial.println("Found sensor");} 
  else {
  Serial.println("No TCS34725 found ... check your connections");while (1);}// Now we're ready to get readings!



  // Set Pulse Width scaling to 20%


  // Set Sensor output as input
  pinMode(sensorOut, INPUT);

}

void loop() {
  float red, green, blue;
  tcs.setInterrupt(false); // turn on LED delay(900); // takes 50ms to read tcs.getRGB(&red, &green, &blue);
  tcs.setInterrupt(true); // turn off LED Serial.print("R:\t"); Serial.print(int(red));
  Serial.print("\tG:\t"); Serial.print(int(green));
  Serial.print("\tB:\t"); Serial.print(int(blue));// Serial.print("\t");
  // Serial.print((int)red, HEX); Serial.print((int)green, HEX); Serial.print((int)blue, HEX);
  Serial.print("\n");// uint16_t red, green, blue, clear;
  //
  // tcs.setInterrupt(false); // turn on LED
  //
  // delay(60); // takes 50ms to read
  //
  // tcs.getRawData(&red, &green, &blue, &clear);
  //
  // tcs.setInterrupt(true); // turn off LED
  //
  // Serial.print("C:\t"); Serial.print(int(clear));
  // Serial.print("R:\t"); Serial.print(int(red));
  // Serial.print("\tG:\t"); Serial.print(int(green));
  // Serial.print("\tB:\t"); Serial.print(int(blue));
  // Serial.println();
  #if defined(ARDUINO_ARCH_ESP32)
    ledcWrite(1, gammatable[(int)red]);
    ledcWrite(2, gammatable[(int)green]);
    ledcWrite(3, gammatable[(int)blue]);
  #else
    analogWrite(redpin, gammatable[(int)red]);
    analogWrite(greenpin, gammatable[(int)green]);
    analogWrite(bluepin, gammatable[(int)blue]);
  #endif
  uint16_t r, g, b, c, colorTemp, lux;
 
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);
 
  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");

  // Read Red Pulse Width
  redPW = r/divisor ;
  // Delay to stabilize sensor
  delay(200);

  // Read Green Pulse Width
  greenPW = g/divisor;
  // Delay to stabilize sensor
  delay(200);

  // Read Blue Pulse Width
  bluePW = b/divisor;
  // Delay to stabilize sensor
  delay(200);

    

  // Print output to Serial Monitor

  average = ((r + g + b) / 3);

  
  analogWrite(bluepin, r);
  analogWrite(greenpin, b);
  analogWrite(redpin, g);
  if (r > 100){
    lcd.print(" MELLOW ");
    for (int x = 0; x <10; x++){
    lcd.scrollDisplayLeft();
    }
  } else if (b > 100){
    lcd.print("FOCUS");
    for (int x = 0; x <10; x++){
    lcd.scrollDisplayLeft();
    }
  } else if (g > 100){
    lcd.print("ANGRY");
    for (int x = 0; x <10; x++){
    lcd.scrollDisplayLeft();
    }
  }
  
  lcd.scrollDisplayLeft();
  delay(500);
}
