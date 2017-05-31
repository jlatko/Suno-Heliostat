#include "mirror.h"
#include "memory/memory.h"
#include <Energia.h>

const unsigned int Mirror::delayTimes[]= {
  POLLING_DAY,
  POLLING_END_OF_RANGE,
  POLLING_MOVING,
  POLLING_NIGHT,
  POLLING_EDIT
};
const unsigned int Mirror::stepNumberToPinH[]= {
  COIL_1_A_H_PIN,
  COIL_2_A_H_PIN,
  COIL_1_B_H_PIN,
  COIL_2_B_H_PIN
};
const unsigned int Mirror::stepNumberToPinV[]= {
  COIL_1_A_V_PIN,
  COIL_2_A_V_PIN,
  COIL_1_B_V_PIN,
  COIL_2_B_V_PIN
};

// INIT: sets the motor control pins to output
void Mirror::init(){
  // pinMode(ENABLE_H_PIN, OUTPUT);
  pinMode(COIL_1_A_H_PIN, OUTPUT);
  pinMode(COIL_1_B_H_PIN, OUTPUT);
  pinMode(COIL_2_A_H_PIN, OUTPUT);
  pinMode(COIL_2_B_H_PIN, OUTPUT);

  // pinMode(ENABLE_V_PIN, OUTPUT);
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
  offsetAngleH = (float)read(OFFSET_H_ADDRESS);
  offsetAngleV = (float)read(OFFSET_V_ADDRESS);
}

// Memory management - saving steps and settings to the memory
void Mirror::saveOffsets(){
  write(OFFSET_H_ADDRESS, offsetAngleH);
  write(OFFSET_V_ADDRESS, offsetAngleV);
}

void Mirror::saveStepsH(){
  write(STEPS_H_ADDRESS, stepsH);
}

void Mirror::saveStepsV(){
  write(STEPS_V_ADDRESS, stepsV);
}

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

int Mirror::angleToStepsH(float angle){
  return (int)(angle * STEPS_PER_DEGREE_H);
}

int Mirror::angleToStepsV(float angle){
  // TODO do the calibration (interpolation or approximation)
  return (int)(angle * STEPS_PER_DEGREE_H);
}

void Mirror::calculateBasic(spa_data *spa){
  basicAngleH = spa->azimuth_astro/2;
  basicAngleH = spa->elevation/2;
}

// === Setup: changes the offset after a single click  ===
// TODO: make sure what should be the max and min offset
void Mirror::setLeft(){
  // additional variable just for being super-cautious not to let the offset go out of range
  float tmpOffset = offsetAngleH - SETTUP_STEP_H;
  if( tmpOffset >= -END_OF_RANGE_H_ANGLE ){
    offsetAngleH = tmpOffset;
    Serial.print("LEFT: Offset H: " );
    PRINT(offsetAngleH);
    delay(20);
  }
}

void Mirror::setRight(){
  float tmpOffset = offsetAngleH + SETTUP_STEP_H;
  if( tmpOffset <= END_OF_RANGE_H_ANGLE ){
    offsetAngleH = tmpOffset;
    Serial.print("Right: Offset H: " );
    PRINT(offsetAngleH);
    delay(20);
  }
}

void Mirror::setUp(){
  int tmpOffset = offsetAngleV + SETTUP_STEP_V;
  if( tmpOffset <= END_OF_RANGE_TOP_ANGLE ){
    offsetAngleV = tmpOffset;
    Serial.print("UP: Offset V: " );
    PRINT(offsetAngleV);
    delay(20);
  }
}

void Mirror::setDown(){
  int tmpOffset = offsetAngleV - SETTUP_STEP_V;
  if( tmpOffset >= END_OF_RANGE_BOTTOM_ANGLE ){
    offsetAngleV = tmpOffset;
    Serial.print("DOWN: Offset V: " );
    PRINT(offsetAngleV);
    delay(20);
  }
}

// --- end of range interrupts ---
void Mirror::touchLeft(){
  stepsH = -END_OF_RANGE_H;
  // touchedLeft = true;
  PRINT("Touched left");
  delay(20);
}

void Mirror::touchRight(){
  stepsH = END_OF_RANGE_H;
  // touchedRight = true;
  PRINT("Touched right");
  delay(20);
}

void Mirror::touchTop(){
  stepsV = END_OF_RANGE_TOP;
  // touchedTop = true;
  PRINT("Touched top");
  delay(20);
}

void Mirror::touchBottom(){
  stepsV = END_OF_RANGE_BOTTOM;
  // touchedBottom = true;
  PRINT("Touched bottom");
  delay(20);
}

// MOVING

// sets one of the motor coil wires to high for a moment
void Mirror::makeStepH(){
  int step = ((stepsH % 4) + 4) % 4;
  digitalWrite(stepNumberToPinH[step], HIGH);
  delay(MOTOR_SIGNAL_HIGH_TIME);
  digitalWrite(stepNumberToPinH[step], LOW);
  delay(MOTOR_SIGNAL_GAP_TIME);
}

void Mirror::makeStepV(){
  int step = ((stepsV % 4) + 4) % 4;
  digitalWrite(stepNumberToPinV[step], HIGH);
  delay(MOTOR_SIGNAL_HIGH_TIME);
  digitalWrite(stepNumberToPinV[step], LOW);
  delay(MOTOR_SIGNAL_GAP_TIME);
}

// crucial piece of code - loop for moving the motors

void Mirror::repositionH(){
  while( true ){
    // TODO: delete ( || true)
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
}

void Mirror::repositionV(){
  while( true ){
    // ( || true) - temporary, only to check without the button
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
}

//
// repositioning
//
void Mirror::reposition(){
  setMode(Mirror::MOVING);
  PRINT("Moving...");
  delay(20);

  desiredH = angleToStepsH(basicAngleH + offsetAngleH);
  if( desiredH > END_OF_RANGE_H){
    desiredH = END_OF_RANGE_H;
  }else if( desiredH < -END_OF_RANGE_H ){
    desiredH = -END_OF_RANGE_H;
  }

  desiredV = angleToStepsV(basicAngleV + offsetAngleV);
  if( desiredH > END_OF_RANGE_H){
    desiredH = END_OF_RANGE_H;
  }else if( desiredH < -END_OF_RANGE_H ){
    desiredH = -END_OF_RANGE_H;
  }
  repositionH();
  repositionV();
  if( desiredH == END_OF_RANGE_H || desiredV == END_OF_RANGE_BOTTOM  ){
    setMode(Mirror::END_OF_RANGE);
  } else {
    setMode(Mirror::DAY);
  }
}

void Mirror::repositionToReset(){
  setMode(Mirror::MOVING);
  PRINT("To reset...");
  delay(20);
  stepsH = END_OF_RANGE_H;
  stepsV = END_OF_RANGE_TOP;
  desiredH = - END_OF_RANGE_H;
  desiredV = - END_OF_RANGE_BOTTOM;
  repositionH();
  repositionV();
  setMode(Mirror::DAY);
}

void Mirror::toBeginning(){
  setMode(Mirror::MOVING);
  PRINT("To begining...");
  delay(20);
  desiredH = BEGINING_H;
  desiredV = BEGINING_V;
  repositionH();
  repositionV();
  setMode(Mirror::NIGHT);
}
