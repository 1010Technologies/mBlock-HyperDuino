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

byte MOTOR_A1 = 0;
byte MOTOR_A2 = 0;
byte MOTOR_ASPEED = 0;
byte MOTOR_B1 = 0;
byte MOTOR_B2 = 0;
byte MOTOR_BSPEED = 0;
byte MOTOR_ENABLE = 0;

// HyperDuino board versions
// 100 = 1.0 (no motor pins)
// 240 = 2.4
// 350 = 3.5
// 400 = 4.0
void hd_MotorSetup(int board)
{
  if (board == 400)
  {
    MOTOR_A1 = 3;
    MOTOR_A2 = 4;
    MOTOR_ASPEED = 5;
    MOTOR_B1 = 7;
    MOTOR_B2 = 8;
    MOTOR_BSPEED = 6;
  }
  else if (board == 350)
  {
    MOTOR_A1 = 8;
    MOTOR_A2 = 9;
    MOTOR_ASPEED = 10;
    MOTOR_B1 = 12;
    MOTOR_B2 = 13;
    MOTOR_BSPEED = 11;
  }
  else if (board == 240)
  {
    MOTOR_ENABLE = 6;
    MOTOR_A1 = 7;
    MOTOR_A2 = 8;
    MOTOR_ASPEED = 9;
    MOTOR_B1 = 4;
    MOTOR_B2 = 5;
    MOTOR_BSPEED = 3;
  }
}

void hd_MotorMove(int speedA, int speedB)
{
  if (MOTOR_A1 != 0)
  {
    pinMode(MOTOR_B1,OUTPUT);
    pinMode(MOTOR_B2,OUTPUT);
    pinMode(MOTOR_A1,OUTPUT);
    pinMode(MOTOR_A2,OUTPUT);
    if (MOTOR_ENABLE != 0) digitalWrite(MOTOR_ENABLE,HIGH);
    analogWrite(MOTOR_ASPEED,abs(speedA));
    analogWrite(MOTOR_BSPEED,abs(speedB));
    digitalWrite(MOTOR_A1, speedA > 0 ? HIGH : LOW);
    digitalWrite(MOTOR_A2, speedA > 0 ? LOW : HIGH);
    digitalWrite(MOTOR_B1, speedB > 0 ? HIGH : LOW);
    digitalWrite(MOTOR_B2, speedB > 0 ? LOW : HIGH);
  }
}

void hd_MotorForward(int speedA, int speedB)
{
  hd_MotorMove(speedA, speedB);
}

void hd_MotorBackward(int speedA, int speedB)
{
  hd_MotorMove(-speedA, -speedB);
}

void hd_MotorLeft(int speed)
{
  hd_MotorMove(abs(speed), -abs(speed));
}

void hd_MotorRight(int speed)
{
  hd_MotorMove(-abs(speed), abs(speed));
}

void hd_MotorStop(void)
{
  if (MOTOR_A1 != 0)
  {
    if (MOTOR_ENABLE != 0) digitalWrite(MOTOR_ENABLE, LOW);
    analogWrite(MOTOR_BSPEED, 0);
    analogWrite(MOTOR_ASPEED, 0);
    digitalWrite(MOTOR_A1, LOW);
    digitalWrite(MOTOR_A2, LOW);
    digitalWrite(MOTOR_B1, LOW);
    digitalWrite(MOTOR_B2, LOW);
  }
}
