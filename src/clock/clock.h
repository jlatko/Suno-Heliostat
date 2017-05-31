#ifndef CLOCK_H
#define CLOCK_H

#include <Energia.h>
#include <ctime>
#include "spa.h"
#include "print.h"
#include "math.h"

// in seconds
#define EDITTING_DELAY 2

int minuteOfDay(const tm dt);

class Clock
{
private:
  // temporary timer mock
  volatile unsigned int counter = 0;

  tm sunset;
  tm sunrise;
  time_t edittingEnd;
public:
  //tmp
  void mockClock(){
    counter++;
    PRINT(counter);
  };

  time_t getTime();
  tm getTm();

  bool isSunset();
  bool isSunrise();
  bool isEdittingEnd();
  void updateEdittingEnd();
  void calculateSunsetSunrise(spa_data *spa);
};
#endif
