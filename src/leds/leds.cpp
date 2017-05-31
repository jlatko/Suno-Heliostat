#include "leds.h"
#include <Energia.h>
#include "inc/tm4c123gh6pm.h"

#define RED ( 1 << 1)
#define BLUE ( 1 << 2)
#define GREEN ( 1 << 3)

void initLeds(){
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
  GPIO_PORTF_DIR_R = RED|GREEN|BLUE;
  GPIO_PORTF_DEN_R = RED|GREEN|BLUE;
  GPIO_PORTF_DATA_R = 0x00000000;
}

void ledsOff(){
  GPIO_PORTF_DATA_R = 0x00000000;
}

void red(){
  GPIO_PORTF_DATA_R = RED;
}
void blue(){
  GPIO_PORTF_DATA_R = BLUE;
}
void green(){
  GPIO_PORTF_DATA_R = GREEN;
}
void yellow(){
  GPIO_PORTF_DATA_R = GREEN|RED;
}
void pink(){
  GPIO_PORTF_DATA_R = RED|BLUE;
}
void cyan(){
  GPIO_PORTF_DATA_R = GREEN|BLUE;
}
void white(){
  GPIO_PORTF_DATA_R = GREEN|RED|BLUE;
}

void testLeds(){
  red();
  delay(100);
  green();
  delay(100);
  blue();
  delay(100);
  yellow();
  delay(100);
  pink();
  delay(100);
  cyan();
  delay(100);
  white();
  delay(100);
  ledsOff();
}
