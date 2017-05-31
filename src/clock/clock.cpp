#include "clock.h"

int minuteOfDay(const tm dt){
    return dt.tm_hour*60 + dt.tm_hour;
}
time_t Clock::getTime(){
  time_t t;
  return t;
}

tm Clock::getTm(){
    tm tm;
    return tm;
}

bool Clock::isSunset(){
  return minuteOfDay(getTm()) > minuteOfDay(sunset);
}
bool Clock::isSunrise(){
  return minuteOfDay(getTm()) > minuteOfDay(sunset);
}
bool Clock::isEdittingEnd(){
  return getTime() >= edittingEnd;
}
void Clock::updateEdittingEnd(){
  edittingEnd = getTime() + EDITTING_DELAY;
}

void Clock::calculateSunsetSunrise(spa_data *spa){
  sunset.tm_hour = spa->sunset;
  sunset.tm_min = ((int)(spa->sunset*60))%60;
  sunrise.tm_hour = spa->sunrise;
  sunrise.tm_min = ((int)(spa->sunrise*60))%60;
  // sunrise = DateTime( 0, 0, 0, spa->sunrise, (int)(spa->sunrise*60)%60);
}
