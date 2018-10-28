#include <FastLED.h>

// How many leds in your strip?
// Hoop has 50 total
#define NUM_LEDS 50

// 1st-9th hoop 1
// 10th in between hooop 1 & 2
// 11th-20th in hoop 2
// 21st in between hoop 2 & 3
// 22nd-33rd in hoop 3
// 34th = in between hoop 3 & 4
// 35th-47th = hoop 4
// 48th just outside of hoop 4
// 49th-50th hang off bottom of hoop 4

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 11
// #define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
	Serial.begin(57600);
	Serial.println("resetting");
	LEDS.addLeds<WS2812,DATA_PIN,GRB>(leds,NUM_LEDS);
	LEDS.setBrightness(84);
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void loop() { 
	static uint8_t hue = 0;
	Serial.print("x");
	// First slide the led in one direction
	for(int i = 0; i < NUM_LEDS; i++) {
		// Set the i'th led to red 
		leds[i] = CHSV(hue++, 255, 255);
		// Show the leds
		FastLED.show(); 
		// now that we've shown the leds, reset the i'th led to black
		// leds[i] = CRGB::Black;
		fadeall();
		// Wait a little bit before we loop around and do it again
		delay(10);
	}
	Serial.print("x");

	// Now go in the other direction.  
	for(int i = (NUM_LEDS)-1; i >= 0; i--) {
		// Set the i'th led to red 
		leds[i] = CHSV(hue++, 255, 255);
		// Show the leds
		FastLED.show();
		// now that we've shown the leds, reset the i'th led to black
		// leds[i] = CRGB::Black;
		fadeall();
		// Wait a little bit before we loop around and do it again
		delay(10);
	}
}
