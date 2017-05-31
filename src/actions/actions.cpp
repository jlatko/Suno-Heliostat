#include "actions.h"
#include <Energia.h>

void reposition(Mirror& mirror, Clock& clk, spa_data * spa){
  PRINT("Repositioning...");
  recalculateSpa(clk, spa);
  mirror.calculateBasic(spa);
  mirror.reposition();
}

void endOfDay(Mirror& mirror, Clock& clk, spa_data * spa){
  PRINT("End of day");
  delay(20);
  recalculateSpa(clk, spa);
  clk.calculateSunsetSunrise(spa);
  mirror.toBeginning();
  mirror.setMode(Mirror::NIGHT);
}

void beginningOfDay(Mirror& mirror){
  PRINT("Begenning of day");
  mirror.repositionToReset();
}

void recalculateSpa(Clock &clock, spa_data * spa){
  tm dt = clock.getTm();
  spa->year = dt.tm_year;
  spa->month = dt.tm_mon;
  spa->day = dt.tm_mday;
  spa->hour = dt.tm_hour;
  spa->minute = dt.tm_min;
  spa->second = dt.tm_sec;
}
