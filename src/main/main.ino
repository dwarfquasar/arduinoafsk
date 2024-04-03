//Simple data transmitter

  int dataSend = 9, ptt = 4, led = 13, dR = 0, sgnlPin = 27;
  /*
   * bitLength is the inverse of the baudrate in ms
   * 45.5 bd = 22 ms
   * 100 bd = 10 ms
   */
  uint8_t bitLength = 22;
  uint8_t startPulse = 22;
  uint8_t stopPulse = 33;

  uint8_t markTone = 2125 - 1315;
  uint8_t spaceTone = 2295 - 1315;
  
  //spk+ is white
  //mic+ red
  
  int EMA0_S_lo = 0, EMA0_S_hi = 0, EMA1_S_lo = 0, EMA1_S_hi = 0, sgnlVal = 0;
  float EMA0_a_lo = 0.3, EMA0_a_hi = 0.6, EMA1_a_lo = 0.3, EMA1_a_hi = 0.6;
  int hPass0 = 0, bPass0 = 0, hPass1 = 0, bPass1 = 0;
  int T1_cmp;

static const uint8_t bcodeLTRS[] =
{
  0x18, 0x13, 0xE, 0x12, 0x10, 0x16, 0xB, 0x5, 0xC,
  0x1A, 0x1E, 0x9, 0x7, 0x6, 0x3, 0xD, 0x1D, 0xA,
  0x14, 0x1, 0x1C, 0xF, 0x19, 0x17, 0x15, 0x11

};
  /*
   * Change between letters and figures
   * numbers are in figures
   * SPACE = 0x4
   * LTRS = 0x1F
   * FIGS = 0x1B
   * 1 = Q, 2 = W, 3 = E, 4 = R, 5 = T, 6 = Y, 7 = U, 8 = I, 9 = O, 0 = P
   */

static const uint8_t bcodeFIGS[] =
{
  0xD, 0x1D, 0x19, 0x10, 0xA, 0x1, 0x15, 0x1C, 0xC, 0x3
  /* 0,1,2,3,4,5,6,7,8,9 */
};


uint8_t ascii_to_baudot(int chr){
  if(chr > 64 && chr < 91){
  	chr += 32;
  	return bcodeLTRS[chr-97];
  	}
  else if(chr > 96 && chr < 123){
  	return bcodeLTRS[chr-97];
  }
  else if(chr > 47 && chr < 58){
    return bcodeFIGS[chr-48];
  }
  else return 0;
}

void TimerTone(bool MARK){
  /*
   * 16Mhz / 2125 Hz = 7529 clock cycles
   * 16Mhz / 2295 Hz = 6971 clock cycles
   * TOP value is ICRn register 
   * ICR1 = 7529 for 2125 Hz
   * OCRnx = 4000 for square wave
   * Shift CS10 bit in to use prescalar of 1 (no scaling)
   * WGM11 and WGM13 for cnt UP to ICR1, Down to 0x0000, PWM mode
   */

  uint8_t clkc = MARK ? 200 : 180;
  
  //TCCR1A = (1 << WGM11);
  TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
  TCCR1B = (1 << CS10) | (1 << WGM13) | (1 << WGM12) | (1 << WGM11);
  ICR1 = clkc;
  OCR1A = clkc;
  
  unsigned long Time = millis();
  do{
    analogWrite(9, 5);
  }
  while(millis() - Time < bitLength);

  // Stop timer1 or stop making noize
  TCCR1B = 0;
  OCR1A = 0;
  ICR1 = 0;
}

void setup() {
  Serial.begin(115200);
  
  pinMode(dataSend, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(ptt, OUTPUT);
  pinMode(sgnlPin, INPUT);
  pinMode(9, OUTPUT);

  digitalWrite(ptt, LOW);
  
  EMA0_S_lo = analogRead(sgnlPin);
  EMA0_S_hi = analogRead(sgnlPin);
  EMA1_S_lo = analogRead(sgnlPin);
  EMA1_S_hi = analogRead(sgnlPin);
  
  Serial.write("Starting Transmitter...");

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
    //digitalWrite(ptt, HIGH);
    //dR = Serial.read();
    char sR = Serial.read();
    
    //CR (0x0d or '\r') and LF/NL (0x0a or '\n')
    dR = ascii_to_baudot(sR);
    
    while(whatbit < 5){    //5 bits for baudot
      //tone(dataSend, spaceTone, startPulse);   //start pulse (space)
      TimerTone(0);
      delay(bitLength);                        //wait for tx
      if(dR & 0x01){
        //tone(dataSend, markTone, bitLength);   //send mark
        TimerTone(1);
        delay(50);
      }
          
      else{
        //tone(dataSend, spaceTone);             //send space
        TimerTone(0);
        delay(50);
       }
      whatbit++;
      dR = dR >> 1;
    }
    Serial.write(sR);
    //tone(dataSend, markTone, stopPulse);       //end pulse (mark)
    
  }
  else{
    //done sending data so no tone and ptt off
    //noTone(dataSend);
    digitalWrite(ptt, LOW);
  }
}
