#include <Arduino.h>
#include "MPR121_hf.h"

bool isPinTouched(int pin);

// HyperDuino board versions
// 100 = 1.0 (no motor pins)
// 240 = 2.4
// 350 = 3.5
// 400 = 4.0
#ifndef BOARD
#define BOARD 400
#endif

#if BOARD == 400
  #define MOTOR_A1 3
  #define MOTOR_A2 4
  #define MOTOR_ASPEED 5
  #define MOTOR_B1 7
  #define MOTOR_B2 8
  #define MOTOR_BSPEED 6
#elif BOARD == 350
  #define MOTOR_A1 8
  #define MOTOR_A2 9
  #define MOTOR_ASPEED 10
  #define MOTOR_B1 12
  #define MOTOR_B2 13
  #define MOTOR_BSPEED 11
#elif BOARD == 240
  #define MOTOR_ENABLE 6
  #define MOTOR_A1 7
  #define MOTOR_A2 8
  #define MOTOR_ASPEED 9
  #define MOTOR_B1 4
  #define MOTOR_B2 5
  #define MOTOR_BSPEED 3
#else
#endif

void hd_MotorSetup();
void hd_MotorForward(int speed);
void hd_MotorBackward(int speed);
void hd_MotorLeft(int speed);
void hd_MotorRight(int speed);
void hd_MotorStop(void);
