#ifndef CLOCK_H
#define CLOCK_H

#define EDITTING_DELAY 1000

class Clock
{
private:
  // temporary timer mock
  volatile unsigned int counter = 0;

  //time sunset
  //time sunrise
  //time editting end
public:
  //tmp
  void mockClock(){ counter++; };

  int getTime();
  bool isSunset(){
    return counter > 10;
  };
  bool isSunrise(){
    if( counter > 20){
      counter = 0;
      return true;
    }
    return false;
  };
  bool isEdittingEnd(){
    return counter > 6;
  };
  void updateEdittingEnd(){};
  void calculateSunsetSunrise(){};

};

#endif
