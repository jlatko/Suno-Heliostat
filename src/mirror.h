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
  void setMode(Mode mode) {
    this->mode = mode;
    Serial.print("Setting mode: ");
    Serial.println(mode);
    delay(10);
  };

  void toBeginning(){
    setMode(Mirror::MOVING);
    Serial.println("To begining...");
    delay(20);
    desiredH = BEGINING_H;
    desiredV = BEGINING_V;
    repositionH();
    repositionV();
    setMode(Mirror::NIGHT);
  };

  void repositionToReset(){
    setMode(Mirror::MOVING);
    Serial.println("To reset...");
    delay(20);
    // idea: set the steps count to a very big number (over END_OF_RANGE) and just reposition to the oposite end (the interrupt will do the work)
    stepsH = END_OF_RANGE_H;
    stepsV = END_OF_RANGE_V;
    desiredH = - END_OF_RANGE_H;
    desiredV = - END_OF_RANGE_V;
    repositionH();
    repositionV();
    setMode(Mirror::DAY);
  };

  void reposition(){
    desiredH = basicH + offsetH;
    if( desiredH > END_OF_RANGE_H){
      desiredH = END_OF_RANGE_H;
    }else if( desiredH < -END_OF_RANGE_H ){
      desiredH = -END_OF_RANGE_H;
    }

    desiredV = basicV + offsetV;
    if( desiredH > END_OF_RANGE_H){
      desiredH = END_OF_RANGE_H;
    }else if( desiredH < -END_OF_RANGE_H ){
      desiredH = -END_OF_RANGE_H;
    }
    setMode(Mirror::MOVING);
    // TODO: handle end of range
    Serial.println("Moving...");
    delay(20);
    repositionH();
    repositionV();
    if( desiredH == END_OF_RANGE_H || desiredV == END_OF_RANGE_V ){
      setMode(Mirror::END_OF_RANGE);
    } else {
      setMode(Mirror::DAY);
    }
  };

// returns false if reached end of range
  bool repositionH();
  bool repositionV();

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
