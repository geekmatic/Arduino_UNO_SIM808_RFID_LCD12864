void func_gps() {
  
  if (millis() - prev_ms_gps >= 300000){ //chastota otpravki danih msec
    prev_ms_gps = millis();
    if (dosend==1) return;
    A6command("AT+CGPSPWR=1", "OK", "ERROR", 5000, 1);
    if (dosend==1) return;
    A6command("AT+CGPSSTATUS?", "OK", "ERROR", 5000, 1);
    if (dosend==1) return;

    if ((signal_level_str.indexOf("Location 3D Fix")>-1)||(signal_level_str.indexOf("Location 2D Fix")>-1)){
      A6command("AT+CGPSINF=2", "+CGPSINF:", "ERROR", 5000, 1);
      if (dosend==1) return;
      signal_level_str = signal_level_str.substring(signal_level_str.indexOf(",")+1, signal_level_str.length());
      signal_level_str = signal_level_str.substring(signal_level_str.indexOf(",")+1, signal_level_str.length());
      String part;
      part = signal_level_str.substring(0, signal_level_str.indexOf(","));
      latitude = part.toFloat();
//      Serial.print("latitude=");
//      Serial.print(latitude,4);
      signal_level_str = signal_level_str.substring(signal_level_str.indexOf(",")+1, signal_level_str.length());
      signal_level_str = signal_level_str.substring(signal_level_str.indexOf(",")+1, signal_level_str.length());
      part = signal_level_str.substring(0, signal_level_str.indexOf(","));
      longitude = part.toFloat();
      dosendgps = 1;
/*      Serial.print(" longitude=");
      Serial.println(longitude,4);*/
    }
  }
}
