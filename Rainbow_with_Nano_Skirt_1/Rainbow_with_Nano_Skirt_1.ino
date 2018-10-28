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

const int ledsPerStrip = LEDS_LENGTH;
int rainbowColors[180];

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  for (int i=0; i<180; i++) {
    int hue = i * 2;
    int saturation = 100;
    int lightness = 50;
    // pre-compute the 180 rainbow colors
    rainbowColors[i] = makeColor(hue, saturation, lightness);
  }
}

void loop() {
  // TODO this is an attempt to port my old Teensy/OctoWS2811 rainbow to NeoPixel/Nano
  // I think there's a problem with the rainbowColors data type feeding into the NeoPixel color type
  // Casting colors didn't work
  rainbow(10, 2500);
}


// phaseShift is the shift between each row.  phaseShift=0
// causes all rows to show the same colors moving together.
// phaseShift=180 causes each row to be the opposite colors
// as the previous.
//
// cycleTime is the number of milliseconds to shift through
// the entire 360 degrees of the color wheel:
// Red -> Orange -> Yellow -> Green -> Blue -> Violet -> Red
//
void rainbow(int phaseShift, int cycleTime)
{
  int color, x, y, offset, wait;

  wait = cycleTime * 1000 / ledsPerStrip;
  for (color=0; color < 180; color++) {
    // digitalWrite(1, HIGH);
    for (x=0; x < ledsPerStrip; x++) {
      for (y=0; y < 8; y++) {
        int index = (color + x + y*phaseShift/2) % 180;
        // leds.setPixel(x + y*ledsPerStrip, rainbowColors[index]);
        // strip.setPixelColor(x + y*ledsPerStrip, (uint32_t) rainbowColors[index]);
        strip.setPixelColor(x + y*ledsPerStrip, Wheel(rainbowColors[index]));
      }
    }
    strip.show();
    delayMicroseconds(wait);
  }
}

void hoopDownStrobe() {
  int i, j;
  for (j=0; j<255; j=j+20) {
    for (i=0; i<4; i++) {
      hoopNColor(i, Wheel(j));
      delay(150);
      hoopNColor(i, 0);
    }
    delay(20);
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


// Convert HSL (Hue, Saturation, Lightness) to RGB (Red, Green, Blue)
//
//   hue:        0 to 359 - position on the color wheel, 0=red, 60=orange,
//                            120=yellow, 180=green, 240=blue, 300=violet
//
//   saturation: 0 to 100 - how bright or dull the color, 100=full, 0=gray
//
//   lightness:  0 to 100 - how light the color is, 100=white, 50=color, 0=black
//
int makeColor(unsigned int hue, unsigned int saturation, unsigned int lightness)
{
  unsigned int red, green, blue;
  unsigned int var1, var2;

  if (hue > 359) hue = hue % 360;
  if (saturation > 100) saturation = 100;
  if (lightness > 100) lightness = 100;

  // algorithm from: http://www.easyrgb.com/index.php?X=MATH&H=19#text19
  if (saturation == 0) {
    red = green = blue = lightness * 255 / 100;
  } else {
    if (lightness < 50) {
      var2 = lightness * (100 + saturation);
    } else {
      var2 = ((lightness + saturation) * 100) - (saturation * lightness);
    }
    var1 = lightness * 200 - var2;
    red = h2rgb(var1, var2, (hue < 240) ? hue + 120 : hue - 240) * 255 / 600000;
    green = h2rgb(var1, var2, hue) * 255 / 600000;
    blue = h2rgb(var1, var2, (hue >= 120) ? hue - 120 : hue + 240) * 255 / 600000;
  }
  return (red << 16) | (green << 8) | blue;
}

unsigned int h2rgb(unsigned int v1, unsigned int v2, unsigned int hue)
{
  if (hue < 60) return v1 * 60 + (v2 - v1) * hue;
  if (hue < 180) return v2 * 60;
  if (hue < 240) return v1 * 60 + (v2 - v1) * (240 - hue);
  return v1 * 60;
}

// alternate code:
// http://forum.pjrc.com/threads/16469-looking-for-ideas-on-generating-RGB-colors-from-accelerometer-gyroscope?p=37170&viewfull=1#post37170

