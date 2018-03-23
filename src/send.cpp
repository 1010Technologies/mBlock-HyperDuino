#include "Send.h"

Send::Send() {
  mySerial = new SoftwareSerial(rx, tx);
  mySerial->begin(9600);

}

void Send::sendCommand(int8_t command, int16_t dat) {
  delay(20);
  Send_buf[0] = 0x7e; //starting byte
  Send_buf[1] = 0xff; //version
  Send_buf[2] = 0x06; //the number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command; //
  Send_buf[4] = 0x00;//0x00 = no feedback, 0x01 = feedback
  Send_buf[5] = (int8_t)(dat >> 8);//datah
  Send_buf[6] = (int8_t)(dat); //datal
  Send_buf[7] = 0xef; //ending byte
  for (uint8_t i = 0; i < 8; i++) //
  {
    mySerial->write(Send_buf[i]) ;
  }
}



int Send::decodeMP3Answer() {
  int decodedMP3Answer = 0;
  uint8_t i = 0;
  String mp3answer = "";

  while (mySerial->available() && (i < 10))
  {
    uint8_t b = mySerial->read();
    ansbuf[i] = b;
    i++;


    mp3answer += sbyte2hex(b);
  }

  //Serial.println(mp3answer);

  if (!ansbuf[6] == 0x00) {
      
      decodedMP3Answer =1;
  }

  return decodedMP3Answer;
}

String Send::sbyte2hex(uint8_t b)
{
  String shex;

  shex = "0X";

  if (b < 16) shex += "0";
  shex += String(b, HEX);
  shex += " ";
  return shex;
}