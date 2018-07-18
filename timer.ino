void rfidtimer() {
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
}  

ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
//  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  str="";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if(mfrc522.uid.uidByte[i] < 0x10) str+="0";
    
    str+=String(mfrc522.uid.uidByte[i], HEX);
  } 
//  Serial.println(str);
  
  u8g.firstPage();  
  do {
    draw(8);
  } while( u8g.nextPage() );
  dosend=1;
      
}
