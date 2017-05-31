#include <spa.h>
#include <Energia.h>
#include "print.h"
#ifndef MIRROR_H
#define MIRROR_H

// #define STEPS_PER_DEGREE_V 10
#define STEPS_PER_DEGREE_H 10

#define END_OF_RANGE_TOP 8
#define END_OF_RANGE_BOTTOM -8
#define END_OF_RANGE_H 20

#define END_OF_RANGE_TOP_ANGLE 30
#define END_OF_RANGE_BOTTOM_ANGLE -30
#define END_OF_RANGE_H_ANGLE 60

#define BEGINING_H (-END_OF_RANGE_H + 5)
#define BEGINING_V (END_OF_RANGE_BOTTOM + 5)

// end of range sensor pins
#define LEFT_END_PIN 9
#define RIGHT_END_PIN 29
#define BOTTOM_END_PIN 8
#define TOP_END_PIN 28

#define SETTUP_STEP_H 0.2
#define SETTUP_STEP_V 0.2

// how long is the delay in main loop
#define POLLING_DAY 1000
#define POLLING_END_OF_RANGE 1001
#define POLLING_MOVING 1002
#define POLLING_NIGHT 1003
#define POLLING_EDIT 100

// how long the coil is supplied with voltage and how long is the delay after the move
#define MOTOR_SIGNAL_HIGH_TIME 50
#define MOTOR_SIGNAL_GAP_TIME 200

// #define ENABLE_H_PIN 8
#define COIL_1_A_H_PIN 2
#define COIL_1_B_H_PIN 23
#define COIL_2_A_H_PIN 4
#define COIL_2_B_H_PIN 24

// #define ENABLE_V_PIN 26
#define COIL_1_A_V_PIN 36
#define COIL_1_B_V_PIN 16
#define COIL_2_A_V_PIN 35
#define COIL_2_B_V_PIN 15

#define STEPS_H_ADDRESS 0x00000200
#define STEPS_V_ADDRESS 0x00000204
#define OFFSET_H_ADDRESS 0x00000208
#define OFFSET_V_ADDRESS 0x00000212

class Mirror {
public:
  enum Mode {
    DAY,
    END_OF_RANGE,
    MOVING,
    NIGHT,
    EDIT
  };

  void calculateBasic(spa_data *spa);

  Mode getMode() { return mode; };
  int getDelay() { return delayTimes[mode]; };
  void setMode(Mode mode) { this->mode = mode; };

  void toBeginning();
  void repositionToReset();
  void reposition();

  void repositionH();
  void repositionV();

  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();

  void setLeft();
  void setRight();
  void setUp();
  void setDown();

  void makeStepH();
  void makeStepV();

  void touchLeft();
  void touchRight();
  void touchTop();
  void touchBottom();

  void saveOffsets();
  void saveStepsH();
  void saveStepsV();
  void reset();

  int angleToStepsH(float angle);
  int angleToStepsV(float angle);

  void init();
private:
  // bool touchedLeft = false;
  // bool touchedRight = false;
  // bool touchedTop = false;
  // bool touchedBottom = false;
  volatile Mode mode;
  volatile int stepsV = 0;
  volatile int stepsH = 0;
  volatile float offsetAngleV = 0;
  volatile float offsetAngleH = 0;
  volatile double basicAngleV = 0;
  volatile double basicAngleH = 0;
  volatile int desiredV = 0;
  volatile int desiredH = 0;
  static const unsigned int delayTimes[5];
  static const unsigned int stepNumberToPinH[4];
  static const unsigned int stepNumberToPinV[4];
};



#endif