#ifndef DEBUG_H
#define DEBUG_H

#if DEBUG_ENABLED
#define PRINT(x) Serial.println(x); delay(20)
#define INIT_SERIAL() Serial.begin(9600)
#else
#define PRINT(x) do {} while(0)
#define INIT_SERIAL() do {} while(0)
#endif
#endif
