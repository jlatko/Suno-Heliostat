#include "actions.h"
#include <Energia.h>

void reposition(Mirror& mirror, Clock& clock, spa_data * spa){
  Serial.println("Repositioning...");

  mirror.calculateBasic(spa);
  mirror.reposition();
  delay(20);
}

void endOfDay(Mirror& mirror, Clock& clock){
  Serial.println("End of day");
  delay(20);
  clock.calculateSunsetSunrise();
  mirror.toBeginning();
  mirror.setMode(Mirror::NIGHT);
}

void beginningOfDay(Mirror& mirror){
  Serial.println("Begenning of day");
  mirror.repositionToReset();
  delay(20);
}
