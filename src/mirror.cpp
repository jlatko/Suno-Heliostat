#include "mirror.h"
#include <Energia.h>



#define SETTUP_STEP_H 1
#define SETTUP_STEP_V 1

// how long is the delay in main loop
#define POLLING_DAY 1000
#define POLLING_END_OF_RANGE 1001
#define POLLING_MOVING 1002
#define POLLING_NIGHT 1003
#define POLLING_EDIT 100

// how long the coil is supplied with voltage and how long is the delay after the move
#define MOTOR_SIGNAL_HIGH_TIME 50
#define MOTOR_SIGNAL_GAP_TIME 200

#define ENABLE_1_H_PIN 5
#define ENABLE_2_H_PIN 25
#define COIL_11_A_H_PIN 10
#define COIL_11_B_H_PIN 30
#define COIL_12_A_H_PIN 8
#define COIL_12_B_H_PIN 29

#define ENABLE_1_V_PIN 26
#define ENABLE_2_V_PIN 26
#define COIL_11_A_V_PIN 26
#define COIL_11_B_V_PIN 26
#define COIL_12_A_V_PIN 26
#define COIL_12_B_V_PIN 26



const unsigned int Mirror::delayTimes[]= {
  POLLING_DAY,
  POLLING_END_OF_RANGE,
  POLLING_MOVING,
  POLLING_NIGHT,
  POLLING_EDIT
};
const unsigned int Mirror::stepNumberToPinH[]= {
  COIL_11_A_H_PIN,
  COIL_12_A_H_PIN,
  COIL_11_B_H_PIN,
  COIL_12_B_H_PIN
};
const unsigned int Mirror::stepNumberToPinV[]= {
  COIL_11_A_V_PIN,
  COIL_12_A_V_PIN,
  COIL_11_B_V_PIN,
  COIL_12_B_V_PIN
};

// INIT: sets the motor control pins to output
void Mirror::init(){
  pinMode(ENABLE_1_H_PIN, OUTPUT);
  pinMode(ENABLE_2_H_PIN, OUTPUT);
  pinMode(COIL_11_A_H_PIN, OUTPUT);
  pinMode(COIL_11_B_H_PIN, OUTPUT);
  pinMode(COIL_12_A_H_PIN, OUTPUT);
  pinMode(COIL_12_B_H_PIN, OUTPUT);

  pinMode(ENABLE_1_V_PIN, OUTPUT);
  pinMode(ENABLE_2_V_PIN, OUTPUT);
  pinMode(COIL_11_A_V_PIN, OUTPUT);
  pinMode(COIL_11_B_V_PIN, OUTPUT);
  pinMode(COIL_12_A_V_PIN, OUTPUT);
  pinMode(COIL_12_B_V_PIN, OUTPUT);



}
 void Mirror::calculateBasic(spa_data *spa){

 }

// Setup: changes the offset after a single click
// TODO: make sure what should be the max and min offset
void Mirror::setLeft(){
  // additional variable just for being super-cautious not to let the offset go out of range
  int tmpOffset = offsetH - SETTUP_STEP_H;
  if( tmpOffset >= -END_OF_RANGE_H ){
    offsetH = tmpOffset;
    Serial.print("LEFT: Offset H: " );
    Serial.println(offsetH);
    delay(20);
  }
}

void Mirror::setRight(){
  int tmpOffset = offsetH + SETTUP_STEP_H;
  if( tmpOffset <= END_OF_RANGE_H ){
    offsetH = tmpOffset;
    Serial.print("Right: Offset H: " );
    Serial.println(offsetH);
    delay(20);
  }
}

void Mirror::setUp(){
  int tmpOffset = offsetH + SETTUP_STEP_V;
  if( tmpOffset <= END_OF_RANGE_V ){
    offsetV = tmpOffset;
    Serial.print("UP: Offset V: " );
    Serial.println(offsetV);
    delay(20);
  }
}

void Mirror::setDown(){
  int tmpOffset = offsetH - SETTUP_STEP_V;
  if( tmpOffset >= -END_OF_RANGE_V ){
    offsetV = tmpOffset;
    Serial.print("DOWN: Offset V: " );
    Serial.println(offsetV);
    delay(20);
  }
}

// end of range interrupts
void Mirror::touchLeft(){
  stepsH = -END_OF_RANGE_H;
  touchedLeft = true;
  Serial.println("Touched left");
  delay(20);
}

void Mirror::touchRight(){
  stepsH = END_OF_RANGE_H;
  touchedRight = true;
  Serial.println("Touched right");
  delay(20);
}

void Mirror::touchTop(){
  stepsV = END_OF_RANGE_V;
  touchedTop = true;
  Serial.println("Touched top");
  delay(20);
}

void Mirror::touchBottom(){
  stepsV = -END_OF_RANGE_V;
  touchedBottom = true;
  Serial.println("Touched bottom");
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

bool Mirror::repositionH(){

// enable the motor driver
  digitalWrite(ENABLE_1_H_PIN, HIGH);
  digitalWrite(ENABLE_2_H_PIN, HIGH);

  while( true ){
    Serial.print("stepsH: ");
    Serial.println(stepsH);
    Serial.print("desiredH: ");
    Serial.println(desiredH);
    delay(20);
    // TODO: delete ( || true)
    if(stepsH < desiredH && (digitalRead(RIGHT_END_PIN) == HIGH )){
      stepsH++;
      makeStepH();
    }else if( stepsH > desiredH && ( digitalRead(LEFT_END_PIN) == HIGH )){
      stepsH--;
      makeStepH();
    }else{
      // moves until it reaches the desired angle or the end of range
      break;
    }
  }
  digitalWrite(ENABLE_1_H_PIN, LOW);
  digitalWrite(ENABLE_2_H_PIN, LOW);
  return true;
}

bool Mirror::repositionV(){
  digitalWrite(ENABLE_1_V_PIN, HIGH);
  digitalWrite(ENABLE_2_V_PIN, HIGH);
  while( true ){
    // ( || true) - temporary, only to check without the button
    if(stepsV < desiredV && (digitalRead(TOP_END_PIN) == HIGH || true )){
      stepsV++;
      makeStepV();
    }else if( stepsV > desiredV && (digitalRead(BOTTOM_END_PIN) == HIGH || true )){
      stepsV--;
      makeStepV();
    }else{
      break;
    }
  }
  digitalWrite(ENABLE_1_V_PIN, LOW);
  digitalWrite(ENABLE_2_V_PIN, LOW);
  return true;
}
