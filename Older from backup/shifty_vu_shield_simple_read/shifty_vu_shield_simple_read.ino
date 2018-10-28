/*  Read from the Macetech Shifty VU shield and write value to serial
    adapted from http://docs.macetech.com/doku.php/rusty_vu
*/

/*  Arduino Uno analog pins (from hardware/arduino/variants/standard/pins_arduino.h)
static const uint8_t A0 = 14;
static const uint8_t A1 = 15;
static const uint8_t A2 = 16;
static const uint8_t A3 = 17;
static const uint8_t A4 = 18;
static const uint8_t A5 = 19;
static const uint8_t A6 = 20;
static const uint8_t A7 = 21;
*/

/* Apparently pins 2 and 3 also work here
*/
#define LEFTPIN A2    //A2 a.k.a. 16
#define RIGHTPIN A3   //A3 a.k.a. 17

float adc1avg = 0;
float adc2avg = 0;
float max1 = 0;
float max2 = 0;
float min1 = 0;
float min2 = 0;
unsigned long iter = 0;
float avg1 = 0;
float avg2 = 0;


void setup() {
  // Make SURE that the REFSEL jumper is disconnected/open before running this sketch!
  analogReference(INTERNAL);
  
  Serial.begin(9600);

  // Setting Shifty audio pins (A2 & A3) to input mode
  pinMode(LEFTPIN, INPUT);
  pinMode(RIGHTPIN, INPUT);

  /*
  // ShiftBrite stuff
  pinMode(datapin, OUTPUT);
  pinMode(latchpin, OUTPUT);
  pinMode(enablepin, OUTPUT);
  pinMode(clockpin, OUTPUT);
  
  SPCR = (1<<SPE)|(1<<MSTR)|(0<<SPR1)|(1<<SPR0);
  digitalWrite(latchpin, LOW);
  digitalWrite(enablepin, LOW);
  */
}

void loop() {
  /*
  adc1avg = adc1avg * 0.95 + analogRead(2) * 0.05;
  adc2avg = adc2avg * 0.95 + analogRead(3) * 0.05;
  drawGraph();
  WriteLEDArray();
  delay(1);
  */
  
  /*
  adc1avg = analogRead(LEFTPIN);
  adc2avg = analogRead(RIGHTPIN);
  */
  // This is the Macetech code that has a short-duration average
  adc1avg = adc1avg * 0.95 + analogRead(LEFTPIN) * 0.05;
  adc2avg = adc2avg * 0.95 + analogRead(RIGHTPIN) * 0.05;
  
  Serial.print("adc1avg: ");
  printDouble(adc1avg, 2);
  Serial.print(", ");
  Serial.print("adc2avg: ");
  printDouble(adc2avg, 2);
  //Serial.println("");
  Serial.print(", ");
  
  Serial.print("min1: ");
  printDouble(min1,2);
  Serial.print(", max1: ");
  printDouble(max1,2);
  Serial.print(", avg1: ");
  printDouble(avg1,2);
  Serial.print(", min2: ");
  printDouble(min2, 1);
  Serial.print(", max2: ");
  printDouble(max2,2);
  Serial.print(", avg2: ");
  printDouble(avg2,2);
  //printDouble(adc2avg, 2);
  //printDouble(adc2avg, 2);
  Serial.println("");

  avg1 = ((avg1 * iter) + adc1avg) / (iter + 1);
  avg2 = ((avg2 * iter) + adc2avg) / (iter + 1);
  if (adc1avg > max1)
    max1 = adc1avg;
  if ((adc1avg < min1) || (min1 == 0))
    min1 = adc1avg;
  if (adc2avg > max2)
    max2 = adc2avg;
  if ((adc2avg < min2) || (min2 == 0))
    min2 = adc1avg;
  
  iter++;
  
  delay(1000);
}

void printDouble( double val, byte precision){
  // prints val with number of decimal places determine by precision
  // precision is a number from 0 to 6 indicating the desired decimial places
  // example: lcdPrintDouble( 3.1415, 2); // prints 3.14 (two decimal places)
 
  if(val < 0.0){
    Serial.print('-');
    val = -val;
  }

  Serial.print (int(val));  //prints the int part
  if( precision > 0) {
    Serial.print("."); // print the decimal point
    unsigned long frac;
    unsigned long mult = 1;
    byte padding = precision -1;
    while(precision--)
      mult *=10;
 
      if(val >= 0)
        frac = (val - int(val)) * mult;
      else
        frac = (int(val)- val ) * mult;
      unsigned long frac1 = frac;
      while( frac1 /= 10 )
        padding--;
      while(  padding--)
        Serial.print("0");
      Serial.print(frac,DEC) ;
  }
}
