/***********************************************************/
// Based upon the demo code for the Serial MP3 Player Catalex (YX5300 chip):
// https://github.com/cefaloide/ArduinoSerialMP3Player
// Hardware: Serial MP3 Player *1
// http://www.dx.com/p/uart-control-serial-mp3-music-player-module-for-arduino-avr-arm-pic-blue-silver-342439#.VfHyobPh5z0
//

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "SerialMP3Player.h"

SoftwareSerial mp3(ARDUINO_RX, ARDUINO_TX);
//#define mp3 Serial3    // Connect the MP3 Serial Player to the Arduino MEGA Serial3 (14 TX3 -> RX, 15 RX3 -> TX)


/********************************************************************************/
/*Function: sbyte2hex. Returns a byte data in HEX format.                 */
/*Parameter:- uint8_t b. Byte to convert to HEX.                                */
/*Return: String                                                                */
//
static String sbyte2hex(uint8_t b)
{
  String shex = "";
  if (b < 16) shex += "0";
  shex += String(b, HEX);
  shex += " ";
  return shex;
}

/********************************************************************************/
/*Function: sanswer. Returns a String answer from mp3 UART module.          */
/*Parameter:- uint8_t b. void.                                                  */
/*Return: String. If the answer is well formated answer.                        */
//
static void sanswer(uint8_t ansbuf[10])
{
  uint8_t i = 0;

  // Get only 10 Bytes
  while (mp3.available() && (i < 10))
  {
    uint8_t b = mp3.read();
    ansbuf[i] = b;
    i++;
  }
}


/********************************************************************************/
/*Function decodeMP3Answer: Decode MP3 answer.                                  */
/*Parameter:-void                                                               */
/*Return: The                                                  */
//
static int decodeMP3Answer() {
  int decodedMP3Answer;
  uint8_t ansbuf[10] = {0}; // Buffer for the answers.    // BETTER LOCALLY

  sanswer(ansbuf);

//  Serial.print("Recv ");
//  for (uint8_t i = 0; i < 10; i++) Serial.print(sbyte2hex(ansbuf[i]));
//  Serial.println();

  switch (ansbuf[3])
  {
    case RESP_PLAY_STATUS:
    case RESP_FILE_COUNT:
    case RESP_PLAYING:
    case RESP_FOLDER_FILE_COUNT:
    case RESP_FOLDER_COUNT:
      decodedMP3Answer = ansbuf[6];
      break;
    default:
      decodedMP3Answer = ansbuf[3];
      break;
  }

  return decodedMP3Answer;
}



/********************************************************************************/
/*Function: Send command to the MP3                                         */
/*Parameter:-int8_t command                                                     */
/*Parameter:-int16_ dat  parameter for the command                              */

int MP3_sendCommand(int8_t command, int16_t dat)
{
  int8_t Send_buf[8] = {0}; // Buffer for Send commands.  // BETTER LOCALLY
  uint8_t i = 0;
  uint8_t wantFeedback;

  delay(20);

  switch (command)
  {
    case CMD_SEL_DEV:
    case CMD_PLAYING_N:
    case CMD_QUERY_STATUS:
    case CMD_QUERY_VOLUME:
    case CMD_QUERY_FLDR_TRACKS:
    case CMD_QUERY_TOT_TRACKS:
    case CMD_QUERY_FLDR_COUNT:
      wantFeedback = 1;
      mp3.listen();
      break;
    default:
      wantFeedback = 0;
      break;
  }

  Send_buf[0] = 0x7e;   //
  Send_buf[1] = 0xff;   //
  Send_buf[2] = 0x06;   // Len
  Send_buf[3] = command;//
  Send_buf[4] = wantFeedback;   // 0x00 NO, 0x01 feedback
  Send_buf[5] = (int8_t)(dat >> 8);  //datah
  Send_buf[6] = (int8_t)(dat);       //datal
  Send_buf[7] = 0xef;   //
  for (i = 0; i < 8; i++) {
    mp3.write(Send_buf[i]);
  }
  
//  Serial.print("Send ");
  for (i = 0; i < 8; i++) {
//    Serial.print(sbyte2hex(Send_buf[i]));
  }
//  Serial.println();

  delay(20);

  int result = 1;

  if (wantFeedback)
  {
    while (i < 10)
    {
      if (mp3.available())
      {
        result = decodeMP3Answer();
        break;
      }
      delay(100);
      i++;
    }
    mp3.stopListening();
  }

  return result;
}



int MP3_IsAvailable(void)
{
  static byte initialized = 0;
  static byte mp3available = 0;
  int answer = 0;
  int i = 0;

  if (!initialized || mp3available)
  {
    digitalWrite(ARDUINO_TX, HIGH);
    pinMode(ARDUINO_TX, OUTPUT);
    pinMode(ARDUINO_RX, INPUT);
    digitalWrite(ARDUINO_RX, HIGH);  // pullup for normal logic!
  }

  if (!initialized)
  {
    initialized = 1;
    mp3.begin(9600);
    mp3available = MP3_sendCommand(CMD_SEL_DEV, DEV_TF);
//      MP3_sendCommand(CMD_SET_VOLUME, 30);  // volume 0-30
    // If the MP3 Player isn't plugged in, restore our analog pins.
    if (!mp3available)
    {
      mp3.end();
      digitalWrite(ARDUINO_TX, LOW);
      pinMode(ARDUINO_TX, INPUT);
      digitalWrite(ARDUINO_RX, LOW);
      pinMode(ARDUINO_RX, INPUT);
    }
  }

  return mp3available;
}





