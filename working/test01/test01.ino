// Include different libraries for LCD and RGB sensor
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <LiquidCrystal.h>


// define sensor output pin
#define sensorOut 8
// define LED pins
#define redpin 10
#define greenpin 11
#define bluepin 9
// define and declare variables to store RGB values as gamma value
#define commonAnode true
byte gammatable[256];
//declare RGB sensor object
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
// Create an LCD object
LiquidCrystal lcd = LiquidCrystal(2, 3, 4, 5, 6, 7);



void setup() {
  Serial.begin(9600);
  // Check to find RGB sensor and appropriate output to serial monitor
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    
  while (1); // halt!
  } // use these three pins to drive an LED
  // if statement to attach leds to pins or set their pinmode to output
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
  #endif
  // Calculate gamma values and create gamma table
  for (int i=0; i<256; i++) {
    float x = i;
    x /= 255;
    x = pow(x, 2.5);
    x *= 255; 
    if (commonAnode) {
      gammatable[i] = 255 - x;
    } else {
      gammatable[i] = x;
    }
    Serial.println(gammatable[i]);
  }  
  // Set Sensor output as input
  pinMode(sensorOut, INPUT);
}

void loop() {
  // declare floating point variables for RGB
  float red, green, blue;
  tcs.setInterrupt(false); // set rgb sensor interrupt to false
  tcs.getRGB(&red, &green, &blue); // get rgb values from sensor
  tcs.setInterrupt(true); // turn off LED 
  //output rgb values to serial monitor
  Serial.print("R:\t"); Serial.print(int(red));
  Serial.print("\tG:\t"); Serial.print(int(green));
  Serial.print("\tB:\t"); Serial.print(int(blue));
  Serial.print("\n");

  // output gamma values to LEDs
  #if defined(ARDUINO_ARCH_ESP32)
    ledcWrite(1, gammatable[(int)red]);
    ledcWrite(2, gammatable[(int)green]);
    ledcWrite(3, gammatable[(int)blue]);
  #else
    analogWrite(redpin, gammatable[(int)red]);
    analogWrite(greenpin, gammatable[(int)green]);
    analogWrite(bluepin, gammatable[(int)blue]);
  #endif

  
  uint16_t r, g, b, c, colorTemp, lux; // Ben comment here
  tcs.getRawData(&r, &g, &b, &c); //get rgb values from sensor and store in r,g,b and c
  colorTemp = tcs.calculateColorTemperature(r, g, b); //calculate colour temp and store in variable
  lux = tcs.calculateLux(r, g, b); // calculate lux and store in variable
  
  
  // Print output to Serial Monitor
  Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
  Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  Serial.println(" ");
   
  //when Red is high Green should light up
  if ((r > b) && ( r > g)){
    analogWrite(greenpin, gammatable[(int)green]);
    lcd.print("MELLOW ");
    delay(10);
    analogWrite(greenpin, LOW);
    lcd.scrollDisplayLeft();
    delay(10);
  }
  //When Blue is high Red lights up
  if ((b > r) && (b > g)){
    analogWrite(redpin, gammatable[(int)red]);
    lcd.print("FOCUS ");
    delay(10);
    analogWrite(redpin, LOW);
    lcd.scrollDisplayLeft();
    delay(10);
  }
  
  //When Green is high Blue lights up
  if ((g > r) && (g > b)){
    analogWrite(bluepin, gammatable[(int)blue]);
    lcd.print("ANGRY ");
    delay(10);
    analogWrite(bluepin, LOW);
    lcd.scrollDisplayLeft();
    delay(10);
  }
  // scroll output on lcd to the left
  lcd.scrollDisplayLeft();
  //delay(500);
}
