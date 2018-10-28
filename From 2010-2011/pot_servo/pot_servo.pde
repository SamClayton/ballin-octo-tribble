#include <Servo.h>
Servo myservo;
int pos = 0;
const int knobPin = 5;
int knobValue, servoValue;

void setup() {
    myservo.attach(9);
    
    Serial.begin(9600); 
}

void loop() {
  knobValue = analogRead(knobPin);
  servoValue = map(knobValue, 0, 1023, 0, 180);
  
  for (pos = 0; pos < 180; pos +=1) {
    myservo.write(pos);
    delay(15);
  }
  Serial.println(pos);
  for (pos = 180; pos > 1; pos -=1) {
    myservo.write(pos);
    delay(15);
  }
  Serial.println(pos);

}
