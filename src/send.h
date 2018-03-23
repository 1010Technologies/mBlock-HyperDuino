#ifndef send_h

#include <Arduino.h>
#include <SoftwareSerial.h>

//#define ARDUINO_RX 6 
//#define ARDUINO_TX 5
const byte rx=6;
const byte tx=5;

//String mp3Answer;



///@brief Class for DemoClass
class Send
{
  public:
    Send();
    void sendCommand(int8_t command, int16_t dat);
    int decodeMP3Answer();
    String sbyte2hex(uint8_t b);
    int8_t Send_buf[8] = {0};
    uint8_t ansbuf[10] = {0};

    SoftwareSerial * mySerial;

};

#endif
