/*
    Make sure before uploading code via Arduino IDE:
    
    0. Board is Adafruit Trinket 8MHz
    1. Programmer is USBtinyISP

    https://learn.adafruit.com/introducing-trinket/setting-up-with-arduino-ide
*/


// Include for OLED
#include <SSD1306_minimal.h>
#include "spiral.h"
#include "game_over.h"
#include "square.h"

// instantiate our OLED display
SSD1306_Mini display;

// pins
constexpr int potPin = 3;
constexpr int sigPin = 4;

void updateDisplay(){
  display.clear();
  // display.startScreen();
  // display.printString("Hello, World!");
//  display.drawImage(squarez, 0,0,5,5);
  // display.drawImage(GameOver,0,0,128,8);
  // oled.drawImage(BirdSprite,0,currentSpritePosition,8,1);
}

void setup() {
  // initialize with the I2C addr 0x3C
  display.init(0x3C);

  pinMode(sigPin, OUTPUT);
  updateDisplay();
}

void loop() {
  digitalWrite(sigPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(sigPin, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
