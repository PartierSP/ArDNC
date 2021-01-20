#include <Wire.h>
#include <SD.h>
#include <Adafruit_RGBLCDShield.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
File myFile;

// Define LCD screen color constants
#define OFF 0x0
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7 

#define BUFFSIZE 10

int baud;      //int value of baud rate
int databit;   //16=5 bits, 32=6 bits, 64=7 bits, 128=8 bits
int parity;    //0=even, 1=odd, 2=none
int stopbit;   //4=1 bit, 8=2 bits

void setup() {
  char a;
  char chsetting[BUFFSIZE];
  char chvalue[BUFFSIZE];
  int i;
  
  //Initialize LCD;
  lcd.begin(60,2);
  lcd.setBacklight(BLUE);
  lcd.print("Initializing...");
  
  if (!SD.begin(4)){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SD init failed");
    lcd.setBacklight(RED);
   
    while(1);
  }
  myFile=SD.open("config.cfg");
  if(myFile){
    while (myFile.available()) {
      //Read Config settings
      a=myFile.read();
      switch(a){
        case 32:
          //space character
          //ignore
          break;
        case 9:
          //tab character
          //ignore
          break;
        case 10:
          //New line
          setparam(chsetting,chvalue);
          char chsetting[BUFFSIZE];
          char chvalue[BUFFSIZE];
          i=0;
          break;
        case 35:
          //Hash character
          //Comment, ignore until new line
          i=3;
          break;
        case 61:
          //Equals character
          i=1;
          break;
        default:
          if(i==0){
            //strcat(chsetting,a);
            if(strlen(chsetting)+1<BUFFSIZE){
              chsetting[strlen(chsetting)]=a;
            }
          } else {
            if(i==2){
//              strcat(chvalue,a);
            }
          }
      }
    }
    myFile.close();
    //initialize serial port
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Using COM set:");
    lcd.setCursor(0,1);
    lcd.setBacklight(GREEN);
    switch(databit+parity+stopbit){
      case 22:
        Serial.begin(baud,SERIAL_5N1);
        lcd.print("5N1 - ");
        lcd.print(baud);
        break;
      case 38:
        Serial.begin(baud,SERIAL_6N1);
        lcd.print("6N1 - ");
        lcd.print(baud);
        break;
      case 70:
        Serial.begin(baud,SERIAL_7N1);
        lcd.print("7N1 - ");
        lcd.print(baud);
        break;
      case 20:
        Serial.begin(baud,SERIAL_5E1);
        lcd.print("5E1 - ");
        lcd.print(baud);
        break;
      case 36:
        Serial.begin(baud,SERIAL_6E1);
        lcd.print("6E1 - ");
        lcd.print(baud);
        break;
      case 68:
        Serial.begin(baud,SERIAL_7E1);
        lcd.print("7E1 - ");
        lcd.print(baud);
        break;
      case 132:
        Serial.begin(baud,SERIAL_8E1);
        lcd.print("8E1 - ");
        lcd.print(baud);
        break;
      case 21:
        Serial.begin(baud,SERIAL_5O1);
        lcd.print("5O1 - ");
        lcd.print(baud);
        break;
      case 37:
        Serial.begin(baud,SERIAL_6O1);
        lcd.print("6O1 - ");
        lcd.print(baud);
        break;
      case 69:
        Serial.begin(baud,SERIAL_7O1);
        lcd.print("7O1 - ");
        lcd.print(baud);
        break;
      case 133:
        Serial.begin(baud,SERIAL_8O1);
        lcd.print("8O1 - ");
        lcd.print(baud);
        break;
      case 26:
        Serial.begin(baud,SERIAL_5N2);
        lcd.print("5N2 - ");
        lcd.print(baud);
        break;
      case 42:
        Serial.begin(baud,SERIAL_6N2);
        lcd.print("6N2 - ");
        lcd.print(baud);
        break;
      case 74:
        Serial.begin(baud,SERIAL_7N2);
        lcd.print("7N2 - ");
        lcd.print(baud);
        break;
      case 138:
        Serial.begin(baud,SERIAL_8N2);
        lcd.print("8N2 - ");
        lcd.print(baud);
        break;
      case 24:
        Serial.begin(baud,SERIAL_5E2);
        lcd.print("5E2 - ");
        lcd.print(baud);
        break;
      case 40:
        Serial.begin(baud,SERIAL_6E2);
        lcd.print("6E2 - ");
        lcd.print(baud);
        break;
      case 72:
        Serial.begin(baud,SERIAL_7E2);
        lcd.print("7E2 - ");
        lcd.print(baud);
        break;
      case 136:
        Serial.begin(baud,SERIAL_8E2);
        lcd.print("8E2 - ");
        lcd.print(baud);
        break;
      case 25:
        Serial.begin(baud,SERIAL_5O2);
        lcd.print("5O2 - ");
        lcd.print(baud);
        break;
      case 41:
        Serial.begin(baud,SERIAL_6O2);
        lcd.print("6O2 - ");
        lcd.print(baud);
        break;
      case 73:
        Serial.begin(baud,SERIAL_7O2);
        lcd.print("7O2 - ");
        lcd.print(baud);
        break;
      case 137:
        Serial.begin(baud,SERIAL_8O2);
        lcd.print("8O2 - ");
        lcd.print(baud);
        break;
      default:
        //calculates to 134
        Serial.begin(baud,SERIAL_8N1);
        lcd.print("8N1 - ");
        lcd.print(baud);
        break;
    }
    delay(1000);
  }else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("config.cfg not");
    lcd.setCursor(0,1);
    lcd.print("found.");
    lcd.setBacklight(RED);
    while(1); //We die here.
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}

void setparam(char* setting, char* value){
  int a;
  do{
    if(setting=="baud"){
      baud=atoi(value);
      break;
    }
    if(setting=="parity"){
      if(value=="odd"){
        parity=1;
      }else{
        if(value=="even"){
          parity=0;
        }else{
          //Assume default 'none'
          parity=2;
        }
      }
      break;
    }
    if(setting=="stopbits"){
      if(value=="2"){
        stopbit=8;
      }else{
        //Assume default 1 stopbit
        stopbit=4;
      }
      break;
    }
    if(setting=="databits"){
      if(value=="5"){
        databit=16;
        break;
      }
      if(value=="6"){
        databit=32;
        break;
      }
      if(value=="7"){
        databit=64;
        break;
      }
      //Assuming databit setting of 8.
      databit=128;
      break;
    }
    //Unknown setting found in config file.
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Unknown setting:");
    lcd.setCursor(0,1);
    lcd.print(setting);
    lcd.setBacklight(YELLOW);
    delay(1000);
  }while(1==2);  //only run loop once
}
