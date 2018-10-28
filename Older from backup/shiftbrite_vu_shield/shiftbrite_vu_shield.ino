/* From the Rusty VU project http://docs.macetech.com/doku.php/rusty_vu */

#define clockpin 13 // CI
#define enablepin 10 // EI
#define latchpin 9 // LI
#define datapin 11 // DI
 
#define NumLEDs 1
 
int LEDChannels[NumLEDs][3] = {0};
int SB_CommandMode;
int SB_RedCommand;
int SB_GreenCommand;
int SB_BlueCommand;

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
 
   pinMode(datapin, OUTPUT);
   pinMode(latchpin, OUTPUT);
   pinMode(enablepin, OUTPUT);
   pinMode(clockpin, OUTPUT);
   SPCR = (1<<SPE)|(1<<MSTR)|(0<<SPR1)|(0<<SPR0);
   digitalWrite(latchpin, LOW);
   digitalWrite(enablepin, LOW);
 
   pinMode(2, INPUT);
   pinMode(A3, INPUT);
   //digitalWrite(A2, HIGH);
 
 analogReference(INTERNAL); 
 
  Serial.begin(9600); 
}
 
void SB_SendPacket() {
 
    if (SB_CommandMode == B01) {
     SB_RedCommand = 120;
     SB_GreenCommand = 100;
     SB_BlueCommand = 100;
    }
 
    SPDR = SB_CommandMode << 6 | SB_BlueCommand>>4;
    while(!(SPSR & (1<<SPIF)));
    SPDR = SB_BlueCommand<<4 | SB_RedCommand>>6;
    while(!(SPSR & (1<<SPIF)));
    SPDR = SB_RedCommand << 2 | SB_GreenCommand>>8;
    while(!(SPSR & (1<<SPIF)));
    SPDR = SB_GreenCommand;
    while(!(SPSR & (1<<SPIF)));
 
}
 
void WriteLEDArray() {
 
    SB_CommandMode = B00; // Write to PWM control registers
    for (int h = 0;h<NumLEDs;h++) {
	  SB_RedCommand = LEDChannels[h][0];
	  SB_GreenCommand = LEDChannels[h][1];
	  SB_BlueCommand = LEDChannels[h][2];
	  SB_SendPacket();
    }
 
    delayMicroseconds(15);
    digitalWrite(latchpin,HIGH); // latch data into registers
    delayMicroseconds(15);
    digitalWrite(latchpin,LOW);
 
    SB_CommandMode = B01; // Write to current control registers
    for (int z = 0; z < NumLEDs; z++) SB_SendPacket();
    delayMicroseconds(15);
    digitalWrite(latchpin,HIGH); // latch data into registers
    delayMicroseconds(15);
    digitalWrite(latchpin,LOW);
 
}
 
void loop() {
 
   LEDChannels[0][0] = 1023;
   LEDChannels[0][1] = 0;
   LEDChannels[0][2] = 0;
 
   LEDChannels[1][0] = 0;
   LEDChannels[1][1] = 0;
   LEDChannels[1][2] = 1023;
 
   WriteLEDArray();
   delay(200);
 
   LEDChannels[0][0] = 0;
   LEDChannels[0][1] = 0;
   LEDChannels[0][2] = 1023;
 
   LEDChannels[1][0] = 1023;
   LEDChannels[1][1] = 0;
   LEDChannels[1][2] = 0;
 
   WriteLEDArray();
   delay(200);

   LEDChannels[0][0] = 0;
   LEDChannels[0][1] = 1023;
   LEDChannels[0][2] = 0;
 
   LEDChannels[1][0] = 1023;
   LEDChannels[1][1] = 0;
   LEDChannels[1][2] = 0;
 
   //WriteLEDArray();
   

   while(true) {
     adc1avg = analogRead(A2);
     adc2avg = analogRead(2);
     //adc1avg = adc1avg * 0.95 + analogRead(2) * 0.05;
     //adc2avg = adc2avg * 0.95 + analogRead(3) * 0.05;
     //printDouble(adc1avg, 2);
     Serial.print("min1: ");
     printDouble(min1,2);
     Serial.print(", max1: ");
     printDouble(max1,2);
     Serial.print(", avg1: ");
     printDouble(avg1,2);
     Serial.print(", val1: ");
     printDouble(adc1avg, 1);
     //printDouble(adc2avg, 2);
     //printDouble(adc2avg, 2);
     Serial.println("");
     
     avg1 = ((avg1 * iter) + adc1avg) / (iter + 1);
     if (adc1avg > max1)
       max1 = adc1avg;
     if ((adc1avg < min1) || (min1 == 0))
       min1 = adc1avg;
     
     iter++;
     
     delay(200);
     //Serial.write(adc2avg.toString());
   }
   
   // delay(200);

 
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
