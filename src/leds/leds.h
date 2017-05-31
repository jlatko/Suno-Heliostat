#ifndef LEDS_H
#define LEDS_H

#define RED ( 1 << 1)
#define BLUE ( 1 << 2)
#define GREEN ( 1 << 3)
#define YELLOW RED|GREEN
#define PINK RED|BLUE
#define CYAN GREEN|BLUE
#define WHITE RED|GREEN|BLUE

void initLeds();
void blink(int color);

void ledsOff();
void red();
void blue();
void green();
void yellow();
void pink();
void cyan();
void white();
void testLeds();

#endif
