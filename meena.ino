/*
    make sure to uplaod with the sparkfun pro-micro (5V, 16MHz processor).

    See README.md for usage details
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Encoder.h>
#include "images/startScreen.h"
#include "images/misc.h"
#include "fonts/luna_fonts.h"


// instantiate the display
Adafruit_SSD1306 display(4);

// arduino pins
constexpr int dialPinOne = 7; // interrupt #2
constexpr int dialPinTwo = 0; // interrupt #4
constexpr int buttonPin  = 1; // interrupt #3
constexpr int sigPin = 5;     // outputs to IRF520 MOS FET

// instantiate the encoder object
Encoder myEnc(dialPinOne, dialPinTwo);

// holds individual digits
int digits[3] = {-1};

// modes
constexpr int INIT = 0;         // during startscreen
constexpr int EDIT = 1;         // when user choosing time
constexpr int RUN = 2;          // during countdown
constexpr int FINISHED = 3;     // once finished

// holds the mode
int mode;




volatile bool buttonPressed = false;

long tick = 0;                              // determined by rotary encoder
unsigned long timerStartedAt = 0;           // time started at in milliseconds
unsigned int desiredDurationInSeconds = 0;  // holds desired timer time in seconds

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
  }
}


// takes an integer and store each of the digits
// in the digits[] array:
// 105 => d[0]=1, d[1]=0,  d[2]=5
// 82  => d[0]=8, d[1]=2,  d[2]=-1
// 7   =? d[0]=7, d[1]=-1, d[2]=-1 
void extractNewDigits(int val){
  int factor = 1;
  int temp = val;
  int newDigit;
  int pos;
  
  if(val == 0){
    digits[0] = digits[1] = digits[2] = 0;
    return;
  }

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
}

// updates OLED
// val is an int between 1 and 999
void displayVal(int val){  
  val = constrain(val, 0, 999);
  extractNewDigits(val);
  display.fillRect(00,0,120,40, BLACK);
  if(val<9){ 
    displayDigit(digits[0],0);
  } else if(val< 99){
    displayDigit(digits[0],0);
    displayDigit(digits[1],40);
  }else{
    displayDigit(digits[0],0);
    displayDigit(digits[1],40);
    displayDigit(digits[2],80);     
  }
}

/*
    I'm using Encoder lib, and its a very
    strange lib, and others were not working,
    so I'm dealing with it.
    The detents make me divide by 4.
    The unavoidable negative count makes me
    have a disgusting "fake" zero.
*/
long fakeZero = 0;
void calculateTicks(){
  long rawTick = myEnc.read();

  if(rawTick < fakeZero){
    fakeZero = rawTick;
  }
  long fakeTick = rawTick - fakeZero;

  tick = fakeTick / 4; // due to the knobs detent
  tick = constrain(tick, 1, 999);
}

void hint(String str){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,55);
  display.println(str);  
}

void editMode(){
  calculateTicks();

  if(buttonPressed){
    display.clearDisplay();
    buttonPressed = false;
    desiredDurationInSeconds = tick;
    timerStartedAt = millis(); 
    mode = RUN;
    return; 
  }

  displayVal(tick);
  hint("Push knob to start.");
  display.display();
}

unsigned int calculateSecondsRemaining(){
  unsigned long timeSinceStartMs = millis() - timerStartedAt;
  unsigned long timeSinceStart = timeSinceStartMs / 1000;
  return (desiredDurationInSeconds * 60) - timeSinceStart;
}

void runMode(){
  unsigned int secondsRemaining = calculateSecondsRemaining();
  if(secondsRemaining <= 0){
    strike();
    mode = FINISHED;
    return;
  }

  if(buttonPressed){
    buttonPressed = false;
    mode = EDIT;
    display.clearDisplay();
    return;
  }

  int minutesRemaining = secondsRemaining / 60.0;
  displayVal(minutesRemaining);
  display.fillRect(0,50,120,20, BLACK);
  hint(String(secondsRemaining % 60));
  display.display();
}

void finishedMode(){
  display.clearDisplay();
  drawStars(star,16,16);
}

long lastButtonPressAt = 0;

void buttonEvent(){
  long pressDelta = millis() - lastButtonPressAt;
  if(pressDelta < 1000) { return; } //debounce

  lastButtonPressAt = millis();
  buttonPressed = true;
}

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

void drawStars(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];
 
  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;
  }

  while (1) {
    if(buttonPressed){
      buttonPressed = false;
      mode = EDIT;
      return;
    } 
    // draw each icon
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, WHITE);
    }
    display.display();
    delay(200);
    
    // then erase it + move it
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, BLACK);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > display.height()) {
        icons[f][XPOS] = random(display.width());
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random(5) + 1;
      }
    }
  }
}

void showWelcomeScreen(){
  display.clearDisplay();
  display.drawBitmap(0, 0,  startScreen, 128, 64, 1);
  display.display();
  delay(3000);
  display.clearDisplay();
}

void setup() {
  mode = INIT;
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonEvent, RISING);
  pinMode(sigPin, OUTPUT);
  pinMode(1, OUTPUT); // tmp
  showWelcomeScreen();
  mode = EDIT;
}

void loop(){
  switch (mode) {
    case EDIT: 
      editMode(); break;
    case RUN:
      runMode(); break;
    case FINISHED:
      finishedMode(); break;
    default:
      editMode(); break;
    delay(10);
  }
}
