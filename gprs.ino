void func_gprs() {

    tone(buzz, 1000);
    delay(100);
    noTone(buzz);

    str1 = "1.php/";
    str1 += "a=";
    str1 += str;
    str1 += ";";
    if (dosendgps==1){
      str1 += "L1=";
      str1 += String(longitude, 5);
      str1 += ";L2=";
      str1 += String(latitude, 5);
      str1 += ";";
      dosendgps = 0;
    }

    A6command("AT+HTTPPARA=\"URL\",\"bee.in.ua/" + str1 + "\"", "OK", "ERROR", 10000, 1);
    A6command("AT+HTTPPARA=\"CID\",1", "OK", "ERROR", 10000, 1);
  
    A6command("AT+HTTPACTION=0", "+HTTPACTION:", "ERROR", 20000, 1);
    
    if (signal_level_str.indexOf("+HTTPACTION: 0,200,")==-1){
      A6command("AT+HTTPTERM", "HTTPTERM", "ERROR", 20000, 1);
      A6command("AT+SAPBR=0,1", "+SAPBR:", "ERROR", 2000, 1);

      A6command("AT+SAPBR=1,1", "+SAPBR:", "ERROR", 2000, 1); //start up the connection
      A6command("AT+HTTPINIT", "OK", "ERROR", 5000, 1);
      A6command("AT+HTTPPARA=\"URL\",\"bee.in.ua/" + str1 + "\"", "OK", "ERROR", 10000, 1);
      A6command("AT+HTTPPARA=\"CID\",1", "OK", "ERROR", 10000, 1);
      A6command("AT+HTTPACTION=0", "+HTTPACTION:", "ERROR", 20000, 1);
    }
    signal_level_str = "";
    A6command("AT+HTTPREAD", "HTTPREAD", "ERROR", 20000, 1);
    parse_dani(signal_level_str);
}
  
void parse_dani(String &outstr)
{
  if (outstr.indexOf("a=")>-1){
    outstr = outstr.substring(outstr.indexOf("a=")+2, outstr.length());
  
    a2 = outstr.toInt();
      u8g.firstPage();  
  do {
    draw(a2);
  } while( u8g.nextPage() );
  if (a2==1){
    tone(buzz, 1000);
    delay(500);
    tone(buzz, 2000);
    delay(400);
    noTone(buzz);
  }else{
    tone(buzz, 200);
    delay(600);
    noTone(buzz);
    delay(2000);
  }
    u8g.firstPage();  
  do {
    draw(0);
  } while( u8g.nextPage() );
//    Serial.print("a2=");
//    Serial.println(a2);
/*    u8g.firstPage();  
  do {
    draw(a2);
  } while( u8g.nextPage() );*/
  }
}
  
byte A6waitFor(String response1, String response2, uint16_t timeOut) {
  uint16_t entry = 0;
  uint16_t count = 0;
  String reply = A6read();
  byte retVal = 99;
  do {
    reply = A6read();
    delay(1);
    entry ++;
  } while ((reply.indexOf(response1) + reply.indexOf(response2) == -2) && entry < timeOut );
  if (entry >= timeOut) {
    retVal = TIMEOUT;
  } else {
    if (reply.indexOf(response1) + reply.indexOf(response2) > -2) retVal = OK;
    else retVal = NOTOK;
  }
  signal_level_str = reply;
  return retVal;
}

byte A6command(String command, String response1, String response2, uint16_t timeOut, uint16_t repetitions) {
  byte returnValue = NOTOK;
  byte count = 0;
  
  while (count < repetitions && returnValue != OK) {
    SIM808board.println(command);
    
    if (A6waitFor(response1, response2, timeOut) == OK) {
      returnValue = OK;
    } else {returnValue = NOTOK;}
    count++;
  }
  
  return returnValue;
}

String A6read() {
  char c;
  String reply = "";
  if (SIM808board.available())  {
    reply = SIM808board.readString();
  }

/*  if (reply!=""){
    Serial.print("Reply: ");
    Serial.println(reply);
  }
  */
  //signal_level_str = reply;
  return reply;
}


