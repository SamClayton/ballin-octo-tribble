/*
  Read LiPo 3.7v battery voltage on an Elegoo Nano
  then blink the LED a corresponding amount
  Meant to test without having it connected to USB serial/power
  Having issues with the 1:4 voltage dividers readings with 0.1 µF capacitor in parallel with the 2nd 1MΩ resistor
  Based on https://jeelabs.org/2013/05/16/measuring-the-battery-without-draining-it/
*/

#include <math.h>

int led = 13;

// the setup routine runs once when you press reset:
void setup() {
  // Use internal 1.1v reference voltage to scale against
  analogReference(INTERNAL);
  // Not sure if this is necessary but added it in
  pinMode(A6, INPUT);

  // Set the built-in LED for output
  pinMode(led, OUTPUT);
}

void loop() {
  // int sensorValue = analogRead(A6);
  // print out the value you read:
  // Serial.println(sensorValue);

  // with analogReference(INTERNAL);
  // 811 with no battery connected
  // 1023 with battery connected

  // with default reference (5v)
  // 636 with battery (scales to 3.1 v reading, which would mean the undivided voltage was 12.4v) (measured 3.85v on lipo alarm)
  // 190 without battery connected (scales to 0.92v, or 3.71v pre-divider)

  float voltage;
  voltage = analogRead(A6) * 4 * 1.1 / 1024;  

  // with reference default 5v
  // 0.68 with battery plugged in; x4 voltage divider = 2.72v (measured 3.85v on lipo alarm)
  // 0.21 with battery unplugged; x4 voltage divider = 0.84v

  // with reference 1.1v
  // 4.40v with batt plugged in
  // 3.76v with batt unplugged, then settled down to 3.54

  int volts = round(voltage);
  for (int i=0; i<volts; i++) {
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
  }

    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
    delay(200);
    digitalWrite(led, HIGH);
    delay(200);
    digitalWrite(led, LOW);
  

  delay(3000);        // delay in between reads for stability
}
