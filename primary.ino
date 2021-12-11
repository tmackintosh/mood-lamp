int LEDPins[] = {3,5,6};
const int adc = 0; //naming pin 0 of analog input side as ‘adc’
int readValue = 0;
int ledValue = 0;
int buttonPin = 2;
int buttonState = 0;
volatile bool on = true;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  for (int i = 0; i < 3; i++){
    pinMode(LEDPins[i],OUTPUT) ; //setting pin 2 as output
  }
}

void loop() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH && on == false) {
    Serial.print("turned on\n");
    buttonState == LOW;
    on = true;
    delay(500);
  }
  if (on == true){
    readValue = analogRead(A0) ; //reading analog voltage and storing it in an integer
    ledValue = map(readValue, 0, 1023, 0, 255);
    for (int j = 0; j < 3; j++){
      buttonState = digitalRead(buttonPin);
      if (buttonState == HIGH && on == true) {
        Serial.print("turned off\n");
        on = false;
      } 
      analogWrite(LEDPins[j],ledValue);
      delay(500);
      digitalWrite(LEDPins[j],LOW);
    }
  }
}
