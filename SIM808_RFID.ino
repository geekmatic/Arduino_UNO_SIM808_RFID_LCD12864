
#include <SPI.h>
#include <MFRC522.h>
#include "U8glib.h"

#define buzz 6
//RFID
#define RST_PIN    5   // 
#define SS_PIN    3    //
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

U8GLIB_ST7920_128X64_1X u8g(9, 8, 7); //SCK = en = 18, MOSI = rw = 16, CS = di = 17
const char *menu_strings[9] = { "  O\266\270\264AH\270E", "  \277POXO\264\270TE", " 3A\277PEЙEHO", "АБВГДЕЖЗИЙКЛМНОП", "eegbbrtn", "rtntymyilui,b", "weteryrtyrt", "w34ryrtyrt", "  \277POBEPKA" };
//u8g.drawStr( 0, 10, "\260\261\262\263\264\265\266\267\270\271\272\273\274"); 

#define OK 1
#define NOTOK 2
#define TIMEOUT 3
#define SIM808board Serial
char end_c[2];
int dosend=0, dosendgps=0;
int a2;

String str, str1;
String signal_level_str;
float longitude, latitude;
unsigned long long prev_ms_gprs, prev_ms_gps;

void setup() {
//  Serial.begin(9600);
//  pinMode(7, INPUT_PULLUP);
  SIM808board.begin(19200);
  
//  Serial.println("S E T U P");
  delay(1000);
  SIM808board.println("AT");
  SIM808board.println("AT");
  prev_ms_gprs = millis();
  prev_ms_gps = millis();
  A6command("AT+CGPSPWR=1", "OK", "ERROR", 5000, 1);

  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();
  
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  
  rfidtimer();
  u8g.firstPage();  
  do {
    draw(0);
  } while( u8g.nextPage() );

    A6command("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK", "ERROR", 2000, 1);
    A6command("AT+SAPBR=3,1,\"APN\",\"internet\"", "OK", "ERROR", 2000, 1);///////////////www.ab.kyivstar.net

    A6command("AT+SAPBR=3,1,\"USER\",\"\"", "OK", "ERROR", 2000, 1);
    A6command("AT+SAPBR=3,1,\"PWD\",\"\"", "OK", "ERROR", 2000, 1);
    A6command("AT+SAPBR=1,1", "+SAPBR:", "ERROR", 2000, 1); //start up the connection
    A6command("AT+HTTPINIT", "OK", "ERROR", 5000, 1);
    pinMode(buzz, OUTPUT);
//  attachInterrupt(digitalPinToInterrupt(7), func_start, FALLING);
}

void loop() {
    
    func_gps(); 
    //A6command("AT+SAPBR=4,1", "OK", "ERROR", 2000, 1);
   // if (jeka==1) digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    //A6command("AT+HTTPINIT=?", "OK", "ERROR", 5000, 1);
    delay(50);
    if (dosend==1) {func_gprs();}
    dosend=0;
}

