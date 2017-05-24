
#ifndef ACTIONS_H
#define ACTIONS_H
#include <spa.h>
#include <mirror/mirror.h>
#include <clock/clock.h>

void reposition(Mirror& mirror, Clock& clock, spa_data * spa);
void endOfDay(Mirror& mirror, Clock& clock);
void beginningOfDay(Mirror& mirror);

#endif
