
#ifndef ACTIONS_H
#define ACTIONS_H
#include <spa.h>
#include <mirror/mirror.h>
#include <clock/clock.h>
#include <print.h>

void reposition(Mirror& mirror, Clock& clock, spa_data * spa);
void endOfDay(Mirror& mirror, Clock& clock, spa_data * spa);
void beginningOfDay(Mirror& mirror);
void recalculateSpa(Clock& clock, spa_data * spa);

#endif
