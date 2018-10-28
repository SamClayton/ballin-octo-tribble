
#include "Tlc5940.h"

//#define NumLEDs 10 // alternately, could do some math with NUM_TLCS
#define NumLEDs 16 // alternately, could do some math with NUM_TLCS

int LEDChannels[NumLEDs][3] = {0};
int numColors = 4;
/*
int ColorPalette[1][3] = {
  {4096, 200, 2500}
};
*/
int ColorPalette[4][3] = {
  {4000, 0, 4000},      // Less washed-out purple
  //{3000, 0, 4095},      // Less washed-out purple
  //{2176, 160, 2480},      // Less washed-out purple
  //{1024, 0, 4095},      // Boring blue
  {0, 4095, 2096},      // Seafoam
  {2096, 100, 100},      // Hot pink
  //{3088, 500, 3952}     // Washed-out purple - G was 1040
  {4096, 2048, 0}     // Green
};

/*
int ColorPalette[4][3] = {
  {1600, 0, 2048},
  {3000, 200, 4095},
  {3000, 0, 3800},
  {2500, 0, 4000}
};
*/
/*
// These are pretty good purples, on inspection the next day. Some may be a bit too pink.
int ColorPalette[4][3] = {
  {1600, 0, 2048},
  {1700, 0, 2048},
  {1800, 0, 2250},
  {1800, 0, 2500}
};
*/

//int LEDOrder[NumLEDs] = {0, 2, 4, 6, 8, 9, 7, 5, 3, 1};
int LEDOrder[NumLEDs] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

// For hue calc
int R, G, B;

void setup()
{
  /* Call Tlc.init() to setup the tlc.
     You can optionally pass an initial PWM value (0 - 4095) for all channels.*/
  Tlc.init();
  hueToRGB(288, 89);
  /*
  ColorPalette[2][0] = 0;
  ColorPalette[2][1] = 4096;
  ColorPalette[2][2] = 0;
  */
  
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Hello world!");  // prints hello with ending line break 
}

/* This loop will create a Knight Rider-like effect if you have LEDs plugged
   into all the TLC outputs.  NUM_TLCS is defined in "tlc_config.h" in the
   library folder.  After editing tlc_config.h for your setup, delete the
   Tlc5940.o file to save the changes. */
 
void loop() {
  for (int Palette = 0; Palette < numColors; Palette++) {
    for (int LED = 0; LED < NumLEDs; LED += 1) {   // killed incrementor
      Serial.print(Palette);
      Serial.print(" ");
      Serial.println(LED);
      Tlc.clear();
      
      setLED(LEDOrder[LED], ColorPalette[Palette]);
      
      Tlc.update();
      delay(250);
    //ColorPalette[0][0] -= 50;
    }
  }
}

void setLED(int LEDnum, int color[3]) {
  Tlc.set(LEDnum * 3, color[0]);
  Tlc.set((LEDnum * 3) + 1, color[1]);
  Tlc.set((LEDnum * 3) + 2, color[2]);
}

void hueToRGB( int hue, int brightness)
{
    unsigned int scaledHue = (hue * 6);
    unsigned int segment = scaledHue / 256; // segment 0 to 5 around the 
                                            // color wheel
    unsigned int segmentOffset = 
      scaledHue - (segment * 256); // position within the segment

    unsigned int complement = 0;
    unsigned int prev = (brightness * ( 255 -  segmentOffset)) / 256;
    unsigned int next = (brightness *  segmentOffset) / 256;

    /*
    if(invert)
    {
      brightness = 255-brightness;
      complement = 255;
      prev = 255-prev;
      next = 255-next;
    }
    */

    switch(segment ) {
    case 0:      // red
        R = brightness;
        G = next;
        B = complement;
    break;
    case 1:     // yellow
        R = prev;
        G = brightness;
        B = complement;
    break;
    case 2:     // green
        R = complement;
        G = brightness;
        B = next;
    break;
    case 3:    // cyan
        R = complement;
        G = prev;
        B = brightness;
    break;
    case 4:    // blue
        R = next;
        G = complement;
        B = brightness;
    break;
   case 5:      // magenta
    default:
        R = brightness;
        G = complement;
        B = prev;
    break;
    }
}

