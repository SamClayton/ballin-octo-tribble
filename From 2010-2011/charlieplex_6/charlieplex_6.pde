const int led[6][3] =
  {
    {0, 1, -1},
    {1, 0, -1},
    {-1, 0, 1},
    {-1, 1, 0},
    {0, -1, 1},
    {1, -1, 0}
  };
  
const int ledOffset[6] = {2, 1, 3, 4, 5, 0};

const int delayAmount = 200;

void setup() {
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  
  //Serial.begin(9600); 
}

void loop() {
  /*
  for (int n=0; n<6; n++) {
    ledOn(n);
    delay (1000);
  }
  digitalWrite(5, HIGH);
  digitalWrite(3, LOW);
  */
  
  /*
  for (int x=-1; x<2; x++) {
    for (int y=-1; y<2; y++) {
      for (int z=-1; z<2; z++) {
        ledGuess(x, y, z);
        delay(300);
        //Serial.println(x);
        //Serial.println(y);
        //Serial.println(z);
      }
    }
  }
  */
  
  int adjustedLed;
  for (int x=0; x<6; x++) {
    ledOn(ledOffset[x]);
    delay(delayAmount);
  }
  
  
  for (int x=4; x>=1
  ; x--) {
    ledOn(ledOffset[x]);
    delay(delayAmount);
  }
  

  // http://uzimonkey.blogspot.com/2009/01/charlieplexing-on-arduino.html
  // LEDs 1 & 3: 5 = HIGH, 3 = LOW, 4 = INPUT
  /*
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  digitalWrite(3, LOW);
  pinMode(4, INPUT);
  digitalWrite(4, LOW);
  
  delay(400);
  
  // LED 1 only
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(5, HIGH);
  digitalWrite(4, LOW);
  pinMode(3, INPUT);
  digitalWrite(3, LOW);

  delay(400);

  // LED 3 only
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(4, HIGH);
  digitalWrite(3, LOW);
  pinMode(5, INPUT);
  digitalWrite(5, LOW);

  delay(400);
  */
  
}

void ledOn(int i) {
  /*
  if (led[i][0]) {
    digitalWrite(3, HIGH);
  } else {
    digitalWrite(3, LOW);
  }
  if (led[i][1]) {
    digitalWrite(4, HIGH);
  } else {
    digitalWrite(4, -1);
  }
  if (led[i][1]) {
    digitalWrite(5, HIGH);
  } else {
    digitalWrite(5, -1);
  }
  */
  ledGuess(led[i][0], led[i][1], led[i][2]);
}

void ledGuess(int x, int y, int z) {
   if (x == -1) {
     //digitalWrite(3, -1);
     pinMode(3, INPUT);
     //digitalWrite(3, LOW);
   } else if (x==0) {
     pinMode(3, OUTPUT);
     digitalWrite(3, LOW);
   } else {
     pinMode(3, OUTPUT);
     digitalWrite(3, HIGH);
   }
   if (y == -1) {
     //digitalWrite(4, -1);
     pinMode(4, INPUT);
     //digitalWrite(4, LOW);
   } else if (y==0) {
     pinMode(4, OUTPUT);
     digitalWrite(4, LOW);
   } else {
     pinMode(4, OUTPUT);
     digitalWrite(4, HIGH);
   }
   if (z == -1) {
     //digitalWrite(5, -1);
     pinMode(5, INPUT);
     //digitalWrite(5, LOW);
   } else if (z==0) {
     pinMode(5, OUTPUT);
     digitalWrite(5, LOW);
   } else {
     pinMode(5, OUTPUT);
     digitalWrite(5, HIGH);
   }
}
