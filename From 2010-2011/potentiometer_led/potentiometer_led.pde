/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

const int ledOne = 12;
const int ledTwo = 11;
  
const int knobPin = 5;

int knobValue, fadeValue;

void setup() {

  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(ledOne, OUTPUT);
  pinMode(ledTwo, OUTPUT);

}

void loop() {
  knobValue = analogRead(knobPin);
  fadeValue = map(knobValue, 0, 1023, 0 , 254);

  digitalWrite(ledOne, HIGH);   // set the LED on
  delay(100);              // wait for a second
  digitalWrite(ledOne, LOW);    // set the LED off
  delay(100);              // wait for a second

  analogWrite(ledTwo, fadeValue);   // set the LED on
  delay(250);              // wait for a second
  digitalWrite(ledTwo, LOW);    // set the LED off
  delay(250);              // wait for a second
}
