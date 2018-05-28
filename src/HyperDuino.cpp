#include <Arduino.h>
#include "HyperDuino.h"
#include "MPR121_hf.h"

#define IS_PIN_TOUCH(p)         ((p) >= 20 && (p) < 32)
#define PIN_TO_TOUCH(p)         ((p) - 20)

bool isPinTouched(int pin)
{
  if (pin >= 2 && pin <= 13 && Capsense.isCapsenseAvailable() &&
    Capsense.isCableConnected())
  {
    return Capsense.isPinTouched(pin);
  }
  return false;
}

// Do we have support for motors?
#ifdef MOTOR_A1

void hd_MotorSetup()
{
  pinMode(MOTOR_B1,OUTPUT);
  pinMode(MOTOR_B2,OUTPUT);
  pinMode(MOTOR_A1,OUTPUT);
  pinMode(MOTOR_A2,OUTPUT);
}

void hd_MotorForward(int speed)
{
#ifdef MOTOR_ENABLE
  digitalWrite(MOTOR_ENABLE,HIGH);
#endif
  analogWrite(MOTOR_ASPEED,speed);
  analogWrite(MOTOR_BSPEED,speed);
  digitalWrite(MOTOR_A1,HIGH);
  digitalWrite(MOTOR_A2,LOW);
  digitalWrite(MOTOR_B1,HIGH);
  digitalWrite(MOTOR_B2,LOW);
}

void hd_MotorBackward(int speed)
{
#ifdef MOTOR_ENABLE
  digitalWrite(MOTOR_ENABLE,HIGH);
#endif
  analogWrite(MOTOR_ASPEED,speed);
  analogWrite(MOTOR_BSPEED,speed);
  digitalWrite(MOTOR_A1,LOW);
  digitalWrite(MOTOR_A2,HIGH);
  digitalWrite(MOTOR_B1,LOW);
  digitalWrite(MOTOR_B2,HIGH);
}

void hd_MotorLeft(int speed)
{
#ifdef MOTOR_ENABLE
  digitalWrite(MOTOR_ENABLE,HIGH);
#endif
  analogWrite(MOTOR_ASPEED,speed);
  analogWrite(MOTOR_BSPEED,speed);
  digitalWrite(MOTOR_A1,HIGH);
  digitalWrite(MOTOR_A2,LOW);
  digitalWrite(MOTOR_B1,LOW);
  digitalWrite(MOTOR_B2,HIGH);
}

void hd_MotorRight(int speed)
{
#ifdef MOTOR_ENABLE
  digitalWrite(MOTOR_ENABLE,HIGH);
#endif
  analogWrite(MOTOR_ASPEED,speed);
  analogWrite(MOTOR_BSPEED,speed);
  digitalWrite(MOTOR_A1,LOW);
  digitalWrite(MOTOR_A2,HIGH);
  digitalWrite(MOTOR_B1,HIGH);
  digitalWrite(MOTOR_B2,LOW);
}

void hd_MotorStop(void)
{
#ifdef MOTOR_ENABLE
  digitalWrite(MOTOR_ENABLE,LOW);
#endif
  analogWrite(MOTOR_BSPEED,0);
  analogWrite(MOTOR_ASPEED,0);
  digitalWrite(MOTOR_A1,LOW);
  digitalWrite(MOTOR_A2,LOW);
  digitalWrite(MOTOR_B1,LOW);
  digitalWrite(MOTOR_B2,LOW);
}

#else

// No motor pins (HyperDuino board 1.0)
void hd_MotorSetup() {}
void hd_MotorForward(int speed) {}
void hd_MotorBackward(int speed) {}
void hd_MotorLeft(int speed) {}
void hd_MotorRight(int speed) {}
void hd_MotorStop(void) {}

#endif
