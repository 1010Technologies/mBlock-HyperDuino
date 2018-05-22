#include <stdint.h>

// The MP3 player is attached to these pins
// Set RX to -1 to prevent callbacks from the Serial MP3 to the Arduino,
// which can interfere with Firmata.
#define ARDUINO_RX      A1  //should connect to TX of the Serial MP3 Player module
#define ARDUINO_TX      A0  //connect to RX of the module

/************ Command byte **************************/
#define CMD_NEXT_SONG     0X01  // Play next song.
#define CMD_PREV_SONG     0X02  // Play previous song.
#define CMD_PLAY_W_INDEX  0X03
#define CMD_VOLUME_UP     0X04
#define CMD_VOLUME_DOWN   0X05
#define CMD_SET_VOLUME    0X06

#define CMD_SNG_CYCL_PLAY 0X08  // Single Cycle Play.
#define CMD_SEL_DEV       0X09
#define CMD_SLEEP_MODE    0X0A
#define CMD_WAKE_UP       0X0B
#define CMD_RESET         0X0C
#define CMD_PLAY          0X0D
#define CMD_PAUSE         0X0E
#define CMD_PLAY_FOLDER_FILE 0X0F

#define CMD_STOP_PLAY     0X16
#define CMD_FOLDER_CYCLE  0X17
#define CMD_SHUFFLE_PLAY  0x18 //
#define CMD_SET_SNGL_CYCL 0X19 // Set single cycle.

#define CMD_SET_DAC 0X1A
#define DAC_ON  0X00
#define DAC_OFF 0X01

#define CMD_PLAY_W_VOL    0X22
#define CMD_PLAYING_N     0x4C
#define CMD_QUERY_STATUS      0x42
#define CMD_QUERY_VOLUME      0x43
#define CMD_QUERY_FLDR_TRACKS 0x4e
#define CMD_QUERY_TOT_TRACKS  0x48
#define CMD_QUERY_FLDR_COUNT  0x4f

#define RESP_MEMCARD_INSERTED 0x3A
#define RESP_COMPLETED_PLAY   0x3D
#define RESP_ERROR            0x40
#define RESP_DATA_RECEIVED    0x41
#define RESP_PLAY_STATUS      0x42
#define RESP_FILE_COUNT       0x48
#define RESP_PLAYING          0x4C
#define RESP_FOLDER_FILE_COUNT 0x4E
#define RESP_FOLDER_COUNT     0x4F


/************ Options **************************/
#define DEV_TF            0X02

int MP3_IsAvailable(void);
int MP3_sendCommand(int8_t command, int16_t dat);

