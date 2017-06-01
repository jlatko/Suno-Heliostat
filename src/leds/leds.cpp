#include "leds.h"
#include "print.h"
#include <Energia.h>
#include "inc/tm4c123gh6pm.h"

void initLeds(){
  PRINT("Init leds");
  // SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
  GPIO_PORTF_DIR_R |= RED|GREEN|BLUE;
  GPIO_PORTF_DEN_R |= RED|GREEN|BLUE;
  GPIO_PORTF_DATA_R &= ~(RED|GREEN|BLUE);
}

void ledsOff(){
  GPIO_PORTF_DATA_R &= ~(RED|GREEN|BLUE);
}

void blink(int color){
  GPIO_PORTF_DATA_R |= color;
  delay(20);
  ledsOff();
}
void setColor(int color){
  GPIO_PORTF_DATA_R |= color;
}

void testLeds(){
  blink(RED);
  delay(100);
  blink(BLUE);
  delay(100);
  blink(GREEN);
  delay(100);
  setColor(WHITE);
  delay(100);
  ledsOff();
}
