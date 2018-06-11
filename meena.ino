/*
    make sure to uplaod with the sparkfun pro-micro (5V, 16MHz processor).

    See README.md for usage details
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
constexpr int dialPinOne = 0; // interrupt #2
constexpr int dialPinTwo = 7; // interrupt #4
constexpr int buttonPin = 8;  // pushbutton input
constexpr int sigPin = 5;     // outputs to IRF520 MOS FET

// holds individual digits
int digits[3] = {-1};

// modes
constexpr int INIT = 0;         // during startscreen
constexpr int EDIT = 1;         // during timer setup
constexpr int RUN = 2;          // during countdown
constexpr int FINISHED = 3;     // once finished
int mode;                       // holds the mode

// holds dial turn quantity
int tick = 0;

// activates the solenoid for 1/10th of a second
// causing striker to strike
void strike(){
  digitalWrite(sigPin, HIGH);
  delay(100);
  digitalWrite(sigPin, LOW);
}

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


// takes an integer and store each of the digits
// in the digits[] array:
// 105 => d[0]=1, d[1]=0,  d[2]=5
// 82  => d[0]=8, d[1]=2,  d[2]=-1
// 7   =? d[0]=7, d[1]=-1, d[2]=-1 
void extractNewDigits(int val){
  val = constrain(val, 1, 999);
  int factor = 1;
  int temp = val;
  int newDigit;
  int pos;
  
  while(temp){
    temp /= 10;
    factor *= 10;
  }

  temp = val;
  while(factor>1){
    factor /= 10;
    newDigit = temp / factor;
    digits[pos] = newDigit;
    temp %= factor;
    pos++;
  }

  if(val<100){ digits[2] = -1; }
  if(val<10){ digits[1] = -1; }
}

// updates OLED
void displayVal(int val){  
 val = constrain(val, 1, 999);
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

void editMode(){
  for(int i = 120; i > 0; i--){
    displayVal(i);
    delay(500);
  }
}

// countdown.
// enter editMode once dial turned or knob pushed
void runMode(){

}

// show a bunch of pretty stars :)
// enter editMode once dial turned or knob pushed
void finishedMode(){

}

void setup() {
  mode = INIT;
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  attachInterrupt(digitalPinToInterrupt(dialPinOne), encoderEvent, CHANGE);
  
  pinMode(sigPin, OUTPUT);
  pinMode(1, OUTPUT); // tmp
  showWelcomeScreen();
  mode = EDIT;
}

void loop(){
  switch mode {
    case EDIT: 
      editMode(); break;
    case RUN:
      runMode(); break;
    case FINISHED:
      finishedMode(); break;
    default:
      editMode(); break;
  }
}

// long timerValueChangedAt;
// int lastTimerValue;
// int timeRemaining;
// bool doneTillReset = false;

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
