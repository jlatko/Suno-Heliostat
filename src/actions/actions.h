
#ifndef ACTIONS_H
#define ACTIONS_H
#include <spa.h>
#include <mirror/mirror.h>
#include <clock/clock.h>
#include <print.h>

// functions used in the main loop depending on the current mode and time

// regular repositioning
void reposition(Mirror& mirror, Clock& clock, spa_data * spa);

// end and begining of day behaviour
void endOfDay(Mirror& mirror, Clock& clock, spa_data * spa);
void beginningOfDay(Mirror& mirror);

// recalculates the Sun position and sunset and sunrise times
void recalculateSpa(Clock& clock, spa_data * spa);

#endif
