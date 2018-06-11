/*
    make sure to uplaod with the sparkfun pro-micro (5V, 16MHz processor)  
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "images/startScreen.h"
#include "images/misc.h"
#include "fonts/luna_fonts.h"

// instantiate the display
Adafruit_SSD1306 display(4);


// arduino pins
constexpr int dialPinOne = 0;
constexpr int dialPinTwo = 7;
constexpr int buttonPin = 4;
constexpr int sigPin = 5;

// holds individual digits
int digits[3] = {-1};

// holds dial turn quantity
int tick = 0;

// activates the solenoid for 1/10th of a second
// striking the instrument
void strike(){
  digitalWrite(sigPin, HIGH);
  delay(100);
  digitalWrite(sigPin, LOW);
}

// fetch potentiometer value and scale to between 1-120
// int fetchAnalog(){
//   int potVal = analogRead(potPin); // returns value in range [0,1023]
//   averager.push(potVal);
//   return 1 + int(averager.getAverage() * .01173);
// }

// beautiful example of bad programming.
// TODO: just make fonts a 2D array.
void displayDigit(int num, int pos){
  switch(num){
    case 0:
      display.drawBitmap(pos, 0,  number_0, 40, 40, 1); break;
    case 1:
      display.drawBitmap(pos, 0,  number_1, 40, 40, 1); break;
    case 2:
      display.drawBitmap(pos, 0,  number_2, 40, 40, 1); break;
    case 3:
      display.drawBitmap(pos, 0,  number_3, 40, 40, 1); break;
    case 4:
      display.drawBitmap(pos, 0,  number_4, 40, 40, 1); break;
    case 5:
      display.drawBitmap(pos, 0,  number_5, 40, 40, 1); break;
    case 6:
      display.drawBitmap(pos, 0,  number_6, 40, 40, 1); break;
    case 7:
      display.drawBitmap(pos, 0,  number_7, 40, 40, 1); break;
    case 8:
      display.drawBitmap(pos, 0,  number_8, 40, 40, 1); break;
    case 9:
      display.drawBitmap(pos, 0,  number_9, 40, 40, 1); break;
    case -1:
      display.drawBitmap(pos, 0,  number__, 40, 40, 1); break;
  }
}

/* 
   takes an integer and stores each of the digits
   in the digits[] array.
*/
void extractNewDigits(int val){
  int factor = 1;
  int temp=val;
  int newDigit;
  int pos;
  
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

// updates OLED
void displayVal(int val){  
 val = constrain(val, 1, 120);
 extractNewDigits(val);
 display.clearDisplay();
 displayDigit(digits[0],0);
 if(val>9){ displayDigit(digits[1],40); }
 if(val>99){ displayDigit(digits[2],80); }
 display.display();
}

void showWelcomeScreen(){
  display.clearDisplay();
  display.drawBitmap(0, 0,  startScreen, 128, 64, 1);
  display.display();
  delay(3000);
}

void encoderEvent() {
  if(digitalRead(dialPinOne) == digitalRead(dialPinTwo)){
    tick++;
  } else {
    tick--;
  }
}

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  attachInterrupt(digitalPinToInterrupt(dialPinOne), encoderEvent, CHANGE);
  
  pinMode(sigPin, OUTPUT);
  pinMode(1, OUTPUT); // tmp
  showWelcomeScreen();
}

void loop(){
  for(int i = 120; i > 0; i--){
    displayVal(i);
    delay(500);
  }
}

long timerValueChangedAt;
int lastTimerValue;
int timeRemaining;
bool doneTillReset = false;

// void loopy() {
//   int newTimerValue = fetchAnalog()*5;
//   if(newTimerValue != lastTimerValue){
//     timerValueChangedAt = millis();
//     lastTimerValue = newTimerValue;
//   }

//   long millisSinceChange = millis() - timerValueChangedAt;

//   if(millisSinceChange > 3000 && !doneTillReset){
//     timeRemaining = (newTimerValue * 60 * 1000) - millisSinceChange;
//     if(timeRemaining == 0){
//       strike();
//       doneTillReset = true;
//     }
//     displayVal(timeRemaining);
//     display.drawImage(ohm,110,30,5,1);
//   }else{
//     doneTillReset = false;
//     displayVal(newTimerValue);
//     display.drawImage(empty_ohm,110,30,5,1);
//   }

//   delay(10);
// }
