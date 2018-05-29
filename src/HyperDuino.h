#include <Arduino.h>
#include "MPR121_hf.h"

bool isPinTouched(int pin);

void hd_MotorSetup(int board);
void hd_MotorForward(int speedA, int speedB);
void hd_MotorBackward(int speedA, int speedB);
void hd_MotorLeft(int speed);
void hd_MotorRight(int speed);
void hd_MotorStop(void);
