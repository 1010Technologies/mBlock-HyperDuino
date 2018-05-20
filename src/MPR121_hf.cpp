#include <Arduino.h>

#include "I2C_hf.h"
#include "MPR121_hf.h"
#include "MPR121Registers_hf.h"


// What average filtered value do we need to drop below for 
// a cable to be considered connected
#define CABLE_CONNECTED_THRESHOLD 900

// How often to perform a detailed cable status check
#define CABLE_CHECK_CACHE_LIFE_MS 200

// Query touch status once every 10 milliseconds, cuts down on spurious accesses
#define TOUCH_STATUS_CACHE_LIFE_MS 10

// When the user plugs in or removes the cable we will need to recalibrate
// this provides a small delay to allow the user to remove their hand from proximity
#define CABLE_RECALIBRATION_DELAY_MS 3000

// How often to scan if the chip is reconnected, if it's been marked as disconnected
#define CAPSENSE_HEARTBEAT_PERIOD 1000

// How long must a pin be pressed for us to consider it "stuck"
#define TOUCH_STUCK_THRESHOLD_MS 15000

// Shared instance
MPR121_hf Capsense;

MPR121_hf::MPR121_hf() {
  // Mark the device as unitialized
  this->deviceAvailable = false;
  // When did we last check if the device was present
  this->lastDeviceHeartbeat = millis();
}

// Connect to a mpr cap sense device
void MPR121_hf::begin(uint8_t address, uint8_t numberActivePins) {
  // Set our device address
  this->address = address;

  // How many capsense inputs are we detecting
  this->pins = numberActivePins;

  // Haven't timed out yet
  this->deviceAvailable = true;

  // Reset touched stuck timer
  for(uint8_t i = 0; i < 12; ++i) {
    this->touchedForMillis[i] = 0; 
  }

  // Configure the internal pullups on the i2c pins
  pinMode(SDA, INPUT_PULLUP);
  pinMode(SCL, INPUT_PULLUP);

  // Make sure the bus is free
  if(this->verifyI2cBus()) {
    // Initialize i2c bus
    I2c.begin();
    this->reset();
    this->configureDevice();
  } else {
    // Mark the device as unavailable for further reads
    this->deviceAvailable = false;
  }
}

// Did we successfully connect to the device?
bool MPR121_hf::isCapsenseAvailable() {
  uint32_t currentMillis = millis();

  // This device has been marked as unavailable, perform a heart beat to confirm
  if(!this->deviceAvailable && currentMillis > (this->lastDeviceHeartbeat + CAPSENSE_HEARTBEAT_PERIOD)) {
    this->lastDeviceHeartbeat = currentMillis;
    // Attempt to read the pin status register (heartbeat)
    if(this->verifyI2cBus() && !I2c.read(this->address, MPR121_TS1, 0x01)) {
      (void)I2c.receive();
      // Successfully got a response, mark as active and perform a reset
      this->deviceAvailable = true;
      // Reset and bring active
      this->reset();
      this->configureDevice();
    }
  }
  return this->deviceAvailable;
}

// Assumes pin is in range 2...13
bool MPR121_hf::isPinTouched(uint8_t pin) {
  static uint16_t touchStatus = ((uint16_t)getRegister(MPR121_TS2) << 8) | (uint16_t)getRegister(MPR121_TS1);
  static uint32_t lastChecked = millis();
  // Cache the status
  if(millis() > (lastChecked + TOUCH_STATUS_CACHE_LIFE_MS)) {
    // Perform a refresh
    touchStatus = ((uint16_t)getRegister(MPR121_TS2) << 8) | (uint16_t)getRegister(MPR121_TS1);
    lastChecked = millis();

    // Scan the pins to make sure nothing is considered stuck?
    for(uint8_t scanForStuckPin = 0; scanForStuckPin < 12; ++scanForStuckPin) {
      // Invert pins in software T0 := T11
      if(touchStatus & (1<<(11 - scanForStuckPin))) {
        // Pin is touched increment counter
        this->touchedForMillis[scanForStuckPin] += TOUCH_STATUS_CACHE_LIFE_MS;
        // Are we considered stuck?
        if(this->touchedForMillis[scanForStuckPin] > TOUCH_STUCK_THRESHOLD_MS) {
            this->touchedForMillis[scanForStuckPin] = 0;
            // Small delay to allow moving appendages away if this is intentional
            delay(CABLE_RECALIBRATION_DELAY_MS);
            // issue a soft reset, to allow recalibration
            this->reset();
            this->configureDevice();
        }
      } else {
        this->touchedForMillis[scanForStuckPin] = 0;
      }
    }
  }
  // Invert pins in software T2 := T13
  return touchStatus & (uint16_t)(1 << (13 - pin));
}

bool MPR121_hf::isCableConnected() {
  static uint16_t meanFilteredValue = getMeanFilteredValue();
  static uint32_t lastChecked = millis();

  static bool cableConnected = false;
  static bool pendingRecalibration = false;
  static uint32_t lastCableStateChange = millis();

  // No device connected
  if(!this->deviceAvailable) return false;

  // Cache the status
  if(millis() > (lastChecked + CABLE_CHECK_CACHE_LIFE_MS)) {
    // Perform a refresh
    uint16_t value = getMeanFilteredValue();
    // Not in a calibration phase
    if(value != 0) {
      meanFilteredValue = value;
      lastChecked = millis();
    }
  }

  // Cable state has changed, we will need to recalibrate
  if(cableConnected != (meanFilteredValue < CABLE_CONNECTED_THRESHOLD)) {
    pendingRecalibration = true;
    lastCableStateChange = millis();
  }

  // Do a recalibration if needed
  if(pendingRecalibration && millis() > (lastCableStateChange + CABLE_RECALIBRATION_DELAY_MS)) {
    pendingRecalibration = false;
    // issue a soft reset, to allow recalibration
    this->reset();
    this->configureDevice();
  }

  // Are we in range of threshold?
  cableConnected = meanFilteredValue < CABLE_CONNECTED_THRESHOLD;
  return cableConnected;
}

uint16_t MPR121_hf::getMeanFilteredValue() {
  uint32_t runningTally = 0;
  for(uint8_t i = 0; i < CAPSENSE_CABLE_DETECTION_PINS; ++i) {
    runningTally += ((uint16_t)getRegister(MPR121_E0FDL + (i<<1)) | ((uint16_t)getRegister(MPR121_E0FDL + (i<<1) + 1) << 8));
  }
  return runningTally / CAPSENSE_CABLE_DETECTION_PINS;
}

// Default configuration
void MPR121_hf::configureDevice() {
  // Stop the device
  stop();

  // Default configuration copied from bareconductive driver

  // Input filter for rising state
  setRegister(MPR121_MHDR, 0x01);
  setRegister(MPR121_NHDR, 0x01);
  setRegister(MPR121_NCLR, 0x10);
  setRegister(MPR121_FDLR, 0x20);

  // Input filter for falling state
  setRegister(MPR121_MHDF, 0x01);
  setRegister(MPR121_NHDF, 0x01);
  setRegister(MPR121_NCLF, 0x10);
  setRegister(MPR121_FDLF, 0x20);

  // Input filter for touched state
  setRegister(MPR121_NHDT, 0x01);
  setRegister(MPR121_NCLT, 0x10);
  setRegister(MPR121_FDLT, 0xFF);

  // Unused proximity sensor filter
  setRegister(MPR121_MHDPROXR, 0x0F);
  setRegister(MPR121_NHDPROXR, 0x0F);
  setRegister(MPR121_NCLPROXR, 0x00);
  setRegister(MPR121_FDLPROXR, 0x00);
  setRegister(MPR121_MHDPROXF, 0x01);
  setRegister(MPR121_NHDPROXF, 0x01);
  setRegister(MPR121_NCLPROXF, 0xFF);
  setRegister(MPR121_FDLPROXF, 0xFF);
  setRegister(MPR121_NHDPROXT, 0x00);
  setRegister(MPR121_NCLPROXT, 0x00);
  setRegister(MPR121_FDLPROXT, 0x00);

  // Debounce configuration (used primarily for interrupts)
  setRegister(MPR121_DTR, 0x11);

  // Electrode clock frequency etc
  setRegister(MPR121_AFE1, 0xFF);
  setRegister(MPR121_AFE2, 0x30);

  // Enable autoconfiguration / calibration
  setRegister(MPR121_ACCR0, 0x00);
  setRegister(MPR121_ACCR1, 0x00);

  // Tuning parameters for the autocalibration algorithm
  setRegister(MPR121_USL, 0x00);
  setRegister(MPR121_LSL, 0x00);
  setRegister(MPR121_TL, 0x00);

  // Default sensitivity thresholds
  setTouchThreshold(60);
  setReleaseThreshold(20);

  // Restart capture
  start();
}

void MPR121_hf::setTouchThreshold(uint8_t value) {
  for(uint8_t i = 0; i < 13; ++i){
    setRegister(MPR121_E0TTH + (i<<1), value);
  }
}

void MPR121_hf::setReleaseThreshold(uint8_t value) {
  for(uint8_t i = 0; i < 13; ++i){
    setRegister(MPR121_E0RTH + (i<<1), value);
  }
}

void MPR121_hf::setRegister(uint8_t registr, uint8_t value) {
  if(this->deviceAvailable) {
    if(I2c.write(this->address, registr, value) != 0) {
      // An error occurred, mark the device as unavailable
      this->deviceAvailable = false;
    }
  }
}

uint8_t MPR121_hf::getRegister(uint8_t registr) {
  // Request a single byte from the remote device
  if(this->deviceAvailable) {
    if(I2c.read(this->address, registr, 0x01) != 0) {
      // An error occured, mark the device as unavailable
      this->deviceAvailable = false;
      // An error occurred
      return -1;
    }
    return I2c.receive();
  } else {
    // No device available
    return -1;
  }
}

void MPR121_hf::stop() {
  // Turn off all electrodes to stop
  setRegister(MPR121_ECR, 0xC0);
}

void MPR121_hf::start() {
  // Listen on all electrodes
  setRegister(MPR121_ECR, 0xCC);
}

void MPR121_hf::reset() {
  // Issue device reset
  setRegister(MPR121_SRST, 0x63);
}

// Double check the bus is idling appropriately
// A4 (SDA) and A5 (SCL) should be pulled high, ready for i2c data
// if A5 is low, due to clock stretching the AVR might
// lock up indefinitely
bool MPR121_hf::verifyI2cBus() {
  // Vih for ttl is approximately ~2.0 volts
  float threshold = (2.0f/5.0f) * 1024;
  if ((SDA == A4) && (SCL == A5)) {
    // Arduino Uno and compatible boards
    return analogRead(SDA) > threshold && analogRead(SCL) > threshold;
  } else {
    // If the I2C pins are not ADC, then we'll have to trust a digital read
    return digitalRead(SDA) && digitalRead(SCL);
  }
}

// Used to bypass buffering, when using something like a straight through sketch
uint16_t MPR121_hf::getTouchBitArray() {
  return ((uint16_t)getRegister(MPR121_TS2) << 8) | (uint16_t)getRegister(MPR121_TS1);
}

