#include <Arduino.h>

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


