/* Cycle through each color on all 16 LEDs simultaneously. */


/* NUM_TLCS is defined in "tlc_config.h" in the library folder.  After editing tlc_config.h 
   for your setup, delete the
   Tlc5940.o file to save the changes. */
#include "Tlc5940.h"

// For hue calc
int R, G, B;

void setup()
{
  /* Call Tlc.init() to setup the tlc.
     You can optionally pass an initial PWM value (0 - 4095) for all channels.*/
  Tlc.init(4095);
  delay(500);
  
  Serial.begin(9600);
  Serial.print("Setup done with "); Serial.print(NUM_TLCS); Serial.println(" TLCs!");
  delay(100);
}
 
void loop() {
  //setFirstDozenRed();
  
  /*
  redChaser();
  greenChaser();
  blueChaser();
  */

  // First 2 set to blue
  //BlinkFirstTwoBlue();
  
  /*
  setAllRed();
  Serial.println("Red, waiting...");
  delay(500);
  */
  /*
  Tlc.clear();
  Tlc.set(0, 4095);
  Tlc.update();
  delay(500);
  */
  
  setAllBlue();
  Serial.println("Blue, waiting...");
  delay(500);
  setAllGreen();
  Serial.println("Green, waiting...");
  delay(500);
  int pink[3] = {2096, 100, 100};
  setAllPhysLEDs(pink);
  Serial.println("Pink, waiting...");
  delay(3000);
  setAllPhysLEDs(4000,0,4000);
  Serial.println("Purple, waiting...");
  delay(3000);

  
  /*
  setAllRed();
  Serial.println("Red, waiting...");
  delay(500);
  */
}

void redChaser() {
  eachChannelLoop(100, 3, 0);
}

void greenChaser() {
  eachChannelLoop(100, 3, 1);
}

void blueChaser() {
  eachChannelLoop(100, 3, 2);
}

void eachChannelLoop(int delayTime, int skipChannels, int firstChannel) {
  for (int i=firstChannel; i<(NUM_TLCS*16); i+=skipChannels) {
    Serial.println(i);
    Tlc.clear();
    Tlc.set(i, 4095);
    Tlc.update();
    delay(delayTime);
  }
}

void eachChannelLoop() {
  eachChannelLoop(500, 1, 0);
}

void BlinkFirstTwoBlue() {
  Tlc.clear();
  Tlc.set(2, 4095);
  Tlc.update();
  delay(5000);
  Tlc.set(5, 4095);
  Tlc.update();
  delay(5000);
}

void setFirstDozenRed() {
  /* Because REASONS (?!), this causes my board to reboot after the call to Tlc.update() */
  Tlc.clear();
  for (int i=0; i<16; i+=3) {
    Serial.println(i);
    Tlc.set(i, 4000);
    delay(15);
  }
  Tlc.update();
  delay(5000);
}

void setAllRed() {
  /* Because REASONS (?!), this causes my board to reboot after the call to Tlc.update() 
     Strangely, setAllBlue(), setAllGreen(), and setAllWhite() work just fine, and I can 
     cycle between them with no issues. */
  int red[3] = {4095,0,0};
  setAllPhysLEDs(red);
}

void setAllBlue() {
  int blue[3] = {0,0,4095};
  setAllPhysLEDs(blue);
}

void setAllGreen() {
  int green[3] = {0,4095,0};
  setAllPhysLEDs(green);
}

void setAllWhite() {
  int white[3] = {4095,4095,4095};
  setAllPhysLEDs(white);
}

void setAllPhysLEDs(int color[3]) {
  Tlc.clear();
  int physLEDs = NUM_TLCS / 3 * 16;
  for (int physLED = 0; physLED < physLEDs; physLED++) {
    setPhysLED(physLED, color);
  }
  Tlc.update();
}

void setAllPhysLEDs(int red, int green, int blue) {
  int color[3] = {red, green, blue};
  Tlc.clear();
  int physLEDs = NUM_TLCS / 3 * 16;
  for (int physLED = 0; physLED < physLEDs; physLED++) {
    setPhysLED(physLED, color);
  }
  Tlc.update();
}

void setPhysLED(int physLEDnum, int color[3]) {
  //Serial.print(physLEDnum*3); Serial.print(": "); Serial.print(color[0]); Serial.print(", "); Serial.print(color[1]); Serial.print(", "); Serial.print(color[2]); 
  //Serial.println("");

  Tlc.set(physLEDnum * 3, color[0]);
  printChannelAndBrightness(physLEDnum * 3, color[0]);
  Tlc.set((physLEDnum * 3)+1, color[1]);
  printChannelAndBrightness((physLEDnum * 3)+1, color[1]);
  Tlc.set((physLEDnum * 3)+2, color[2]);
  printChannelAndBrightness((physLEDnum * 3)+2, color[2]);
}

void printChannelAndBrightness(int channel, int brightness){
  Serial.print(channel);
  Serial.print(": ");
  Serial.println(brightness);
}

void setPhysLED(int physLEDnum, int hue, int brightness) {
  hueToRGB(hue, brightness);
  int color[3] = {R,G,B};
  Tlc.set(physLEDnum * 3, color[0]);
  Tlc.set((physLEDnum * 3)+1, color[1]);
  Tlc.set((physLEDnum * 3)+2, color[2]);
  
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

