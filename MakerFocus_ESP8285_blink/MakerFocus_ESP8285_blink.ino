// Blue MakerFocus board LED is on pin 16
int PIN = 16;

void setup() {
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, LOW);
}

void loop() {
  digitalWrite(PIN, HIGH);
  delay(500);
  digitalWrite(PIN, LOW);
  delay(1000);
}
