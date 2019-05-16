#include <Arduino.h>
#include <SoftwareSerial.h>
#include "HyperDuino.h"
#include "MPR121_hf.h"

#define IS_PIN_TOUCH(p)         ((p) >= 20 && (p) < 32)
#define PIN_TO_TOUCH(p)         ((p) - 20)

int playFrequencyDelay[12] = {0};
int playFrequencyOffset[12] = {0};

bool isCapsenseInitialized = false;

#define BUFLEN 80
char buf[BUFLEN+1] = {0};

void hd_InitCapsense()
{
  if (!isCapsenseInitialized)
  {
    isCapsenseInitialized = true;
    Capsense.begin(CAPSENSE_ADDRESS, CAPSENSE_PINS);
    for (int _i = 2; _i <= 13; _i++)
    {
      pinMode(_i, OUTPUT);
    }
  }
}

bool hd_isPinTouched(int pin)
{
  if (Capsense.isCapsenseAvailable() && Capsense.isCableConnected())
  {
    if (pin >= 2 && pin <= 13)
    {
      return Capsense.isPinTouched(pin);
    }
  }
  return false;
}

int hd_getTouchPin()
{
  if (Capsense.isCapsenseAvailable() && Capsense.isCableConnected())
  {
    for (int pin=2; pin <= 13; pin++)
    {
      if (Capsense.isPinTouched(pin)) return pin;
    }
  }
  return 0;
}

void hd_lcdPrint(LiquidCrystal_I2C lcd, double value)
{
  if ((long) value == value)
  {
    lcd.print((long) value);
  }
  else
  {
    lcd.print(value);
  }
}

void hd_lcdPrint(LiquidCrystal_I2C lcd, const String value)
{
  lcd.print(value);
}

void hd_lcdPrint(LiquidCrystal_I2C lcd, const char value[])
{
  lcd.print(value);
}

void hd_lcdPrint(LiquidCrystal_I2C lcd, double value, int decimals)
{
  lcd.print(value, decimals);
}

String bluetoothResponse = "";

void hd_bluetoothCheckResponse(SoftwareSerial &sSerial) 
{
  delay(30);
  int i = 0;
  while (sSerial.available() && i < BUFLEN)
  {
    buf[i++] = sSerial.read();
  }
  if (i > 0)
  {
    buf[i] = '\0';
  }
}

bool hd_bluetoothHasResponse(SoftwareSerial &sSerial)
{
  hd_bluetoothCheckResponse(sSerial);
  return (strlen(buf) > 0);
}

String hd_bluetoothGetResponse()
{
  if (strlen(buf) > 0)
  {
    String result(buf);
    buf[0] = '\0';
    return result;
  }
  return "";
}

String hd_softSerialReceive(SoftwareSerial &sSerial)
{
  hd_bluetoothCheckResponse(sSerial);
  return hd_bluetoothGetResponse();
}

void hd_softSerialSend(SoftwareSerial &sSerial, char *s)
{
  sSerial.write(s);
  delay(30);
}

String hd_softSerialSendReceive(SoftwareSerial &sSerial, char *s)
{
  hd_softSerialSend(sSerial, s);
  return hd_softSerialReceive(sSerial);
}

// Length in bytes of Bitty messages (including the opcode)
byte bittyLen[9] = {0, 2, 3, 3, 2, 3, 2, 2, 7};
char bittyMsg[7] = {0};

bool hd_bittyControllerHasEvent(SoftwareSerial &sSerial)
{
  hd_bluetoothCheckResponse(sSerial);
  char c = buf[0];
  if (c > 0 && c <= 8 && strlen(buf) == bittyLen[c])
  {
    memcpy(bittyMsg, buf, bittyLen[c]);
    buf[0] = '\0';
    return true;
  }
  return false;
}

bool hd_bittyControllerEvent(String event)
{
  if (event == "Dpad")
    return bittyMsg[0] == 1;
  if (event == "Touchpad motion")
    return bittyMsg[0] == 2;
  if (event == "Touchpad control")
    return bittyMsg[0] == 3;
  if (event == "Other control")
    return bittyMsg[0] == 4;
  if (event == "Sampling Control")
    return bittyMsg[0] == 6;
  if (event == "Button")
    return bittyMsg[0] == 7;
  if (event == "Pin")
    return bittyMsg[0] == 8;
  return false;
}


bool hd_bittyControllerDpad(String dpad, String button, int pressed)
{
  if (bittyMsg[0] != 1)
    return false;
  int d = bittyMsg[1];
  if (d < 1 || d > 16)
    return false;
  if (pressed && d % 2 != 1)
    return false;
  if (dpad == "left") {
    if (bittyMsg[1] > 8)
      return false;
  } else if (bittyMsg[1] < 9) {
    return false;
  }
  d = ((d - 1) % 8)/2;
  switch (d)
  {
    case 0:
      return (button == "top");
    case 1:
      return (button == "bottom");
    case 2:
      return (button == "left");
    case 3:
      return (button == "right");
    }
}


int hd_bittyControllerMotionForward()
{
  if (bittyMsg[0] != 2)
    return 0;
  return (int) bittyMsg[1];
}


int hd_bittyControllerMotionRight()
{
  if (bittyMsg[0] != 2)
    return 0;
  return (int) bittyMsg[2];
}


bool hd_bittyControllerTouchpad(int touched)
{
  if (bittyMsg[0] != 3)
    return false;
  return bittyMsg[2] == touched;
}


bool hd_bittyControllerOther(int pressed)
{
  if (bittyMsg[0] != 4)
    return false;
  return bittyMsg[2] == pressed;
}


int hd_bittyControllerButton()
{
  if (bittyMsg[0] != 7)
    return -1;
  return (int) bittyMsg[1];
}


void hd_InitPlayFrequency()
{
  // Use Timer0 (millis) for hd_PlayFrequency
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
  }

  void hd_PlayFrequency(float freq, int pin)
  {
    float freq1 = freq;
    if (freq1 > 500)
      freq1 = 500;
    if (pin >= 2 && pin <= 13)
    {
      if (freq1 > 0)
      {
        pinMode(pin, OUTPUT);
        playFrequencyDelay[pin - 2] = (int)(500 / freq1 + 0.5);
        playFrequencyOffset[pin - 2] = millis() % playFrequencyDelay[pin - 2];
      } else {
      playFrequencyDelay[pin - 2] = 0;
    }
  }
}

void hd_StopPlayFrequency(int pin)
{
  if (pin >= 2 && pin <= 13)
  {
    playFrequencyDelay[pin - 2] = 0;
    digitalWrite(pin, LOW);
  }
}

// Used by hd_PlayFrequency
// Interrupt is called once a millisecond, to update the LEDs
SIGNAL(TIMER0_COMPA_vect)
{
  unsigned long t = millis();
  for (int pin = 2; pin <= 13; pin++)
  {
    int pd = playFrequencyDelay[pin - 2];
    if (pd != 0 && (t - playFrequencyOffset[pin - 2]) % pd == 0)
    {
      digitalWrite(pin, 1 - digitalRead(pin));
    }
  }
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

void hd_MotorStop(int motor)
{
  if (MOTOR_A1 != 0)
  {
    if (motor & 1)
    {
      analogWrite(MOTOR_ASPEED, 0);
      digitalWrite(MOTOR_A1, LOW);
      digitalWrite(MOTOR_A2, LOW);
    }
    if (motor & 2)
    {
      analogWrite(MOTOR_BSPEED, 0);
      digitalWrite(MOTOR_B1, LOW);
      digitalWrite(MOTOR_B2, LOW);
    }
  }
}
