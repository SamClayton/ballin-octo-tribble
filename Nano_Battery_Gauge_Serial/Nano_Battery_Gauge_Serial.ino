/*
 * Based on NoPartsBatteryGaugeAVR.c
 * http://wp.josh.com/2014/11/06/battery-fuel-guage-with-zero-parts-and-zero-pins-on-avr/
 *
 */ 

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  analogReference(INTERNAL);
  delay(1);

}

void loop() {
  int sensorValue = analogRead(A7);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  // Sam: I've scaled this to the 1.1 internal reference voltage
  float voltage = sensorValue * (1.1 / 1023.0);
  Serial.println(voltage);
  Serial.println(sensorValue);
  delay(1000);      // Pause before next round
}
