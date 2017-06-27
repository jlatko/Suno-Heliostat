#include <spa.h>
#include <mirror/mirror.h>
#include <clock/clock.h>
#include <actions/actions.h>
#include <Energia.h>
#include <leds/leds.h>
#include <print.h>

#define SPA_LON  -8.6166667
#define SPA_LAT  41.15
#define SPA_TIMEZONE 1
#define SPA_ELEVATION 0
#define SPA_TEMPERATURE 15
#define SPA_PRESSURE 1000
#define SPA_FUNC SPA_ALL

#define BUTTON_UP 11
#define BUTTON_DOWN 31
#define BUTTON_LEFT 12
#define BUTTON_RIGHT 32

#define SLEEP_TIME 500
#define TICKS_PER_SECOND (1000/SLEEP_TIME)


using namespace std;

// variables for the sun position algorithm, mirror state and clock
spa_data spa;
Mirror mirror;
Clock clk;

// timer used for polling in main loop
// set to max to trigger first loop immediatelly
volatile uint16_t timer = 0xFFFF;

void setup();
void loop();
void initSPA();
void onClick();
void initButtons();
void initEndOfRangeSensors();
void reset();

// all components and variables are initialised
void setup()
{
    INIT_SERIAL();
    initSPA();
    initButtons();
    initEndOfRangeSensors();
    initLeds();
    testLeds();
    clk.init();
    mirror.init();

    // set reset to the PUSH2 button on the tiva c board
    pinMode(PUSH2, INPUT_PULLUP);
    attachInterrupt(PUSH2, reset, FALLING);

    recalculateSpa(clk, &spa);
    clk.calculateSunsetSunrise(&spa);
    mirror.setMode(Mirror::DAY);
    PRINT("Hello");
}


void loop()
{
  // polling time depends on the mode
  if(timer/TICKS_PER_SECOND >= mirror.getDelay()){
    timer = 0;
    // chooses the right action depending on the mode and time
    switch(mirror.getMode()){
      // END_OF_RANGE has longer polling than DAY, but still checks the repositioning
      case Mirror::DAY:
      case Mirror::END_OF_RANGE:
        if( clk.isSunset() ){
          // if it is after sunset the mirror will move to the begining of range, recalculate sunset and sunrise values and stop repositioning untill the sunrise (mode set to NIGHT)
          endOfDay(mirror, clk, &spa);
        } else {
          // else if the sun is still shining the mirror will reposition itself
          reposition(mirror, clk, &spa);
        }
        break;
      case Mirror::NIGHT:
        if( clk.isSunrise() ){
          // if it is after sunrise the mirror will move to touch the end of range sensors and start repositioning (mode set to DAY)
          beginningOfDay(mirror);
        }
        break;
      case Mirror::EDIT:
        if( clk.isEdittingEnd() ){
          // if a certain amount of time passed from the last button press, the offset values will be saved to the memory and the mirror will reposition itself
          mirror.saveOffsets();
          PRINT("editting end");
          reposition(mirror, clk, &spa);
        }
        break;
    }
  }
  timer++;
  sleep(SLEEP_TIME);
}

void initSPA(){
  spa.longitude     = SPA_LON;
  spa.latitude      = SPA_LAT;
  spa.timezone      = SPA_TIMEZONE;
  spa.elevation     = SPA_ELEVATION;
  spa.pressure      = SPA_PRESSURE;
  spa.temperature   = SPA_TEMPERATURE;
  spa.function      = SPA_FUNC;
}

// Button Interrupts
// Editting works only during the day and is disabled during the night or while the mirror is moving
void setLeft() {
  PRINT("left");
  if( mirror.getMode() != Mirror::MOVING && mirror.getMode() != Mirror::NIGHT ){
    mirror.setMode(Mirror::EDIT);
    clk.updateEdittingEnd();
    mirror.setLeft();
  }
  blink(GREEN);
}
void setRight() {
  PRINT("right");
  if( mirror.getMode() != Mirror::MOVING  && mirror.getMode() != Mirror::NIGHT ){
    mirror.setMode(Mirror::EDIT);
    clk.updateEdittingEnd();
    mirror.setRight();
  }
  blink(GREEN);
}
void setUp() {
  PRINT("Up");
  if( mirror.getMode() != Mirror::MOVING  && mirror.getMode() != Mirror::NIGHT ){
    mirror.setMode(Mirror::EDIT);
    clk.updateEdittingEnd();
    mirror.setUp();
  }
  blink(GREEN);
}
void setDown() {
  PRINT("Down");
  if( mirror.getMode() != Mirror::MOVING  && mirror.getMode() != Mirror::NIGHT ){
    mirror.setMode(Mirror::EDIT);
    clk.updateEdittingEnd();
    mirror.setDown();
  }
  blink(GREEN);
}

//Interrupts for end of range
void endOfRangeLeft(){
  mirror.touchLeft();
  blink(RED);
}

void endOfRangeRight(){
  mirror.touchRight();
  blink(RED);
}

void endOfRangeTop(){
  mirror.touchTop();
  blink(RED);
}

void endOfRangeBottom(){
  mirror.touchBottom();
  blink(RED);
}

void initButtons(){
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  attachInterrupt(BUTTON_LEFT, setLeft , FALLING);
  attachInterrupt(BUTTON_RIGHT, setRight , FALLING);
  attachInterrupt(BUTTON_DOWN, setDown , FALLING);
  attachInterrupt(BUTTON_UP, setUp , FALLING);
}

void initEndOfRangeSensors(){
  pinMode(LEFT_END_PIN, INPUT_PULLUP);
  pinMode(RIGHT_END_PIN, INPUT_PULLUP);
  pinMode(BOTTOM_END_PIN, INPUT_PULLUP);
  pinMode(TOP_END_PIN, INPUT_PULLUP);

  attachInterrupt(LEFT_END_PIN, endOfRangeLeft , FALLING);
  attachInterrupt(RIGHT_END_PIN, endOfRangeRight , FALLING);
  attachInterrupt(BOTTOM_END_PIN, endOfRangeBottom , FALLING);
  attachInterrupt(TOP_END_PIN, endOfRangeTop , FALLING);
}

// reset button interrupt
void reset(){
  PRINT("reset");
  blink(WHITE);
  mirror.reset();
}
