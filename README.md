# ArDNC
ArDNC is an Arduino based program that is designed to transfer NC files from an SD card to a CNC machine via serial communications.  
The system will utilize a config.cfg file found in the root directory of the SD card to set up communication parameters and to locate where the NC files are stored.  It will have an Adafruit 16 x 2 LCD RGB Shield + 5 key keypad ( https://learn.adafruit.com/rgb-lcd-shield ) to allow the operator to control ArDNC.

## config.cfg
A sample 'config.cfg' file is provided along with detailed comments.

## Prog Design
Files within this directory are provided to explain the program flow, screens displayed, and hardware design.  'Display Pages' lists the various screens to be displayed on the LCD screen.  'Ardnc.dia' is a Dia < http://live.gnome.org/Dia > file which contains the program's general flow.  'ArDNC.fzz' is a Fritzing < http://www.fritzing.org > file containing basic hardware and electrical design.
