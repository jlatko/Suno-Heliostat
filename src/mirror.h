#include <spa.h>
#include <Energia.h>
#ifndef MIRROR_H
#define MIRROR_H


#define STEPS_PER_DEGREE_V 10
#define STEPS_PER_DEGREE_H 10


class Mirror {
public:
  enum Mode {
    DAY,
    END_OF_RANGE,
    MOVING,
    NIGHT,
    EDIT
  };
  Mirror calculateBasic(spa_data *spa);
  // void repositionToSun(){};

  Mode getMode() { return mode; };
  int getDelay() { return delayTimes[mode]; };
  void setMode(Mode mode) {
    this->mode = mode;
    Serial.print("Setting mode: ");
    Serial.println(mode);
    delay(10);
  };
  void toBeginning(){};

  void repositionToReset(){
    setMode(Mirror::MOVING);
    Serial.println("To reset...");
    delay(5000);
    setMode(Mirror::DAY);
  };

  void repositionToCalculated(){
    setMode(Mirror::MOVING);
    Serial.println("Moving...");
    delay(5000);
    setMode(Mirror::DAY);
  };

  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();

private:
  volatile Mode mode;
  volatile int stepsVb = 0;
  volatile int stepsH = 0;
  volatile int offsetV = 0;
  volatile int offsetH = 0;
  volatile int basicV = 0;
  volatile int basicH = 0;
  static const unsigned int delayTimes[5];
};



#endif
