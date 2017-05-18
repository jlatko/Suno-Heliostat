#include <spa.h>
#include <mirror.h>
#include <clock.h>
#include <buttons.h>
#include <actions.h>
#include <Energia.h>

#define SPA_LON  -8.6166667
#define SPA_LAT  41.15
#define SPA_TIMEZONE 1
#define SPA_ELEVATION 0
#define SPA_TEMPERATURE 15
#define SPA_PRESSURE 1000
#define SPA_FUNC SPA_ALL

#define BUTTON_LEFT PUSH1
#define BUTTON_RIGHT PUSH2
#define BUTTON_UP 12
#define BUTTON_DOWN 13

spa_data spa;
Mirror mirror;
Clock timer;

// volatile int stepsV;
// volatile int stepsH;
// volatile int offsetV;
// volatile int offsetH;
// volatile int basicV;
// volatile int basicH;

void setup();
void loop();
void initSPA();
void onClick();
void initButtons();

void setup()
{
  // pinMode(GREEN_LED, OUTPUT);     // set pin as output

    Serial.begin(9600);
    delay(10);
    initSPA();
    initButtons();
    mirror.setMode(Mirror::DAY);
}

void loop()
{
  switch(mirror.getMode()){
    case Mirror::DAY:
    // if sunset -> endOfDay()
    // else if nextMove (optional) -> repostion
      if( timer.isSunset() ){
        endOfDay(mirror, timer);
      } else {
        reposition(mirror, timer);
      }
      break;
    case Mirror::END_OF_RANGE:
    // if sunset -> endOfDay()
      if( timer.isSunset() ){
        endOfDay(mirror, timer);
      } else {
        reposition(mirror, timer);
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
        reposition(mirror, timer);
      }
      break;
  }

  // Serial.println(Clock::getTime());
  // spa_calculate(&spa);
  // Serial.println(spa.incidence);
  // Serial.println(spa.azimuth);
  // Serial.println(spa.sunrise);
  // Serial.println(spa.sunset);
  // delay(20);

  sleep(mirror.getDelay());

  //tmp
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
void moveLeft() {
  if( mirror.getMode() != Mirror::MOVING && mirror.getMode() != Mirror::NIGHT ){
    mirror.setMode(Mirror::EDIT);
    timer.updateEdittingEnd();
    mirror.moveLeft();
  }
}
void moveRight() {
  if( mirror.getMode() != Mirror::MOVING  && mirror.getMode() != Mirror::NIGHT ){
    mirror.setMode(Mirror::EDIT);
    timer.updateEdittingEnd();
    mirror.moveRight();
  }
}
void moveUp() {
  if( mirror.getMode() != Mirror::MOVING  && mirror.getMode() != Mirror::NIGHT ){
    mirror.setMode(Mirror::EDIT);
    timer.updateEdittingEnd();
    mirror.moveUp();
  }
}
void moveDown() {
  if( mirror.getMode() != Mirror::MOVING  && mirror.getMode() != Mirror::NIGHT ){
    mirror.setMode(Mirror::EDIT);
    timer.updateEdittingEnd();
    mirror.moveDown();
  }
}

void initButtons(){
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  attachInterrupt(BUTTON_LEFT, moveLeft , FALLING);
  attachInterrupt(BUTTON_RIGHT, moveRight , FALLING);
  attachInterrupt(BUTTON_DOWN, moveDown , FALLING);
  attachInterrupt(BUTTON_UP, moveUp , FALLING);
}
