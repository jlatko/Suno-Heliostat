#include <spa.h>
#include <mirror/mirror.h>
#include <clock/clock.h>
#include <actions/actions.h>
#include <Energia.h>
#include <leds/leds.h>
#include <print.h>
// #define PRINT(x) Serial.println(x); delay(20)

#define SPA_LON  -8.6166667
#define SPA_LAT  41.15
#define SPA_TIMEZONE 1
#define SPA_ELEVATION 0
#define SPA_TEMPERATURE 15
#define SPA_PRESSURE 1000
#define SPA_FUNC SPA_ALL

#define BUTTON_UP 11
#define BUTTON_DOWN 31
#define BUTTON_LEFT 32
#define BUTTON_RIGHT 12

using namespace std;

spa_data spa;
Mirror mirror;
Clock clk;

void setup();
void loop();
void initSPA();
void onClick();
void initButtons();
void initEndOfRangeSensors();
void reset();

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

    pinMode(PUSH2, INPUT_PULLUP);
    attachInterrupt(PUSH2, reset, FALLING);

    recalculateSpa(clk, &spa);
    clk.calculateSunsetSunrise(&spa);
    mirror.setMode(Mirror::DAY);
    PRINT("Hello");
}

void loop()
{
  switch(mirror.getMode()){
    // END_OF_RANGE has longer polling than DAY, but still checks the repositioning in case the user changes the offset
    case Mirror::DAY:
    case Mirror::END_OF_RANGE:
      if( clk.isSunset() ){
        endOfDay(mirror, clk, &spa);
      } else {
        reposition(mirror, clk, &spa);
      }
      break;
    case Mirror::NIGHT:
      if( clk.isSunrise() ){
        beginningOfDay(mirror);
      }
      break;
    case Mirror::EDIT:
      if( clk.isEdittingEnd() ){
        mirror.saveOffsets();
        PRINT("editting end");
        reposition(mirror, clk, &spa);
      }
      break;
  }

  sleep(mirror.getDelay());
  PRINT("loop delay...");
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

// Editting works only during the day
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

//Interrupts for end of range, has to reset the count,
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
  // pinMode(LEFT_END_PIN, INPUT);
  // pinMode(RIGHT_END_PIN, INPUT);
  // pinMode(BOTTOM_END_PIN, INPUT);
  // pinMode(TOP_END_PIN, INPUT);

// for testing with internal pullups
  pinMode(LEFT_END_PIN, INPUT_PULLUP);
  pinMode(RIGHT_END_PIN, INPUT_PULLUP);
  pinMode(BOTTOM_END_PIN, INPUT_PULLUP);
  pinMode(TOP_END_PIN, INPUT_PULLUP);

  attachInterrupt(LEFT_END_PIN, endOfRangeLeft , FALLING);
  attachInterrupt(RIGHT_END_PIN, endOfRangeRight , FALLING);
  attachInterrupt(BOTTOM_END_PIN, endOfRangeBottom , FALLING);
  attachInterrupt(TOP_END_PIN, endOfRangeTop , FALLING);
}

void reset(){
  PRINT("reset");
  blink(WHITE);
  mirror.reset();
}
