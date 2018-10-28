/* Skate light controller
 * Monitors 3.7v LiPo battery voltage. Turns LEDs off when voltage drops below 3.2v.
 * Elegoo Nano based device with Wolfwoop 5v step-up voltage regulator (says it only works down to 3.5v)
 * Uses a 1:4 voltage divider circuit with 0.1 µF capacitor in parallel with the last 1MΩ resistor
*/

#include <Adafruit_NeoPixel.h>

// WS2811 data pin
#define PIN 11
const int numLeds = 6;

// LiPo monitoring pin
const int lipoPin = A0;
// LiPo warning voltage - LEDs blink red
const float warningVoltage = 3.20;
// LiPo cutoff voltage - WARNING never below 3.0v!!!
const float cutoffVoltage = 3.10;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLeds, PIN, NEO_GRB + NEO_KHZ800);

// the setup routine runs once when you press reset:
void setup() {
  // Use internal 1.1v reference voltage to scale against
  analogReference(INTERNAL);
  pinMode(lipoPin, INPUT);
  // Take a reading and throw it out
  analogRead(lipoPin);

  Serial.begin(9600);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  float voltage = readLipoVoltageAvg();
  if (voltage > warningVoltage) {
    rainbow(20);
  } else if ((voltage > cutoffVoltage) && (voltage < warningVoltage)) {
    // Blink red
    blinkRed();
    delay(1000);
  } else {
    // Explicitly cut the lights off so we don't leave them on, as is default
    blank();
    delay(3000);
  }
}

float readLipoVoltage() {
  float voltage;
  voltage = (float) analogRead(lipoPin) * 4 * 1.1 / 1024;
  Serial.println(voltage);
  return voltage;  
}

float readLipoVoltageAvg() {
  // int readings[6];
  int readings = 0, numReadings = 6;
  for (int i=0; i<numReadings; i++) {
    readings += analogRead(lipoPin);
    delay(10);
  }
  float voltage;
  voltage = (float) readings / (float) numReadings * 4.0 * 1.1 / 1024.0;
  Serial.println(voltage);
  return voltage;
}

void blinkRed() {
  for (int i=0; i<10; i++) {
    showRed();
    delay(200);
    blank();
    delay(200);
  }
}

void showRed() {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(0 & 255));
    }
    strip.show();
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void blank() {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, 0);
    }
    strip.show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
/*
 * 200 = a nice lavender purple
 * 100 = teal
 * 50 = minty to grass green
 * 0 = red
 */
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
