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
#define BUTTON_LEFT 12
#define BUTTON_RIGHT 32

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

void setup()
{
    INIT_SERIAL();
    delay(10);
    initSPA();
    initButtons();
    initEndOfRangeSensors();
    mirror.init();
    recalculateSpa(clk, &spa);
    clk.calculateSunsetSunrise(&spa);
    mirror.setMode(Mirror::DAY);
    initLeds();
    testLeds();
    delay(10);
    PRINT("Hello");
    delay(20);
}

void loop()
{
  PRINT("loop");
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
        reposition(mirror, clk, &spa);
      }
      break;
  }

  sleep(mirror.getDelay());
  // temporary
  clk.mockClock();
}

void initSPA(){
  spa.longitude     = SPA_LON;
  spa.latitude      = SPA_LAT;
  spa.timezone      = SPA_TIMEZONE;
  spa.elevation     = SPA_ELEVATION;
  spa.pressure      = SPA_PRESSURE;
  spa.temperature   = SPA_TEMPERATURE;
  spa.function      = SPA_FUNC;

// ------
  spa.year = 2017;
  spa.month = 5;
  spa.day = 11;
  spa.hour = 12;
  spa.minute = 0;
  spa.second = 0;

}

// Editting works only during the day
void setLeft() {
  if( mirror.getMode() != Mirror::MOVING && mirror.getMode() != Mirror::NIGHT ){
    mirror.setMode(Mirror::EDIT);
    clk.updateEdittingEnd();
    mirror.setLeft();
  }
}
void setRight() {
  if( mirror.getMode() != Mirror::MOVING  && mirror.getMode() != Mirror::NIGHT ){
    mirror.setMode(Mirror::EDIT);
    clk.updateEdittingEnd();
    mirror.setRight();
  }
}
void setUp() {
  if( mirror.getMode() != Mirror::MOVING  && mirror.getMode() != Mirror::NIGHT ){
    mirror.setMode(Mirror::EDIT);
    clk.updateEdittingEnd();
    mirror.setUp();
  }
}
void setDown() {
  if( mirror.getMode() != Mirror::MOVING  && mirror.getMode() != Mirror::NIGHT ){
    mirror.setMode(Mirror::EDIT);
    clk.updateEdittingEnd();
    mirror.setDown();
  }
}

//Interrupts for end of range, has to reset the count,
void endOfRangeLeft(){
  mirror.touchLeft();
}

void endOfRangeRight(){
  mirror.touchRight();
}

void endOfRangeTop(){
  mirror.touchTop();
}

void endOfRangeBottom(){
  mirror.touchBottom();
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
