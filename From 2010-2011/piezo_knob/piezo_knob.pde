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

  Serial.begin(9600); 
}

void loop() {
  knobValue = analogRead(knobPin);
  fadeValue = map(knobValue, 0, 1023, 200, 4000);

  digitalWrite(ledOne, HIGH);   // set the LED on
  //delay(100);              // wait for a second
  //digitalWrite(ledOne, LOW);    // set the LED off
  //delay(100);              // wait for a second

  // Note: higher delay values mean a slower pulse, i.e. a lower tone
  for (long i=0; i < 30000L; i += fadeValue * 2) {
    digitalWrite(ledTwo, HIGH);
    delayMicroseconds(fadeValue);
    digitalWrite(ledTwo, LOW);
    delayMicroseconds(fadeValue);
  }

  digitalWrite(ledOne, LOW);    // set the LED off
  delay(50);
  Serial.println(knobValue);

  /*
  // Play two tones (notes f and d)
  for (long i=0; i < 300000L; i += 2864) {
    digitalWrite(ledTwo, HIGH);
    delayMicroseconds(1432);
    digitalWrite(ledTwo, LOW);
    delayMicroseconds(1432);
  }
  for (long i=0; i < 300000L; i += 3400) {
    digitalWrite(ledTwo, HIGH);
    delayMicroseconds(1700);
    digitalWrite(ledTwo, LOW);
    delayMicroseconds(1700);
  }
  */
  /*
  analogWrite(ledTwo, fadeValue);   // set the LED on
  delay(250);              // wait for a second
  digitalWrite(ledTwo, LOW);    // set the LED off
  delay(250);              // wait for a second
  */
}
