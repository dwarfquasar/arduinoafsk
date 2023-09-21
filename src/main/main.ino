//Simple data transmitter

  int dataSend = 9, ptt = 4, led = 13, dR = 0, sgnlPin = 27;
  int bitLength = 1;
  //spk+ is white
  //mic+ red
  int EMA0_S_lo = 0, EMA0_S_hi = 0, EMA1_S_lo = 0, EMA1_S_hi = 0, sgnlVal = 0;
  float EMA0_a_lo = 0.3, EMA0_a_hi = 0.6, EMA1_a_lo = 0.3, EMA1_a_hi = 0.6;
  int hPass0 = 0, bPass0 = 0, hPass1 = 0, bPass1 = 0;
  int T1_cmp;

 
void blink(int pin, int num){
  for(int i = num; i == 0; i--){
    digitalWrite(pin, HIGH);
    delay(250);
    digitalWrite(pin, LOW);
    delay(250);
  }
}

ISR(TIMER1_COMPA_vect){
  TCNT1 = T1_cmp;
}


00011
11001
01110
01001
00001
01101
11010
10100
00110
01011
01111
10010
11100
01100
11000
10110
10111
01010
00101
10000
00111
11110
10011
11101
10101
10001

void setup() {
  Serial.begin(115200);
  noInterrupts();
  
  pinMode(dataSend, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(ptt, OUTPUT);
  pinMode(sgnlPin, INPUT);

  digitalWrite(ptt, LOW);
  
  EMA0_S_lo = analogRead(sgnlPin);
  EMA0_S_hi = analogRead(sgnlPin);
  EMA1_S_lo = analogRead(sgnlPin);
  EMA1_S_hi = analogRead(sgnlPin);
  
  Serial.write("Starting Transmitter...");
  blink(led, 4);
  
  interrupts();
}
 
void loop() {
  
/*
  sgnlVal = analogRead(sgnlPin);

  EMA0_S_lo = (EMA0_a_lo*sgnlVal) + ((1-EMA0_a_lo)*EMA0_S_lo);
  EMA0_S_hi = (EMA0_a_hi*sgnlVal) + ((1-EMA0_a_hi)*EMA0_S_hi);

  hPass0 = sgnlVal - EMA0_S_lo;
  bPass0 = EMA0_S_hi - EMA0_S_lo;

  EMA1_S_lo = (EMA1_a_lo*sgnlVal) + ((1-EMA1_a_lo)*EMA1_S_lo);
  EMA1_S_hi = (EMA1_a_hi*sgnlVal) + ((1-EMA1_a_hi)*EMA1_S_hi);

  hPass1 = sgnlVal - EMA1_S_lo;
  bPass1 = EMA1_S_hi - EMA1_S_lo;
*/
  if(Serial.available() > 0){
    int whatbit = 0;
    digitalWrite(ptt, HIGH);
    dR = Serial.read();

    while(whatbit < 8){
      if(dR & 0x01){
        noTone(dataSend);
        tone(dataSend, 2200, bitLength);
        delay(100);
        Serial.write('1');
      }
      else{
        tone(dataSend, 1200);
        delay(100);
        Serial.write('0');
      }
      whatbit++;
      dR = dR >> 1;
    }
    Serial.println();
  }
  else{
    noTone(dataSend);
    digitalWrite(ptt, LOW);
  }

  
  //serial.read - reads 1st byte of incoming data
  //read, left shift, add, repeat, do 3 times to make long
}
