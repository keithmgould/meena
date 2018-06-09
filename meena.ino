/*
    Make sure before uploading code via Arduino IDE:
    
    0. Board is Adafruit Trinket 8MHz
    1. Programmer is USBtinyISP

    Details Here:
    https://learn.adafruit.com/introducing-trinket/setting-up-with-arduino-ide

    Here is the minimal 1306 lib that fits on a Trinket's memory:
    https://github.com/kirknorthrop/SSD1306_minimal
*/


// Include for OLED
#include <SSD1306_minimal.h>
#include "images/startScreen.h"
#include "fonts/luna_fonts2.h"

// instantiate our OLED display
SSD1306_Mini display;

// pins
constexpr int potPin = 3;
constexpr int sigPin = 4;

void updateDisplay(){
  display.clear();
  display.drawImage(number_5,0,0,40,5);
  display.drawImage(number_1,44,0,40,5);
  display.drawImage(number_8,88,0,40,5);
}

void strikeBowl(){
  digitalWrite(sigPin, HIGH);
  delay(100);
  digitalWrite(sigPin, LOW);
}

void setup() {
  // initialize with the I2C addr 0x3C
  display.init(0x3C);
  display.drawImage(number_0,44,0,40,5);
  pinMode(sigPin, OUTPUT);
  display.clear();
  display.drawImage(startScreen,0,0,128,8);
  delay(3000);
  
  updateDisplay();
}

void loop() {
  display.drawImage(number_0,44,0,40,5);
  delay(1000);
  display.drawImage(number_1,44,0,40,5);
  delay(1000);
  display.drawImage(number_2,44,0,40,5);
  delay(1000);
  display.drawImage(number_3,44,0,40,5);
  delay(1000);
  display.drawImage(number_4,44,0,40,5);
  delay(1000);
  display.drawImage(number_5,44,0,40,5);
  delay(1000);
  display.drawImage(number_6,44,0,40,5);
  delay(1000);
  display.drawImage(number_7,44,0,40,5);
  delay(1000);
  display.drawImage(number_8,44,0,40,5);
  delay(1000);
  display.drawImage(number_9,44,0,40,5);
  delay(1000);
}
