// Define color sensor pins
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#define sensorOut 8
#include <LiquidCrystal.h>



// Creates an LCD object. Parameters: (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd = LiquidCrystal(2, 3, 4, 5, 6, 7);







int ledPin1 = 9;    // LED connected to digital pin 9 // blue
int ledPin2 = 10; // green
int ledPin3 = 11; // red

// Variables for Color Pulse Width Measurements
int redPW = 0;
int greenPW = 0;
int bluePW = 0;
int average = 0;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);


void setup() {
  lcd.begin(16, 2);
  lcd.print("scrollDisplayLeft() example");
  Serial.begin(9600);
  if (tcs.begin()) {
  Serial.println("Found sensor");} 
  else {
  Serial.println("No TCS34725 found ... check your connections");while (1);}// Now we're ready to get readings!

  // Set S0 - S3 as outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Set Pulse Width scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  // Set Sensor output as input
  pinMode(sensorOut, INPUT);

}

void loop() {

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
  redPW = r/24 ;
  // Delay to stabilize sensor
  delay(200);

  // Read Green Pulse Width
  greenPW = g/24;
  // Delay to stabilize sensor
  delay(200);

  // Read Blue Pulse Width
  bluePW = b/24;
  // Delay to stabilize sensor
  delay(200);

    

  // Print output to Serial Monitor
  Serial.print("Red PW = ");
  Serial.print(redPW);
  Serial.print(" - Green PW = ");
  Serial.print(greenPW);
  Serial.print(" - Blue PW = ");
  Serial.println(bluePW);
  average = ((bluePW + greenPW + redPW) / 3)

  
  analogWrite(ledPin1, redPW);
  analogWrite(ledPin2, bluePW);
  analogWrite(ledPin3, greenPW);
  if (redPW > 100){
    lcd.scrollDisplayLeft("MELLOW");
  } else if (bluePW > 100){
    lcd.scrollDisplayLeft("FOCUS");
  } else if (greenPW > 100){
    lcd.scrollDisplayLeft("ANGRY");
  }
  
  lcd.scrollDisplayLeft();
  delay(500);
}
