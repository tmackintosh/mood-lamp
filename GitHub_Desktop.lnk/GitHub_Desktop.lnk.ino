/*
  Button

  Turns on and off a light emitting diode(LED) connected to digital pin 13,
  when pressing a pushbutton attached to pin 2.

  The circuit:
  - LED attached from pin 13 to ground through 220 ohm resistor
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.

  created 2005
  by DojoDave <http://www.0j0.org>
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Button
*/

// constants won't change. They're used here to set pin numbers:
const int buttonPin[] = {2,3,4};     // the number of the pushbutton pin
const int ledPin[] = {5,6,7};      // the number of the LED pin

// variables will change:
int buttonState[] = {LOW,LOW,LOW};         // variable for reading the pushbutton status

void setup() {
  Serial.begin(9600);
  // initialize the LED pin as an output:
  for ( int x; x<3; x++){
    pinMode(ledPin[x], OUTPUT);
  // initialize the pushbutton pin as an input:
    pinMode(buttonPin[x], INPUT);
  }
}

void loop() {
  // read the state of the pushbutton value:
  buttonState[0] = digitalRead(buttonPin[0]);
  buttonState[1] = digitalRead(buttonPin[1]);
  buttonState[2] = digitalRead(buttonPin[2]);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState[0] == HIGH) {
    // turn LED on:
    Serial.print("hello");
    digitalWrite(ledPin[0], HIGH); 
  }
  if (buttonState[1] == HIGH) {
    // turn LED on:
    digitalWrite(ledPin[1], HIGH); 
  }   
  if (buttonState[2] == HIGH) {
    // turn LED on:
    digitalWrite(ledPin[2], HIGH);
    } 
  if (buttonState[0] == LOW) {
    // turn LED off:
    digitalWrite(ledPin[0], LOW);
  }
  if (buttonState[1] == LOW) {
    // turn LED off:
    digitalWrite(ledPin[1], LOW);
  }
  if (buttonState[2] == LOW) {
    // turn LED off:
    digitalWrite(ledPin[2], LOW);
  }
}
