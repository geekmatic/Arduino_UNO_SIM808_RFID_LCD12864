void Decode7bit(String &instr, String &outstr)
{
  String part1 = outstr.substring(outstr.indexOf("rahunku")+8, outstr.indexOf(" grn.")); //parsing balansa iz texta operatora
  if (part1.indexOf(".")>-1) {balans = part1.toFloat(); dosendbalans = 1;}
}

void func_balans() {
  if (millis() - prev_ms_balans >= 600000){
    prev_ms_balans = millis();
    signal_level_str = "";
    A6command("AT+CUSD=1,\"*111#\"", "grn", "ERROR", 10000, 1); //USSD proverka balansa
    String inputstr;
    String decodestr=""; 
    
    inputstr = signal_level_str;
    decodestr=inputstr;
    Decode7bit(inputstr, decodestr); 
  }
}      
