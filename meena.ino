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
#include "fonts/luna_fonts.h"

// instantiate our OLED display
SSD1306_Mini display;

// arduino pins
constexpr int potPin = 4;
constexpr int sigPin = 1;

// keep track of changes in potentiometer (knob)
int oldAnalogValue = 0;

// holds individual digits
int digits[3] = {-1};

// activates the solenoid for 1/10th of a second
void strikeBowl(){
  digitalWrite(sigPin, HIGH);
  delay(100);
  digitalWrite(sigPin, LOW);
}

// fetch potentiometer value and scale to between 1-120
int fetchAnalog(){
  int potVal = analogRead(potPin); // returns value in range [0,1023]
  return 1 + int(potVal * .1163245357); // normalize to value in range [1,120]
}

// beautiful example of bad programming.
// TODO: just make fonts a 2D array.
void displayDigit(int num, int pos){
  switch(num){
    case 0:
      display.drawImage(number_0, pos, 0, 40,5); break;
    case 1:
      display.drawImage(number_1, pos, 0, 40,5); break;
    case 2:
      display.drawImage(number_2, pos, 0, 40,5); break;
    case 3:
      display.drawImage(number_3, pos, 0, 40,5); break;
    case 4:
      display.drawImage(number_4, pos, 0, 40,5); break;
    case 5:
      display.drawImage(number_5, pos, 0, 40,5); break;
    case 6:
      display.drawImage(number_6, pos, 0, 40,5); break;
    case 7:
      display.drawImage(number_7, pos, 0, 40,5); break;
    case 8:
      display.drawImage(number_8, pos, 0, 40,5); break;
    case 9:
      display.drawImage(number_9, pos, 0, 40,5); break;
    case -1:
      display.drawImage(number__, pos, 0, 40,5); break;
  }
}

void extractNewDigits(int val){
  int factor = 1;
  int temp=val;
  int newDigit;
  int pos;

  if(val < 100){
    digits[2] = -1;
  }
  if(val< 10){
    digits[1] = -1;
  }

  while(temp){
    temp /= 10;
    factor *= 10;
  }

  while(factor>1){
    factor /= 10;
    newDigit = val / factor;
    digits[pos] = newDigit;
    val = val % factor;
    pos++;
  }
}

void displayVal(int val){  
  val = constrain(val, 1, 120);
  extractNewDigits(val);
  displayDigit(digits[0],0);
  displayDigit(digits[1],40);
  displayDigit(digits[2],80);
}

void showWelcomeScreen(){
  display.clear();
  display.drawImage(meena,5,3,58,3);
  display.drawImage(spiral,75,1,49,6);
  delay(3000);
}

void setup() {
  display.init(0x3C);
  pinMode(sigPin, OUTPUT);
  pinMode(1, OUTPUT); // tmp
//  showWelcomeScreen();

 display.clear();

}

int fakePot = 120;
void loop() {
  int newAnalogValue = fetchAnalog();
  displayVal(fakePot);
  delay(100);
  fakePot--;
  if(fakePot == 1){ fakePot = 120; }
}
