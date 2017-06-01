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
  PRINT("Recalculate SPA");
  tm dt = clock.getTm();
  spa->year = dt.tm_year + 1900;
  spa->month = dt.tm_mon + 1;
  spa->day = dt.tm_mday;
  spa->hour = dt.tm_hour;
  spa->minute = dt.tm_min;
  spa->second = dt.tm_sec;
  spa_calculate(spa);
  PRINT2("year: ", spa->year);
  PRINT2("month: ", spa->month);
  PRINT2("day: ", spa->day);
  PRINT2("hour: ", spa->hour);
  PRINT2("minute: ", spa->minute);
  PRINT2("second: ", spa->second);
  PRINT2("azimuth: ", spa->azimuth_astro);
  PRINT2("incidence: ", spa->incidence);
  PRINT2("sunrise: ", spa->sunrise);
  PRINT2("sunset: ", spa->sunset);
}
