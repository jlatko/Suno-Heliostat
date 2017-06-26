#ifndef DEBUG_H
#define DEBUG_H

// printing to the serial interface is only done if debugging flag is enabled
#if DEBUG_ENABLED
#define PRINT(x) Serial.println(x); delay(20)
#define PRINT2(x,y) Serial.print(x);Serial.println(y); delay(30)
#define INIT_SERIAL() Serial.begin(9600); delay(10)
#else
#define PRINT(x) do {} while(0)
#define PRINT2(x,y) do {} while(0)
#define INIT_SERIAL() do {} while(0)
#endif
#endif
