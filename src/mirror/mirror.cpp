#include "mirror.h"
#include "leds/leds.h"
#include "memory/memory.h"
#include <Energia.h>

// vertical movement angle to steps polynomial coefficients
#define A5 double(-3.962e-08)
#define A4 double(3.242e-06)
#define A3 double(-2.728e-05)
#define A2 double(-0.00314)
#define A1 double(1.387)

// delay times for each mirror mode
const unsigned int Mirror::delayTimes[]= {
  POLLING_DAY,
  POLLING_END_OF_RANGE,
  POLLING_MOVING,
  POLLING_NIGHT,
  POLLING_EDIT
};

// 4 phases of the stepper motors
const unsigned int Mirror::stepNumberToPinH[][2]= {
  {COIL_1_A_H_PIN, COIL_2_A_H_PIN},
  {COIL_1_B_H_PIN, COIL_2_A_H_PIN},
  {COIL_1_B_H_PIN, COIL_2_B_H_PIN},
  {COIL_1_A_H_PIN, COIL_2_B_H_PIN}
};
const unsigned int Mirror::stepNumberToPinV[][2]= {
  {COIL_1_A_V_PIN, COIL_2_A_V_PIN},
  {COIL_1_B_V_PIN, COIL_2_A_V_PIN},
  {COIL_1_B_V_PIN, COIL_2_B_V_PIN},
  {COIL_1_A_V_PIN, COIL_2_B_V_PIN}
};

// INIT: sets the motor control pins to output
void Mirror::init(){
  pinMode(COIL_1_A_H_PIN, OUTPUT);
  pinMode(COIL_1_B_H_PIN, OUTPUT);
  pinMode(COIL_2_A_H_PIN, OUTPUT);
  pinMode(COIL_2_B_H_PIN, OUTPUT);

  pinMode(COIL_1_A_V_PIN, OUTPUT);
  pinMode(COIL_1_B_V_PIN, OUTPUT);
  pinMode(COIL_2_A_V_PIN, OUTPUT);
  pinMode(COIL_2_B_V_PIN, OUTPUT);

// If not end of range set steps from memory
  if(digitalRead(LEFT_END_PIN) == LOW){
    stepsH = -END_OF_RANGE_H;
    saveStepsH();
  } else if(digitalRead(RIGHT_END_PIN) == LOW){
    stepsH = END_OF_RANGE_H;
    saveStepsH();
  }else{
    stepsH = (int)read(STEPS_H_ADDRESS);
  }

  if(digitalRead(BOTTOM_END_PIN) == LOW){
    stepsV = END_OF_RANGE_TOP;
    saveStepsV();
  } else if(digitalRead(TOP_END_PIN) == LOW){
    stepsV = END_OF_RANGE_BOTTOM;
    saveStepsV();
  }else{
    stepsV = (int)read(STEPS_V_ADDRESS);
  }

  // read offsets from the memory
  uint32_t offsetBytesH,offsetBytesV;
  offsetBytesH = read(OFFSET_H_ADDRESS);
  offsetBytesV = read(OFFSET_V_ADDRESS);
  memcpy((char*)&offsetAngleH, (char*)&offsetBytesH, 4);
  memcpy((char*)&offsetAngleV, (char*)&offsetBytesV, 4);
  PRINT("Init mirror");
  PRINT2("Offset H: ",offsetAngleH);
  PRINT2("Offset V: ",offsetAngleV);
  PRINT2("Steps H: ",stepsH);
  PRINT2("Steps V: ",stepsV);
}

// Memory management - saving steps and settings to the memory
void Mirror::saveOffsets(){
  uint32_t offsetBytesH,offsetBytesV;
  memcpy((char*)&offsetBytesH, (char*)&offsetAngleH, 4);
  memcpy((char*)&offsetBytesV, (char*)&offsetAngleV, 4);
  write(OFFSET_H_ADDRESS, offsetBytesH);
  write(OFFSET_V_ADDRESS, offsetBytesV);
}

void Mirror::saveStepsH(){
  write(STEPS_H_ADDRESS, stepsH);
}

void Mirror::saveStepsV(){
  write(STEPS_V_ADDRESS, stepsV);
}

// resets counters and offsets to 0 and writes to the memory
void Mirror::reset(){
  write(STEPS_V_ADDRESS, 0);
  write(STEPS_H_ADDRESS, 0);
  write(OFFSET_H_ADDRESS, 0);
  write(OFFSET_V_ADDRESS, 0);
  stepsH = 0;
  stepsV = 0;
  offsetAngleH = 0;
  offsetAngleV = 0;
}


// Angle to number of steps mappings
int Mirror::angleToStepsH(float angle){
  return (int)(angle * STEPS_PER_DEGREE_H);
}

int Mirror::angleToStepsV(float angle){
  return (int)((A5*angle*angle*angle*angle*angle
             + A4*angle*angle*angle*angle
             + A3*angle*angle*angle
             + A2*angle*angle
             + A1*angle) * STEPS_PER_TURN);
}

// calculates basic mirrors angles having actual sun position data
void Mirror::calculateBasic(spa_data *spa){
  basicAngleH = (spa->azimuth - 180)/2;
  basicAngleV = (90 - spa->incidence)/2;
  PRINT2("Basic H: ",basicAngleH);
  PRINT2("Basic V: ",basicAngleV);
}

// === Setup: changes the offset after a single click  ===
void Mirror::setLeft(){
  float tmpOffset = offsetAngleH - SETTUP_STEP_H;
  if( tmpOffset >= -END_OF_RANGE_H_ANGLE ){
    offsetAngleH = tmpOffset;
    PRINT("LEFT: Offset H: " );
    PRINT(offsetAngleH);
  }
}

void Mirror::setRight(){
  float tmpOffset = offsetAngleH + SETTUP_STEP_H;
  if( tmpOffset <= END_OF_RANGE_H_ANGLE ){
    offsetAngleH = tmpOffset;
    PRINT("Right: Offset H: " );
    PRINT(offsetAngleH);
  }
}

void Mirror::setUp(){
  float tmpOffset = offsetAngleV + SETTUP_STEP_V;
  if( tmpOffset <= END_OF_RANGE_TOP_ANGLE ){
    offsetAngleV = tmpOffset;
    PRINT("UP: Offset V: " );
    PRINT(offsetAngleV);
  }
}

void Mirror::setDown(){
  float tmpOffset = offsetAngleV - SETTUP_STEP_V;
  if( tmpOffset >= END_OF_RANGE_BOTTOM_ANGLE ){
    offsetAngleV = tmpOffset;
    PRINT("DOWN: Offset V: " );
    PRINT(offsetAngleV);
  }
}

// --- end of range interrupts ---
void Mirror::touchLeft(){
  stepsH = -END_OF_RANGE_H;
  PRINT("Touched left");
}

void Mirror::touchRight(){
  stepsH = END_OF_RANGE_H;
  PRINT("Touched right");
}

void Mirror::touchTop(){
  stepsV = END_OF_RANGE_TOP;
  PRINT("Touched top");
}

void Mirror::touchBottom(){
  stepsV = END_OF_RANGE_BOTTOM;
  PRINT("Touched bottom");
}

// MOVING
// sets all the motor pins to low
void Mirror::allLowH(){
  digitalWrite(COIL_1_A_H_PIN, LOW);
  digitalWrite(COIL_1_B_H_PIN, LOW);
  digitalWrite(COIL_2_A_H_PIN, LOW);
  digitalWrite(COIL_2_B_H_PIN, LOW);
}

void Mirror::allLowV(){
  digitalWrite(COIL_1_A_V_PIN, LOW);
  digitalWrite(COIL_1_B_V_PIN, LOW);
  digitalWrite(COIL_2_A_V_PIN, LOW);
  digitalWrite(COIL_2_B_V_PIN, LOW);
}

// sets appropriate polarisation of the motor (depending on the step counter)
void Mirror::makeStepH(){
  int step = ((stepsH % 4) + 4) % 4;
  digitalWrite(stepNumberToPinH[step][0], HIGH);
  digitalWrite(stepNumberToPinH[step][1], HIGH);
  delay(MOTOR_SIGNAL_HIGH_TIME);
  allLowH();
  delay(MOTOR_SIGNAL_GAP_TIME_H);
  blink(YELLOW);
}

void Mirror::makeStepV(){
  int step = ((stepsV % 4) + 4) % 4;
  digitalWrite(stepNumberToPinV[step][0], HIGH);
  digitalWrite(stepNumberToPinV[step][1], HIGH);
  delay(MOTOR_SIGNAL_HIGH_TIME);
  allLowV();
  delay(MOTOR_SIGNAL_GAP_TIME);
  blink(PINK);
}

// crucial piece of code - loop for moving the motors
void Mirror::repositionH(){
  PRINT2("stepsH: ", stepsH);
  PRINT2("desiredH: ", desiredH);
  while( true ){
    if(stepsH < desiredH && (digitalRead(RIGHT_END_PIN) == HIGH )){
      stepsH++;
      makeStepH();
      saveStepsH();
    }else if( stepsH > desiredH && ( digitalRead(LEFT_END_PIN) == HIGH )){
      stepsH--;
      makeStepH();
      saveStepsH();
    }else{
      // moves until it reaches the desired angle or the end of range
      break;
    }
  }
  PRINT("repositioned horizontal");
  PRINT2("stepsH: ", stepsH);
}

void Mirror::repositionV(){
  PRINT2("stepsV: ", stepsV);
  PRINT2("desiredV: ", desiredV);
  while( true ){
    if(stepsV < desiredV && digitalRead(TOP_END_PIN) == HIGH){
      stepsV++;
      makeStepV();
      saveStepsV();
    }else if( stepsV > desiredV && digitalRead(BOTTOM_END_PIN) == HIGH){
      stepsV--;
      makeStepV();
      saveStepsV();
    }else{
      break;
    }
  }
  PRINT("repositioned vertical");
  PRINT2("stepsV: ", stepsV);
}

//
// repositioning
//

// calculates the desired values of the step counters and moves the mirror
void Mirror::reposition(){
  setMode(Mirror::MOVING);
  PRINT("Moving...");

  desiredH = angleToStepsH(basicAngleH + offsetAngleH);
  if( desiredH > END_OF_RANGE_H){
    desiredH = END_OF_RANGE_H;
  }else if( desiredH < -END_OF_RANGE_H ){
    desiredH = -END_OF_RANGE_H;
  }

  desiredV = angleToStepsV(basicAngleV + offsetAngleV);
  if( desiredV > END_OF_RANGE_TOP){
    desiredV = END_OF_RANGE_TOP;
  }else if( desiredV < END_OF_RANGE_BOTTOM ){
    desiredV = END_OF_RANGE_BOTTOM;
  }

  // PRINT2("stepsH: ", stepsH);
  // PRINT2("stepsV: ", stepsV);
  // PRINT2("desired angle H: ", basicAngleH + offsetAngleH);
  // PRINT2("desired angle V: ", basicAngleV + offsetAngleV);
  // PRINT2("desiredH: ", desiredH);
  // PRINT2("desiredV: ", desiredV);

  repositionH();
  repositionV();
  if( desiredH == END_OF_RANGE_H || desiredV == END_OF_RANGE_BOTTOM  ){
    setMode(Mirror::END_OF_RANGE);
  } else {
    setMode(Mirror::DAY);
  }
}

// sets the mirror to left- and downwards for the number of steps equal the whole movement range in order to trigger end of range sensors and reset the counter values
void Mirror::repositionToReset(){
  setMode(Mirror::MOVING);
  PRINT("To reset...");
  stepsH = END_OF_RANGE_H;
  stepsV = END_OF_RANGE_TOP;
  desiredH = - END_OF_RANGE_H;
  desiredV = END_OF_RANGE_BOTTOM;
  repositionH();
  repositionV();
  setMode(Mirror::DAY);
}

// moves the mirror to the begining of the range (leftwards and downwards to almost touch the end of range sensors)
void Mirror::toBeginning(){
  setMode(Mirror::MOVING);
  PRINT("To begining...");
  desiredH = BEGINING_H;
  desiredV = BEGINING_V;
  repositionH();
  repositionV();
  setMode(Mirror::NIGHT);
}
