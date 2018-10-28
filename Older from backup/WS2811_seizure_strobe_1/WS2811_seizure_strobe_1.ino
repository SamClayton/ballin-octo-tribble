/*  OctoWS2811 Seizure Strobe Up
    Copyright (c) 2015 Sam Clayton

  http://www.pjrc.com/teensy/td_libs_OctoWS2811.html

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
// const int ledsPerStrip = 16;
const int ledsPerStrip = 60;
const int AcLedsPerCrystal = 12;

#define OFF 0x000000
#define RED 0xFF0000
#define WHITE 0xFFFFFF

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

  Serial.begin(9600);
}


void loop() {
  //rainbow(10, 7500);
  // blinkFirstGreen();
  // blinkLastGreen();
  // cycleDownOneColor(RED);
  // cycleUpOneColor(RED);
  //cycleChaseColors(20);
  // flickerRandomPixel(2000, 50, RED);
  // flickerRandomPixel(2000, 50, rainbowColors[random(179)]);
  //lightAllUpRandomly(2000, 200, RED);
  //flickerNRandomPixels(500, 100, RED, 4);
  // flickerNRandomPixelsRandomColors(500, 20,4);
  // flickerNRandomPixelsRandomColors(random(100, 1000), random(5,100), random(2, 7));
  //flickerNRandomPixelsRandomColors(random(100, 500), random(3,33), random(8, ledsPerStrip-1));
  //seizureStrobeUp();
  //seizureStrobeUpBlue();
  seizureStrobeUpRandomColorEachFlash();
}

void seizureStrobeUp() {
  for (int i=0x000000; i < 0xFFFFFF; i=i+ 0x010101) {
    for (int j=0; j<ledsPerStrip; j++) {
      leds.setPixel(j, i);
    }
    leds.show();
    delay(50);
    for (int j=0; j<ledsPerStrip; j++) {
      leds.setPixel(j, OFF);
    }
    leds.show();
    delay(50);
  }
}

void seizureStrobeUpBlue() {
  int hue = 160;
  for (int i=5; i < 50; i++) {
    for (int j=0; j<ledsPerStrip; j++) {
      leds.setPixel(j, makeColor(hue, 100, i));
    }
    leds.show();
    delay(50);
    for (int j=0; j<ledsPerStrip; j++) {
      leds.setPixel(j, OFF);
    }
    leds.show();
    delay(50);
  }
}

void flickerStrobeUpBlue() {
  int hue = 160;
  for (int i=5; i < 50; i++) {
    for (int j=0; j<ledsPerStrip; j++) {
      leds.setPixel(j, makeColor(hue, 100, i));
    }
    leds.show();
    delay(10);
    for (int j=0; j<ledsPerStrip; j++) {
      leds.setPixel(j, OFF);
    }
    leds.show();
    delay(10);
  }
}


void seizureStrobeUpRandomColorEachFlash() {
  int hue = 0;
  for (int i=5; i < 75; i++) {
    hue = random(359);
    for (int j=0; j<ledsPerStrip; j++) {
      leds.setPixel(j, makeColor(hue, 100, i));
    }
    leds.show();
    delay(2*i);
    for (int j=0; j<ledsPerStrip; j++) {
      leds.setPixel(j, OFF);
    }
    leds.show();
    delay(2*i);
  }
}


void seizureStrobeUpRainbowColors() {
  for (int i=5; i < 75; i++) {
    for (int j=0; j<ledsPerStrip; j++) {
      leds.setPixel(j, makeColor(random(359), 100, i));
    }
    leds.show();
    delay(2*i);
    for (int j=0; j<ledsPerStrip; j++) {
      leds.setPixel(j, OFF);
    }
    leds.show();
    delay(2*i);
  }
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

// Very spazzy at (1, 200, N)
void flickerRandomPixel(int duration, int onTime, int color) {
  // duration and onTime in milliseconds
  unsigned long endTime = millis() + duration;
  int chosenPixel = random(ledsPerStrip-1);
  Serial.println(endTime);
  while (millis() < endTime) {
    leds.setPixel(chosenPixel, color);
    leds.show();
    delay(onTime);
    leds.setPixel(chosenPixel, OFF);
    leds.show();
    delay(onTime);
    // Serial.println(millis());
  }
  // Serial.println("exiting flickerRandomPixel");
}

void lightAllUpRandomly(int duration, int onTime, int color) {
  // duration and onTime in milliseconds
  unsigned long endTime = millis() + duration;
  while (millis() < endTime) {
    int chosenPixel = random(ledsPerStrip-1);
    leds.setPixel(chosenPixel, color);
    leds.show();
    delay(onTime);
  }
}

void flickerNRandomPixels(int duration, int onTime, int color, int numAffected) {
  // duration and onTime in milliseconds
  unsigned long endTime = millis() + duration;
  int chosenPixels[numAffected];
  randomPixels(chosenPixels, numAffected);
  while (millis() < endTime) {
    setPixelsOneVal(chosenPixels, numAffected, color);
    leds.show();
    delay(onTime);
    setPixelsOneVal(chosenPixels, numAffected, OFF);
    leds.show();
    delay(onTime);
  }
  // Serial.println("exiting flickerNRandomPixels");
}

void flickerNRandomPixelsRandomColors(int duration, int onTime, int numAffected) {
  // duration and onTime in milliseconds
  unsigned long endTime = millis() + duration;
  int chosenPixels[numAffected];
  randomPixels(chosenPixels, numAffected);
  while (millis() < endTime) {
    setPixelsRandomVals(chosenPixels, numAffected);
    leds.show();
    delay(onTime);
    setPixelsOneVal(chosenPixels, numAffected, OFF);
    leds.show();
    delay(onTime);
  }
  // Serial.println("exiting flickerNRandomPixels");
}

void randomPixels(int chosenPixels[], int numToSelect) {
  // int chosenPixels[numToSelect];
  int rnd;
  chosenPixels[0] = random(ledsPerStrip - 1);
  Serial.print(chosenPixels[0]);
  Serial.print(" ");
  for (int i=1; i<numToSelect; i++) {
    do {
      rnd = random(ledsPerStrip-1);
    } while (inArray(rnd, chosenPixels, i));
    chosenPixels[i] = rnd;
    Serial.print(chosenPixels[i]);
    Serial.print(" ");
  }
  Serial.println();
}

boolean inArray(int value, int pixels[], int arraySize) {
  for (int i=0; i<arraySize; i++) {
    if (pixels[i] == value) {
      Serial.print("generated redundant value");
      return true;
    }
  }
  return false;
}

void setPixelsOneVal(int pixelNums[], int arraySize, int color) {
  for (int i=0; i<arraySize; i++) {
    leds.setPixel(pixelNums[i], color);
  }
}

void setPixelsRandomVals(int pixelNums[], int arraySize) {
  for (int i=0; i<arraySize; i++) {
    leds.setPixel(pixelNums[i], rainbowColors[random(179)]);
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
/*
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
*/

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
