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
  }else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("config.cfg not");
    lcd.setCursor(0,1);
    lcd.print("found.");
    lcd.setBacklight(RED);
    while(1);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}

void setparam(char setting[10], char value[10]){
  int a;
  
  switch(setting){
    case "baud":
      a=atoi(setting);
      Serial.begin(a);
      break;
    default:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Unknown setting:");
      lcd.setCursor(0,1);
      lcd.print(setting);
      lcd.setBacklight(YELLOW);
      delay(1000);
  }
}
