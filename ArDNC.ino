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

void setup() {
  Serial.begin(9600);
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
    }
    myFile.close();
  }else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("config.cfg not");
    lcd.setCursor(0,1);
    lcd.print("found");
    lcd.setBacklight(RED);
    while(1);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}

