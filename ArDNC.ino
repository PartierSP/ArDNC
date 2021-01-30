/*
 ArDNC2
 
 Created Mar 2015
 by Mike van Leeuwen, Dipl. T.
 for O'Hara Machine & Tool Inc.
 
 SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 10
 
 The RS232 attaches as follows:
 ** TX - pin 1 (out)
 ** RC - pin 0 (in)
 ** CTS - pin 2 (in)
 ** RTS - pin 3 (out)
 
*/

#include <SD.h>

// include the display library code:
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color
#define OFF 0x0
#define RED 0x1
#define GREEN 0x2
#define YELLOW 0x3
#define BLUE 0x4
#define VIOLET 0x5
#define TEAL 0x6
#define WHITE 0x7

// XON and XOFF #defines
#define XON 0x11
#define XOFF 0x13
#define XON2 0x12
#define XOFF2 0x14

// CTS and RTS pin
#define CTS 2
#define RTS 3

//Configuration settings
char cfgname[16] = "NO-NAME";
char cfgfilename[16] = "";
char cfgdir[32] = "/";
int cfgbaud = 2400;
int cfgdata = 8;
int cfgpar = 0;
int cfgstop = 1;
int cfgsoft = 1;
int cfghard = 0;
int cfgwait = 0;
int cfgcomdebug = 0;
int cfghead = 0;
int cfghrepeat = 0;
int cfgsendperc = 0;

void setup() {
  
  //Initialize CTS and RTS pins
  pinMode(CTS, INPUT);
  pinMode(RTS, OUTPUT);
  digitalWrite(RTS, LOW);

  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  
  lcd.clear();
  lcd.print("Initializing... ");
  lcd.setBacklight(WHITE);
  delay(2000); // Gives time to read the screen.
  
  //Initializing SD Card
  pinMode(10, OUTPUT);
  
  if (!SD.begin(10)) {
    lcd.clear();
    lcd.print("SD Card init ");
    lcd.setCursor(0, 1);
    lcd.print("FAILED");
    lcd.setBacklight(RED);
    return;
  }
  
  readcfg();
    
}

void loop()
{
  choosefile();
}

void choosefile() {

  uint8_t buttons;
  int index=-1;
  int i=0;
  File root;
  
  lcd.clear();
  lcd.print("Select File:");
  lcd.setBacklight(WHITE);
  
  root = SD.open(cfgdir);

  while(true) {
    File entry = root.openNextFile();
    index++;
    if(! entry) {
      if(index==0){
        //Directory is empty
        lcd.clear();
        lcd.print("ERROR. Dir not");
        lcd.setCursor(0, 1);
        lcd.print("found or empty.");
        lcd.setBacklight(RED);
        while(true){
        }
      }
      index=-1;
      entry.close();
      root.close();
      return;
    }
    lcd.setCursor(0, 1);
    lcd.print(entry.name());
    lcd.print("                "); //cleans rest of the line

    //////////////////////////////////////////////////////////
    //               Available Memory Checker               //
    //////////////////////////////////////////////////////////

//    lcd.setCursor(0,0);
//    lcd.print(freeRam());

    delay(500);

    while(true){
      buttons = lcd.readButtons();
      if(buttons) {
        if(buttons & BUTTON_DOWN){
          break;
        }
        if(buttons & BUTTON_RIGHT){
          strcpy(cfgfilename, entry.name());
          while (true){
            sendfile();
            
            lcd.clear();
            lcd.print("Select to resend");
            lcd.setCursor(0, 1);
            lcd.print("Back = file menu");
            lcd.setBacklight(WHITE);
            
            while(true){
              buttons = lcd.readButtons();
              if(buttons) {
                if(buttons & BUTTON_RIGHT){
                  break;
                }
                if(buttons & BUTTON_LEFT){
                  entry.close();
                  root.close();
                  return;
                }
              }
            }
          }
          
          break;
        }
        if(buttons & BUTTON_UP){
          if(index>0){
            entry.close();
            root.close();
            root = SD.open(cfgdir);
            for(i=1; i<index; i++){
              File entry = root.openNextFile();
              entry.close();
            }
            index=i-2;
            break;
          }
        }
      }
    }
    entry.close();
  }
}

void readcfg()
{

  char character;
  char desc[16] = "";
  char valu[16] = "";
  int editvalue = 0;
  int i = 0;

  File cfg = SD.open("config.cfg");
  
  if(cfg) {
    while (cfg.available()) {
      character = cfg.read();
      if(character == '#') {
        //Comment - ignore this line
        while(character != '\n' && cfg.available()){
          character = cfg.read();
        }
        
        i++;
        lcd.setCursor(0,1);
        lcd.print(i);
  
      //if character is a letter, number, slash, period, or underscore
      }else if(isalnum(character) || character == 47 || character == 46 || character == 31) {
        if(editvalue == 1){
          //add character to the value
          append(valu, character);
        }else{
          //add character to the description
          append(desc, character);
        }
      }else if(character == '='){
        //description done now mod value
        editvalue = 1;
      }else if(character == '\n'){
        i++;
        lcd.setCursor(0,1);
        lcd.print(i);
        //end of line, reset editvalue and set proper global
        editvalue = 0;
        if (strcmp("NAME",desc) == 0){
          strcpy(cfgname, valu);
        } else if(strcmp("SPEED",desc) == 0){
          cfgbaud=atoi(valu);
        } else if(strcmp("DATA",desc) == 0){
          cfgdata=atoi(valu);
        } else if(strcmp("PAR", desc) == 0){
          cfgpar=atoi(valu);
        } else if(strcmp("STOP",desc) == 0){
          cfgstop=atoi(valu);
        } else if(strcmp("SOFT",desc) == 0){
          cfgsoft=atoi(valu);
        } else if(strcmp("HARD",desc) == 0){
          cfghard=atoi(valu);
        } else if(strcmp("WAIT",desc) == 0){
          cfgwait=atoi(valu);
        } else if(strcmp("FILE",desc) == 0){
          strcpy(cfgfilename, valu);
        } else if(strcmp("DIR",desc) == 0){
          strcpy(cfgdir, valu);
        } else if(strcmp("COMDEBUG",desc) == 0){
          cfgcomdebug=atoi(valu);
        } else if(strcmp("HEAD",desc) == 0){
          cfghead=atoi(valu);
        } else if(strcmp("HEAD_REPEAT",desc) == 0){
          cfghrepeat=atoi(valu);
        } else if(strcmp("SEND_PERCENT",desc) == 0){
          cfgsendperc=atoi(valu);
        } //ADD MORE SETTINGS HERE.
        strcpy(desc, "");
        strcpy(valu, "");
      } else {
        //Ignore anything else
      }
    }
    cfg.close();

      lcd.clear();
      lcd.print("Init done.");
      lcd.setCursor(0,1);
      lcd.print("Using ");
      lcd.print(cfgname);
      lcd.setBacklight(WHITE);
      delay(2000);
    
  } else { //Read error occured (couldn't find file).
      lcd.clear();
      lcd.print("ERROR");
      lcd.setCursor(0,1);
      lcd.print("Config not found");
      lcd.setBacklight(RED);
      
      // Error stop execution.
      while(true) {
      }
   }
}

void sendfile()
{
  uint8_t button;
  unsigned int fsize=0;
  unsigned int fpos=-1;
  unsigned int i=1000;
  unsigned int psetting=0;
  byte sread=0;
  
  char fullname[47]="";

  lcd.clear();
  lcd.print("Sending...");
  lcd.setBacklight(GREEN);
      
  
  //Figure out port settings
  switch (cfgpar) {
    case 0:
      psetting = 0;
      break;
    case 1:
      psetting = 48;
      break;
    case 2:
      psetting = 32;
      break;
  }
  psetting=psetting + (cfgdata-5)*2;
  psetting=psetting + (cfgstop-1)*8;
 
  // Display Communication Settings
  if(cfgcomdebug == 1){
    lcd.setCursor(13, 0);
    lcd.print(psetting);
    lcd.setBacklight(WHITE);
    delay(3000);
    lcd.setCursor(13, 0);
    lcd.print("   ");
    lcd.setBacklight(GREEN);
 
  }

  Serial.begin(cfgbaud, psetting);
  while (!Serial) {
  }
  
  strcpy(fullname, cfgdir);
  strcat(fullname, cfgfilename);
  
  File dataFile=SD.open(fullname);
  if (dataFile) {

    do{
      button=lcd.readButtons();
    } while(button & BUTTON_RIGHT); //Wait until Right button has been released.
    
    //If hardware handshake send Request To Send
    if(cfghard == 1){
      digitalWrite(RTS, HIGH);
    }

    //Send header string if required
    lcd.setBacklight(GREEN);
    for(int headcount=0; headcount<cfghrepeat; headcount++){
      Serial.write(cfghead);
    }
    
    //Wait for XON if applicable
    if(cfgsoft == 1 && cfgwait == 1){
      lcd.setBacklight(YELLOW);
      while(cfgsoft==1){
        sread = Serial.read();
        button=lcd.readButtons();
        if (sread == XON || sread == XON2 || button & BUTTON_RIGHT){
          break;
        }
      }
      lcd.setBacklight(GREEN);
      do{
        button=lcd.readButtons();
      } while(button & BUTTON_RIGHT); //Wait until Right button has been released.
    }
    
    if(cfgsendperc == 1){
      Serial.write(31);  //Send Percent (%) symbol
      Serial.write(13);  //Send Newline (\n)
    }
  
    fsize = dataFile.size()/1000;
    while (dataFile.available()) {
      if(cfgsoft == 1){
        sread = Serial.read();
        if(sread == XOFF || sread == XOFF2){
          lcd.setBacklight(YELLOW);
          while(true){
            sread = Serial.read();
            if(sread == XON || sread == XON2){
              break;
            }
          }
          lcd.setBacklight(GREEN);
        }
      }

      Serial.write(dataFile.read());
      i++;
      if(i>1023){
        fpos++;
        lcd.setCursor(0,1);
        lcd.print(fpos);
        lcd.print("k of ");
        lcd.print(fsize);
        lcd.print("k");
        i=0;
      }
    }
    dataFile.close();
    
    //Shut off Request To Send
    digitalWrite(RTS, LOW);

  } else {
    
    fsize = dataFile.size();
    
    lcd.clear();
    lcd.print("ERROR");
    lcd.setCursor(0, 1);
    lcd.print("File Open Error.");
    lcd.setBacklight(RED);
    while(true){
    }
  }
}

void append(char* s, char c)
{
  int len = strlen(s);
  s[len]=c;
  s[len+1]='\0';
}

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
