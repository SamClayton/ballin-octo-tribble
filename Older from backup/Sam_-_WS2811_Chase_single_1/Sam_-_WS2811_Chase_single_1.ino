/*  OctoWS2811 Chase
    http://www.pjrc.com/teensy/td_libs_OctoWS2811.html
    Copyright (c) 2015 Sam Clayton

  Required Connections
  --------------------
    pin 2:  LED Strip #1    OctoWS2811 drives 8 LED Strips.
    pin 14: LED strip #2    All 8 are the same length.
    pin 7:  LED strip #3
    pin 8:  LED strip #4    A 100 ohm resistor should used
    pin 6:  LED strip #5    between each Teensy pin and the
    pin 20: LED strip #6    wire to the LED strip, to minimize
    pin 21: LED strip #7    high frequency ringining & noise.
    pin 5:  LED strip #8
    pin 15 & 16 - Connect together, but do not use
    pin 4 - Do not use
    pin 3 - Do not use as PWM.  Normal use is ok.
    pin 1 - Output indicating CPU usage, monitor with an oscilloscope,
            logic analyzer or even an LED (brighter = CPU busier)
*/

#include <OctoWS2811.h>

// Initially set to size of "Bc" crystal prototype
const int ledsPerStrip = 16;

#define OFF 0x000000
#define RED 0xFF0000

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

// pre-soldered WS2812B strands from Aliexpress are WS2811_GRB | WS2811_800KHZ
const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

// Configuration - the first digit here is the # of seconds 
int wait = 0.05 * 2000000;
int rainbowColors[180];

// Map of pixels in a "Bc" crystal, bottom to top
int pixelMap[ledsPerStrip] = {5, 4, 15,  6, 3, 14,  7, 2, 13,  8, 1, 12,  9, 0, 11,  10};


void setup() {
  pinMode(1, OUTPUT);
  digitalWrite(1, HIGH);
  // pre-compute the 180 rainbow colors
  for (int i=0; i<180; i++) {
    int hue = i * 2;
    int saturation = 100;
    int lightness = 50;
    rainbowColors[i] = makeColor(hue, saturation, lightness);
  }
  digitalWrite(1, LOW);
  leds.begin();
}


void loop() {
  //rainbow(10, 7500);
  // blinkFirstGreen();
  // blinkLastGreen();
  // cycleUpDown();
  // cycleDownOneColor(RED);
  // cycleUpOneColor(RED);
  cycleChaseColors(20);
}

void blinkFirstGreen() {
  // this block just blinks the first LED green
  leds.setPixel(0, rainbowColors[55]);
  leds.show();
  delayMicroseconds(wait);
  leds.setPixel(0, OFF);
  leds.show();
  delayMicroseconds(wait);
}

void blinkLastGreen() {
  // this block just blinks the last LED green
  leds.setPixel(ledsPerStrip-1, rainbowColors[55]);
  leds.show();
  delayMicroseconds(wait);
  leds.setPixel(ledsPerStrip-1, OFF);
  leds.show();
  delayMicroseconds(wait);
}

void cycleDownOneColor(int color) {
    for (int i=(ledsPerStrip-1); i >= 0 ; i--) { // was i < leds.numPixels()
    // rainbowColors[0] is a saturated RED when correct byte ordering used
      leds.setPixel(pixelMap[i], color);
      leds.show();
      delayMicroseconds(wait);
      leds.setPixel(pixelMap[i], OFF);
      leds.show();
      delayMicroseconds(wait);
    }
}

void cycleUpOneColor(int color) {
    for (int i=0; i < ledsPerStrip ; i++) {
    // rainbowColors[0] is a saturated RED when correct byte ordering used
      leds.setPixel(pixelMap[i], color);
      leds.show();
      delayMicroseconds(wait);
      leds.setPixel(pixelMap[i], OFF);
      leds.show();
      delayMicroseconds(wait);
    }
}


void cycleChaseColors(int steps) {
  // steps is the number by which to decrement the color grabbed from the rainbow array each cycle
  for (int j=0; j < 180; j=j+steps) {
    cycleUpOneColor(rainbowColors[j]);
    cycleDownOneColor(rainbowColors[j]);
  }
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
  int color, x, y, wait;
  // int offset

  wait = cycleTime * 1000 / ledsPerStrip;
  for (color=0; color < 180; color++) {
    digitalWrite(1, HIGH);
    for (x=0; x < ledsPerStrip; x++) {
      for (y=0; y < 8; y++) {
        int index = (color + x + y*phaseShift/2) % 180;
        leds.setPixel(x + y*ledsPerStrip, rainbowColors[index]);
      }
    }
    leds.show();
    digitalWrite(1, LOW);
    delayMicroseconds(wait);
  }
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
