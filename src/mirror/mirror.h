// Notice: all the variables, functions etc finished with V refer to the vertical movement of the mirror
// and those finished with H refer to the horizontal movement

#include <spa.h>
#include <Energia.h>
#include "print.h"
#ifndef MIRROR_H
#define MIRROR_H

#define STEPS_PER_DEGREE_H 22
#define STEPS_PER_TURN 200

// steper motor counters of end of range positions
#define END_OF_RANGE_TOP 16264
#define END_OF_RANGE_BOTTOM -8024
#define END_OF_RANGE_H 40*22

#define END_OF_RANGE_TOP_ANGLE 60
#define END_OF_RANGE_BOTTOM_ANGLE -30
#define END_OF_RANGE_H_ANGLE 40

// begining of day stepper motor counters
#define BEGINING_H (-END_OF_RANGE_H + 25)
#define BEGINING_V (END_OF_RANGE_BOTTOM + 30)

// end of range sensor pins
#define LEFT_END_PIN 9
#define RIGHT_END_PIN 29
#define BOTTOM_END_PIN 8
#define TOP_END_PIN 28

// defines how much the offset angles change on button press
#define SETTUP_STEP_H 0.5
#define SETTUP_STEP_V 0.5

// how long is the delay in main loop, in seconds
#define POLLING_DAY 60
#define POLLING_END_OF_RANGE 120
#define POLLING_MOVING 2
#define POLLING_NIGHT 240
#define POLLING_EDIT 0

// how long the coil is supplied with voltage and how long is the delay after the move
#define MOTOR_SIGNAL_HIGH_TIME 8
#define MOTOR_SIGNAL_GAP_TIME 4
#define MOTOR_SIGNAL_GAP_TIME_H 20

// #define ENABLE_H_PIN 8
#define COIL_1_A_H_PIN 3
#define COIL_1_B_H_PIN 4
#define COIL_2_A_H_PIN 36
#define COIL_2_B_H_PIN 35

// #define ENABLE_V_PIN 26
#define COIL_1_A_V_PIN 5
#define COIL_1_B_V_PIN 6
#define COIL_2_A_V_PIN 34
#define COIL_2_B_V_PIN 33

// EEPROM memory addresses
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

  void allLowH();
  void allLowV();
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
  static const unsigned int stepNumberToPinH[4][2];
  static const unsigned int stepNumberToPinV[4][2];
};



#endif
