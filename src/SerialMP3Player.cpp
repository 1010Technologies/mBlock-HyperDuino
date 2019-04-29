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

static struct MP3_STATE
{
  boolean isPlaying;
  boolean repeat;
  int fileCount;
  int folderCount;
  int track;
  int folder;
} mp3state = {0};

/********************************************************************************/
/*Function: sbyte2hex. Returns a byte data in HEX format.                 */
/*Parameter:- uint8_t b. Byte to convert to HEX.                                */
/*Return: String                                                                */
//
static String sbyte2hex(uint8_t b)
{
  String shex = "";
  if (b < 16)
    shex += "0";
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
static int decodeMP3Answer()
{
  int decodedMP3Answer;
  uint8_t ansbuf[10] = {0}; // Buffer for the answers.    // BETTER LOCALLY

  sanswer(ansbuf);

  //  Serial.begin(9600);
  //  Serial.print("Recv ");
  //  for (uint8_t i = 0; i < 10; i++) Serial.print(sbyte2hex(ansbuf[i]));
  //  Serial.println();

  switch (ansbuf[3])
  {
  case RESP_COMPLETED_PLAY:
    if (!mp3state.repeat) {
      mp3state.isPlaying = false;
    }
    break;
  case RESP_FOLDER_FILE_COUNT:
    mp3state.fileCount = ansbuf[6];
    break;
  case RESP_FOLDER_COUNT:
    mp3state.folderCount = ansbuf[6];
    break;
  case RESP_ERROR:
    // Bad track number?
    if (ansbuf[6] == 6) {
      mp3state.isPlaying = false;
    }
    break;
  default:
    break;
  }
}

static boolean MP3_CheckState(void)
{
  if (mp3.isListening() && mp3.available())
  {
    decodeMP3Answer();
    return true;
  }
  return false;
}

static void MP3_Initialize(void)
{
  static boolean initialized = false;

  if (!initialized)
  {
    initialized = true;
    digitalWrite(ARDUINO_TX, HIGH);
    pinMode(ARDUINO_TX, OUTPUT);
    pinMode(ARDUINO_RX, INPUT);
    digitalWrite(ARDUINO_RX, HIGH); // pullup for normal logic!
    mp3.begin(9600);
    MP3_SendCommand(CMD_SEL_DEV, DEV_TF);
  }
}

/********************************************************************************/
/*Function: Send command to the MP3                                         */
/*Parameter:-int8_t command                                                     */
/*Parameter:-int16_ dat  parameter for the command                              */

int MP3_SendCommand(int8_t command, int16_t dat)
{
  int8_t Send_buf[8] = {0}; // Buffer for Send commands.  // BETTER LOCALLY
  uint8_t i = 0;
  uint8_t wantFeedback;

  MP3_Initialize();
  if (!mp3.isListening())
  {
    mp3.listen();
  }

  switch (command)
  {
  case CMD_NEXT_SONG:
    mp3state.isPlaying = true;
    mp3state.repeat = false;
    mp3state.track++;
    command = CMD_PLAY_FOLDER_FILE;
    dat = 256*mp3state.folder + mp3state.track;
    break;
  case CMD_PREV_SONG:
    if (mp3state.track > 1) {
      mp3state.isPlaying = true;
      mp3state.repeat = false;
      mp3state.track--;
      command = CMD_PLAY_FOLDER_FILE;
      dat = 256 * mp3state.folder + mp3state.track;
    }
    break;
  case CMD_STOP_PLAY:
    mp3state.isPlaying = false;
    break;
  case CMD_SET_SNGL_CYCL:
    if (dat == 0 && mp3state.isPlaying) {
      mp3state.repeat = true;
    } else {
      mp3state.repeat = false;
    }
    break;
  case CMD_SEL_DEV:
  case CMD_PLAYING_N:
  case CMD_QUERY_STATUS:
  case CMD_QUERY_VOLUME:
  case CMD_QUERY_FLDR_TRACKS:
  case CMD_QUERY_TOT_TRACKS:
  case CMD_QUERY_FLDR_COUNT:
    break;
  case CMD_PLAY_FOLDER_FILE:
    mp3state.isPlaying = true;
    mp3state.repeat = false;
    mp3state.folder = (int8_t)(dat >> 8); //datah
    mp3state.track = (int8_t)(dat);       //datal
    break;
  default:
    break;
  }

  Send_buf[0] = 0x7e;               //
  Send_buf[1] = 0xff;               //
  Send_buf[2] = 0x06;               // Len
  Send_buf[3] = command;            //
  Send_buf[4] = 1;                  // 0x00 NO, 0x01 feedback
  Send_buf[5] = (int8_t)(dat >> 8); //datah
  Send_buf[6] = (int8_t)(dat);      //datal
  Send_buf[7] = 0xef;               //
  for (i = 0; i < 8; i++)
  {
    mp3.write(Send_buf[i]);
  }

  delay(300);
  MP3_CheckState();
}

boolean MP3_IsPlaying(void)
{
  MP3_CheckState();
  return mp3state.isPlaying;
}

