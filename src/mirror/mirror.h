#include <spa.h>
#include <Energia.h>
#ifndef MIRROR_H
#define MIRROR_H

#define STEPS_PER_DEGREE_V 10
#define STEPS_PER_DEGREE_H 10

#define END_OF_RANGE_V 8
#define END_OF_RANGE_H 20

#define BEGINING_H (-END_OF_RANGE_H + 5)
#define BEGINING_V (-END_OF_RANGE_V + 5)

// end of range sensor pins
#define LEFT_END_PIN PUSH1
#define RIGHT_END_PIN PUSH2
#define BOTTOM_END_PIN 22
#define TOP_END_PIN 23

#define SETTUP_STEP_H 1
#define SETTUP_STEP_V 1

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
#define COIL_11_A_H_PIN 10
#define COIL_11_B_H_PIN 30
#define COIL_12_A_H_PIN 9
#define COIL_12_B_H_PIN 29

// #define ENABLE_V_PIN 26
#define COIL_11_A_V_PIN 7
#define COIL_11_B_V_PIN 27
#define COIL_12_A_V_PIN 8
#define COIL_12_B_V_PIN 28

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

  void init();
private:
  bool touchedLeft = false;
  bool touchedRight = false;
  bool touchedTop = false;
  bool touchedBottom = false;
  volatile Mode mode;
  volatile int stepsV = 0;
  volatile int stepsH = 0;
  volatile int offsetV = 0;
  volatile int offsetH = 0;
  volatile int basicV = 0;
  volatile int basicH = 0;
  volatile int desiredV = 0;
  volatile int desiredH = 0;
  static const unsigned int delayTimes[5];
  static const unsigned int stepNumberToPinH[4];
  static const unsigned int stepNumberToPinV[4];
};



#endif
