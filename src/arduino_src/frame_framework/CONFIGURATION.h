//PINS
//ARDUINO PINS
#define WS2812_PIN 5
#define SD_CARD_CS_PIN 4
#define FRAMERATE_OUTPUT_PIN 13 //IF ENABLE_FRAMERATE_OUTPUT defined to check the framerate via oszi

#define SERIAL_BAUD_RATE 115200
//DEFINE YOUR MATRIX SIZE HERE
#define VISIBLE_MATRIX_WITH 8 //WIDTH
#define VISIBLE_MATRIX_HEIGHT 8 //HEIGHT
//YOU CAN DEFINE HERE YOUR MATRIX SETUP
#define MATRIX_ORIGIN_LEFT_DOWN // where is your origin corner MATRIX_ORIGIN_LEFT_UP MATRIX_ORIGIN_LEFT_DOWN MATRIX_ORIGIN_RIGHT_UP MATRIX_ORIGIN_RIGHT_DOWN
#define MATRIX_MODE_ROW // is your setup MATRIX_MODE_ROW or MATRIX_MODE_COLLUM if not you can enable the USE_LED_LOOKUP feature below
#define COUNT_OF_LAYERS 2 //SET LAYER COUNT HERE, IF YOU WANT TO FADE BETWEEN ANIMATIONS 2,4,6,8,.. LAYERS ARE REQUIRED WATCH YOUR RAM
#define LED_COLOR_MODE_RGB // DEFAULT RGB RBG BGR BRG GBR GRB
//#define ENABLE_OUTPUT_INTENSE //enables a additional intense multiplier to the output layer
//#define ENABLE_LAYER_MOVEMENT
#define RAM_SIZE 32768 //DEFINE HERE YOUR CONNECTED SRAM SIZE

//HELPER DEFINES
#define ENABLE_FRAMERATE_OUTPUT
#define _SER_DEBUG_ //ENABE SERIAL DEBUGGIN

//SD DEFINES
#define SD_LOAD_ONLY_RIGHT_SIZE_ANIMATIONS

//YOU CAN HERE CREATE A LOOKUPTABLE IF YOUR LEDS NOT IN ROW OR COLLUM
//you can change the led id here if your setup is not collum/row based
//in the defualt setting the virtual led id of led 0 is 0
//if your real first led has for e.g. the number 10 then you can change it here
//you can use excel sheet 'led_lookup_creator'
//#define USE_LED_LOOKUP
//#define USE_LED_LOOKUP_PROGMEM



#define DISABLE_FLASH_STRINGS //ARE THIS SKETCH IS RUNNING ON A EPS ? F()...


/*
FILE DEFINES
*/

#define SD_FILE_DEF_COLOR_TABLE "COLORS.csv"



//CHARAKTER SETTINGS
#define SD_FILE_FRAME_DATA_SEPERATOR ','
#define SD_FILE_FRAME_HEADER_SEPERATOR '_'
#define SD_FILE_NEW_LINE_CHAR '\n'
#define SD_FILE_CSV_SEPERATION_CHAR ';'


//RAM SETTINGS
#define RAM_COLOR_TABLE_OFFSET 0
#define RAM_FRAME_TABLE_OFFSET (VISIBLE_MATRIX_WITH*VISIBLE_MATRIX_HEIGHT*sizeof(byte))



//HELPER DEFINES
#ifdef DEBUG
#define _SER_DEBUG_
#endif // DEBUG

//IF YOUR ARE USING AN ESP WE CANT USE F()
#ifdef DISABLE_FLASH_STRINGS
#define F(param) param
#endif


//IS OPEN WITH VISUAL STUDIO ? WE DONT NEED THIS
#ifndef _MSC_VER
//#define byte unsigned char;
//#define nullptr NULL
#endif