#include <spa.h>
#include <mirror.h>
#include <clock.h>
#include <buttons.h>
#include <actions.h>
#include <Energia.h>
#include <Wire.h>
// #include "RTClib.h"

#define SPA_LON  -8.6166667
#define SPA_LAT  41.15
#define SPA_TIMEZONE 1
#define SPA_ELEVATION 0
#define SPA_TEMPERATURE 15
#define SPA_PRESSURE 1000
#define SPA_FUNC SPA_ALL

#define BUTTON_LEFT 20
#define BUTTON_RIGHT 21
#define BUTTON_UP 12
#define BUTTON_DOWN 13

// RTC_PCF8523 rtc;

spa_data spa;
Mirror mirror;
Clock timer;

void setup();
void loop();
void initSPA();
void onClick();
void initButtons();
void initEndOfRangeSensors();

void setup()
{
  // pinMode(GREEN_LED, OUTPUT);     // set pin as output

    Serial.begin(9600);
    delay(10);
    initSPA();
    initButtons();
    initEndOfRangeSensors();
    mirror.init();
    mirror.setMode(Mirror::DAY);
    delay(10);

}

void loop()
{

  switch(mirror.getMode()){
    // END_OF_RANGE has longer polling than DAY, but still checks the repositioning in case the user changes the offset
    case Mirror::DAY:
    case Mirror::END_OF_RANGE:
      if( timer.isSunset() ){
        endOfDay(mirror, timer);
      } else {
        reposition(mirror, timer, &spa);
      }
      break;
    case Mirror::MOVING:
    // nothing ?
      break;
    case Mirror::NIGHT:
      // if sunrise in 5 min beginningOfDay
      if( timer.isSunrise() ){
        beginningOfDay(mirror);
      }
      break;
    case Mirror::EDIT:
      if( timer.isEdittingEnd() ){
        reposition(mirror, timer, &spa);
      }
      break;
  }

  sleep(mirror.getDelay());
  // temporary
  timer.mockClock();
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
    timer.updateEdittingEnd();
    mirror.setLeft();
  }
}
void setRight() {
  if( mirror.getMode() != Mirror::MOVING  && mirror.getMode() != Mirror::NIGHT ){
    mirror.setMode(Mirror::EDIT);
    timer.updateEdittingEnd();
    mirror.setRight();
  }
}
void setUp() {
  if( mirror.getMode() != Mirror::MOVING  && mirror.getMode() != Mirror::NIGHT ){
    mirror.setMode(Mirror::EDIT);
    timer.updateEdittingEnd();
    mirror.setUp();
  }
}
void setDown() {
  if( mirror.getMode() != Mirror::MOVING  && mirror.getMode() != Mirror::NIGHT ){
    mirror.setMode(Mirror::EDIT);
    timer.updateEdittingEnd();
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
