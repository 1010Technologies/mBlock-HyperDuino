#include <Arduino.h>
#include "MPR121_hf.h"

bool isPinTouched(int pin);

String hd_softSerialSendReceive(SoftwareSerial &sSerial, char *s);
void hd_softSerialSend(SoftwareSerial &sSerial, char *s);
String hd_softSerialReceive(SoftwareSerial &sSerial);

void hd_MotorSetup(int board);
void hd_MotorForward(int speedA, int speedB);
void hd_MotorBackward(int speedA, int speedB);
void hd_MotorLeft(int speed);
void hd_MotorRight(int speed);
void hd_MotorStop(void);
