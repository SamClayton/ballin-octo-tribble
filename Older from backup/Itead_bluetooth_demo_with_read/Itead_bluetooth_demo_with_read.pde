unsigned int timeout=0;
unsigned char state=0;

int incomingByte;
 
ISR(TIMER2_OVF_vect)          //Timer2  Service 
{ 
  TCNT2 = 0;
  timeout++;
  if (timeout>61)
  {
    state=1;
    timeout=0;
  }
 
}
 
void init_timer2(void)               
{
  TCCR2A |= (1 << WGM21) | (1 << WGM20);   
  TCCR2B |= 0x07;   // by clk/1024
  ASSR |= (0<<AS2);  // Use internal clock - external clock not used in Arduino
  TIMSK2 |= 0x01;   //Timer2 Overflow Interrupt Enable
  TCNT2 = 0;
  sei();   
}
 
void setup()
{
  Serial.begin(38400);
  pinMode(2,INPUT);
  // Changed this from 13 to 3, as per datasheet
  // http://iteadstudio.com/store/images/produce/Wireless/BTbee06/BTBEE-V1.0.pdf
  pinMode(3,OUTPUT);
  Serial.print("AT+NAMEblueduino\r\n");
  Serial.end();
  Serial.begin(9600);
  attachInterrupt(0,cleantime,FALLING);
  init_timer2();
}
 
void loop()
{
  /* switch(state)
  {
  case 0:
    digitalWrite(13,LOW);
    break;
  
  case 1:
  */
    if (Serial.available() > 0) {
      incomingByte = Serial.read();
      // This line not necessary?
      // digitalWrite(13,HIGH);
      Serial.print(incomingByte, BYTE);
      // break;
    }
  //}
 
}
 
void cleantime()
{
  timeout=0;
  state=0;
}
