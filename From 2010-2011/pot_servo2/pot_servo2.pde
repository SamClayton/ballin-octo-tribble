#include <Servo.h>
Servo myservo;
int pos = 0;
const int knobPin = 5;
int knobValue;

void setup() {
    myservo.attach(9);
    
    Serial.begin(9600);
    
    // Turn on the power to the pot, since we have the main 5v connected to servo
    digitalWrite(13, HIGH);
    
}

void loop() {
  knobValue = analogRead(knobPin);
  pos = map(knobValue, 0, 1023, 0, 180);
  
  myservo.write(pos);

  Serial.println(pos);
  
  //delay(500);

}
