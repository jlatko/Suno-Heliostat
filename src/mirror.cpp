#include "mirror.h"
#include <Energia.h>

#define END_OF_RANGE_V 1000
#define END_OF_RANGE_H 1000

#define SETTUP_STEP_H 1
#define SETTUP_STEP_V 1

#define POLLING_DAY 4000
#define POLLING_END_OF_RANGE 1001
#define POLLING_MOVING 1002
#define POLLING_NIGHT 1003
#define POLLING_EDIT 1004

const unsigned int Mirror::delayTimes[]= {
  POLLING_DAY,
  POLLING_END_OF_RANGE,
  POLLING_MOVING,
  POLLING_NIGHT,
  POLLING_EDIT
};

// Setup: changes the offset after a single click
void Mirror::moveLeft(){
  // additional variable just for being super-cautious not to let the offset go out of range
  int tmpOffset = offsetH - SETTUP_STEP_H;
  if( tmpOffset >= -END_OF_RANGE_H ){
    offsetH = tmpOffset;
    Serial.print("LEFT: Offset H: " );
    Serial.println(offsetH);
    delay(20);
  }
}

void Mirror::moveRight(){
  int tmpOffset = offsetH + SETTUP_STEP_H;
  if( tmpOffset <= END_OF_RANGE_H ){
    offsetH = tmpOffset;
    Serial.print("Right: Offset H: " );
    Serial.println(offsetH);
    delay(20);
  }
}

void Mirror::moveUp(){
  int tmpOffset = offsetH + SETTUP_STEP_V;
  if( tmpOffset <= END_OF_RANGE_V ){
    offsetV = tmpOffset;
    Serial.print("UP: Offset V: " );
    Serial.println(offsetV);
    delay(20);
  }
}

void Mirror::moveDown(){
  int tmpOffset = offsetH - SETTUP_STEP_V;
  if( tmpOffset >= -END_OF_RANGE_V ){
    offsetV = tmpOffset;
    Serial.print("DOWN: Offset V: " );
    Serial.println(offsetV);
    delay(20);
  }
}
