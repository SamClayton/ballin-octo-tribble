#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 11
#define LEDS_LENGTH 50

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS_LENGTH, PIN, NEO_GRB + NEO_KHZ800);

// 1st-9th hoop 1
// 10th in between hooop 1 & 2
// 11th-20th in hoop 2
// 21st in between hoop 2 & 3
// 22nd-33rd in hoop 3
// 34th = in between hoop 3 & 4
// 35th-47th = hoop 4
// 48th just outside of hoop 4
// 49th-50th hang off bottom of hoop 4



void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  //Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  hoopNColor(0, Wheel(200));
  hoopNColor(1, Wheel(100));
  hoopNColor(2, Wheel(50));
  hoopNColor(3, Wheel(0));
  */
  int i, j;
  for (j=0; j<225; j=j+10) {
    for (i=0; i<4; i++) {
      hoopNColor(i, Wheel(j + (10 * i)));
    }
    delay(1000);
  }
}

void hoopNColor(int hoop, uint32_t color) {
  int firstPixel[] = {0, 10, 21, 34};
  int lastPixel[] = {8, 19, 32, 46};

  uint16_t i;
  /* Serial.print("start pixel: ");
  Serial.print(firstPixel[hoop]);
  Serial.print("\nend pixel: ");
  Serial.print(lastPixel[hoop]);
  Serial.print("\n"); */
  // for(i=0; i<strip.numPixels(); i++) {
  for (i=firstPixel[hoop]; i<(lastPixel[hoop]+1); i++) {
    // Serial.println(i);
    strip.setPixelColor(i, color);
    // strip.setPixelColor(i, Wheel(i & 255));
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

