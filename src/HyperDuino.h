#include <Arduino.h>
#include "MPR121_hf.h"

// Ideally, we would only include these within the blocks that use them.
// However, mBlock appears to have a bug where it will sometimes erase one
// block's #include lines if another block has a different #include.
// So, to avoid this, just always include everything. This is messy but won't
// cause code bloat because the Arduino compiler strips out unused functions.
#include "LiquidCrystal_I2C_hf.h"
#include "SerialMP3Player.h"

bool isPinTouched(int pin);

String hd_softSerialSendReceive(SoftwareSerial &sSerial, char *s);
void hd_softSerialSend(SoftwareSerial &sSerial, char *s);
String hd_softSerialReceive(SoftwareSerial &sSerial);

void hd_PlayFrequency(float freq, int pin);
void hd_StopPlayFrequency(int pin);

void hd_MotorSetup(int board);
void hd_MotorForward(int speedA, int speedB);
void hd_MotorBackward(int speedA, int speedB);
void hd_MotorLeft(int speed);
void hd_MotorRight(int speed);
void hd_MotorStop(void);
