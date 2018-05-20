#ifndef MPR121_H
#define MPR121_H

#include <stdint.h>

/* The address of our mpr121 capacitive sense chip */
#ifndef CAPSENSE_ADDRESS
#define CAPSENSE_ADDRESS 0x5A
#define CAPSENSE_CABLE_DETECTION_PINS 6 /* How many pins to use to detect a plugged in cable */
#endif

class MPR121_hf {
  private:
    uint8_t address;
    uint8_t pins;
    bool deviceAvailable;
    uint32_t lastDeviceHeartbeat;
    // Keep track of how long pins are "pressed" to make sure we don't get a bad autocalibrate
    uint16_t touchedForMillis[12];
    
    void configureDevice();
    uint16_t getMeanFilteredValue();
    void setTouchThreshold(uint8_t value);
    void setReleaseThreshold(uint8_t value);
    void setRegister(uint8_t registr, uint8_t value);
    uint8_t getRegister(uint8_t registr);
    void stop();
    void start();
    void reset();
    bool verifyI2cBus();
   
  public:
    MPR121_hf();
    void begin(uint8_t address, uint8_t numberActivePins);
    bool isCableConnected();
    bool isPinTouched(uint8_t pin);
    bool isCapsenseAvailable();
    uint16_t getTouchBitArray();
};

extern MPR121_hf Capsense;

#endif
