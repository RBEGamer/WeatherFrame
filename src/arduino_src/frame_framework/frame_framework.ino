//WeatherFrame Framework C 2016 Marcel Ochsendorf github.com/RBEGamer marcel.ochsendorf@gmail.com marcelochsendorf.com
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <SPI.h>
#include <SD.h>
//SYSTEM DEFINES
#define _PLATTFORM_ESP_ //ARE THIS SKETCH IS RUNNING ON A EPS ? 
#ifdef _PLATTFORM_ESP_
#define F(param) param
#endif
//#define byte unsigned char;



//PINS
//ARDUINO PINS
#define WS2812_PIN 5
#define SD_CARD_CS_PIN 4
#define FRAMERATE_OUTPUT_PIN 13 //IF ENABLE_FRAMERATE_OUTPUT defined to check the framerate via oszi

//DEFINE YOUR MATRIX SIZE HERE
#define VISIBLE_MATRIX_WITH 8 //WIDTH
#define VISIBLE_MATRIX_HEIGHT 8 //HEIGHT
//YOU CAN DEFINE HERE YOUR MATRIX SETUP
#define MATRIX_ORIGIN_LEFT_UP // where is your origin corner MATRIX_ORIGIN_LEFT_UP MATRIX_ORIGIN_LEFT_DOWN MATRIX_ORIGIN_RIGHT_UP MATRIX_ORIGIN_RIGHT_DOWN
#define MATRIX_MODE_ROW // is your setup MATRIX_MODE_ROW or MATRIX_MODE_COLLUM if not you can enable the USE_LED_LOOKUP feature below
#define COUNT_OF_LAYERS 2 //SET LAYER COUNT HERE, IF YOU WANT TO FADE BETWEEN ANIMATIONS 2,4,6,8,.. LAYERS ARE REQUIRED WATCH YOUR RAM
#define LED_COLOR_MODE_RGB // DEFAULT RGB RBG BGR BRG GBR GRB
#define ENABLE_OUTPUT_INTENSE //enables a additional intense multiplier to the output layer
//#define ENABLE_LAYER_MOVEMENT
#define RAM_SIZE 2048 //DEFINE HERE YOUR CONNECTED SRAM SIZE

//HELPER DEFINES
#define ENABLE_FRAMERATE_OUTPUT
#define _SER_DEBUG_ //ENABE SERIAL DEBUGGIN



//CONST DEFINES
#ifdef ENABLE_LAYER_MOVEMENT
#define MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR 3 // must be 3 or 1 if you choose one you cant move outside the visible area
#else
#define MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR 1 // must be 3 or 1 if you choose one you cant move outside the visible area
#endif // ENABLE_LAYER_MOVEMENT
#define LED_COUNT (VISIBLE_MATRIX_WITH*VISIBLE_MATRIX_HEIGHT)
#define TOTAL_MATRIX_WIDHT (MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR * VISIBLE_MATRIX_WITH)
#define TOTAL_MATRIX_HEIGHT (MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR * VISIBLE_MATRIX_HEIGHT)
#define TOTAL_MATRIX_CELL_COUNT (VISIBLE_MATRIX_WITH*VISIBLE_MATRIX_HEIGHT)
#define FRAME_SIZE_WIDTH VISIBLE_MATRIX_WITH
#define FRAME_SIZE_HEIGHT VISIBLE_MATRIX_HEIGHT
//ANCHOR IST UP-LEFT IN THE VISIBLE AREA  HERE YOU CAN ADD A OFFSET
#if MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR == 3
#define ANCHOR_X (1*VISIBLE_MATRIX_WITH)+0
#define ANCHOR_Y (1*VISIBLE_MATRIX_HEIGHT)+0
#else
#define ANCHOR_X 0
#define ANCHOR_Y 0
#endif

//HELPER DEFINES
#ifdef DEBUG
#define _SER_DEBUG_
#endif // DEBUG
#ifdef ENABLE_FRAMERATE_OUTPUT
bool framerate_output_state = false;
#endif

//YOU CAN HERE CREATE A LOOKUPTABLE IF YOUR LEDS NOT IN ROW OR COLLUM
//you can change the led id here if your setup is not collum/row based
//in the defualt setting the virtual led id of led 0 is 0
//if your real first led has for e.g. the number 10 then you can change it here
//you can use excel sheet 'led_lookup_creator'
//#define USE_LED_LOOKUP
//#define USE_LED_LOOKUP_PROGMEM
#ifdef USE_LED_LOOKUP
const int led_id_lookup[LED_COUNT] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254 };
#else
#ifdef USE_LED_LOOKUP_PROGMEM
const int led_id_lookup[LED_COUNT] PROGMEM = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254 };
#endif
#endif




//COLOR PALETTE
//if you define COLOR_TABLE_PROGMEM the color table will be stored in the flash memory so it can be slower
#define COLOR_TABLE_PROGMEM
/* COLOR TABLE SEE EXCEL FILE*/
#define FTC FRM_COLOR //<- is for the excel table generation because the cell char limit.... :/
#ifdef COLOR_TABLE_PROGMEM
const FTC color_lookup_table[64] PROGMEM = { FTC(124,124,124),FTC(0,0,252),FTC(0,0,188),FTC(68,40,188),FTC(148,0,132),FTC(168,0,32),FTC(168,16,0),FTC(136,20,0),FTC(80,48,0),FTC(0,120,0),FTC(0,104,0),FTC(0,88,0),FTC(0,64,88),FTC(0,0,0),FTC(0,0,0),FTC(0,0,0),FTC(188,188,188),FTC(0,120,248),FTC(0,88,248),FTC(104,68,252),FTC(216,0,204),FTC(228,0,88),FTC(248,56,0),FTC(228,92,16),FTC(172,124,0),FTC(0,184,0),FTC(0,168,0),FTC(0,168,68),FTC(0,136,136),FTC(0,0,0),FTC(0,0,0),FTC(0,0,0),FTC(248,248,248),FTC(60,188,252),FTC(104,136,252),FTC(152,120,248),FTC(248,120,248),FTC(248,88,152),FTC(248,120,88),FTC(252,160,68),FTC(248,184,0),FTC(184,248,24),FTC(88,216,84),FTC(88,248,152),FTC(0,232,216),FTC(120,120,120),FTC(0,0,0),FTC(0,0,0),FTC(252,252,252),FTC(164,228,252),FTC(184,184,248),FTC(216,184,248),FTC(248,184,248),FTC(248,164,192),FTC(240,208,176),FTC(252,224,168),FTC(248,216,120),FTC(216,248,120),FTC(184,248,184),FTC(184,248,216),FTC(0,252,252),FTC(248,216,248),FTC(0,0,0),FTC(0,0,0) };
#else
const FTC color_lookup_table[64] = { FTC(124,124,124),FTC(0,0,252),FTC(0,0,188),FTC(68,40,188),FTC(148,0,132),FTC(168,0,32),FTC(168,16,0),FTC(136,20,0),FTC(80,48,0),FTC(0,120,0),FTC(0,104,0),FTC(0,88,0),FTC(0,64,88),FTC(0,0,0),FTC(0,0,0),FTC(0,0,0),FTC(188,188,188),FTC(0,120,248),FTC(0,88,248),FTC(104,68,252),FTC(216,0,204),FTC(228,0,88),FTC(248,56,0),FTC(228,92,16),FTC(172,124,0),FTC(0,184,0),FTC(0,168,0),FTC(0,168,68),FTC(0,136,136),FTC(0,0,0),FTC(0,0,0),FTC(0,0,0),FTC(248,248,248),FTC(60,188,252),FTC(104,136,252),FTC(152,120,248),FTC(248,120,248),FTC(248,88,152),FTC(248,120,88),FTC(252,160,68),FTC(248,184,0),FTC(184,248,24),FTC(88,216,84),FTC(88,248,152),FTC(0,232,216),FTC(120,120,120),FTC(0,0,0),FTC(0,0,0),FTC(252,252,252),FTC(164,228,252),FTC(184,184,248),FTC(216,184,248),FTC(248,184,248),FTC(248,164,192),FTC(240,208,176),FTC(252,224,168),FTC(248,216,120),FTC(216,248,120),FTC(184,248,184),FTC(184,248,216),FTC(0,252,252),FTC(248,216,248),FTC(0,0,0),FTC(0,0,0) };
#endif


//func deklare
void show_output_layer();

//_--------------------VARS





float layer_intense[COUNT_OF_LAYERS];
#ifdef ENABLE_OUTPUT_INTENSE
float output_layer_intense = 1.0f;
#endif // ENABLE_OUTPUT_INTENSE


byte layers[COUNT_OF_LAYERS][TOTAL_MATRIX_WIDHT][TOTAL_MATRIX_HEIGHT]; //our layers
FRM_COLOR output_layer[VISIBLE_MATRIX_WITH][VISIBLE_MATRIX_HEIGHT]; //the final layer to draw
//CONST COLORS
const FRM_COLOR clear_color = FRM_COLOR(0, 0, 0);
const unsigned int clear_color_id = 0; //see clearcolor at lookup table
const FRM_ANCHOR center = FRM_ANCHOR(ANCHOR_X, ANCHOR_Y);




//LED DEFINES
Adafruit_NeoPixel led_matrix = Adafruit_NeoPixel(TOTAL_MATRIX_CELL_COUNT, WS2812_PIN, NEO_GRB + NEO_KHZ800);






//FRAME PARSE INFO
struct SD_FRAME_HEADER {
	unsigned char animation_id;
	unsigned char frame_curr;
	unsigned char  frame_max;
	unsigned char frame_data_w;
	unsigned char frame_data_h;
	void print_header() {
		Serial.print(F("FRAME_HEADER FOR ANIMATION:")); Serial.print(animation_id); Serial.print(F(" CURR:")); Serial.print(frame_curr); Serial.print(F(" FROM : ")); Serial.print(frame_max); Serial.print(F(" W : ")); Serial.print(frame_data_w); Serial.print(F(" H : ")); Serial.println(frame_data_h);
	}

	void read_ram(const int _start_addr) {
		animation_id = read_from_ram(_start_addr + 0);
		frame_curr = read_from_ram(_start_addr + 1);
		frame_max = read_from_ram(_start_addr + 2);
		frame_data_w = read_from_ram(_start_addr + 3);
		frame_data_h = read_from_ram(_start_addr + 4);
	}

};

//GET LED
inline void set_led_color( FRM_ANCHOR _pos,  FRM_COLOR* _color) {
  unsigned int led_id = 0;
  //CHECK IF POINT IS IN RANGE
#ifdef MATRIX_ORIGIN_LEFT_UP
  //NO CHANGE
#endif
#ifdef MATRIX_ORIGIN_LEFT_DOWN
  _pos.y = (VISIBLE_MATRIX_HEIGHT - 1) - _pos.y;
  _pos.x = (VISIBLE_MATRIX_WITH - 1) - _pos.x;
#endif

#ifdef MATRIX_ORIGIN_RIGHT_UP
  _pos.x = (VISIBLE_MATRIX_WITH - 1) - _pos.x;
#endif

#ifdef MATRIX_ORIGIN_RIGHT_DOWN
  _pos.y = (VISIBLE_MATRIX_HEIGHT - 1) - _pos.y;
#endif
#ifdef MATRIX_MODE_COLLUM
  if (_pos.x % 2) //x gerade
    led_id = (_pos.x * VISIBLE_MATRIX_HEIGHT) + _pos.y;
  else //x ungerade
    led_id = (_pos.x * VISIBLE_MATRIX_HEIGHT) + ((VISIBLE_MATRIX_HEIGHT - 1) - _pos.y);
#endif
#ifdef MATRIX_MODE_ROW
  if (_pos.y % 2) //y gerade
    led_id =  _pos.x + (_pos.y * VISIBLE_MATRIX_WITH);
  else //y ungerade
    led_id = ((VISIBLE_MATRIX_WITH - 1) - _pos.x) + (_pos.y * VISIBLE_MATRIX_WITH);
#endif
  //GET ID VIA LOOKUPTABLE
#ifdef USE_LED_LOOKUP
  if (led_id > LED_COUNT) {
    led_id = 0;
  }
  //get new id
  led_id = led_id_lookup[led_id];
#endif
  //SET COLOR
  //CHANGE IT TO HEXVALUE
#ifdef  ENABLE_OUTPUT_INTENSE
  _color->r =(_color->r * output_layer_intense);
  _color->g = (_color->g * output_layer_intense);
  _color->b = (_color->b * output_layer_intense);
#endif //  ENABLE_OUTPUT_INTENSITY
#ifdef LED_COLOR_MODE_RGB
led_matrix.setPixelColor(led_id, _color->r, _color->g, _color->b);
#endif
#ifdef LED_COLOR_MODE_RBG
led_matrix.setPixelColor(led_id, _color->r, _color->b, _color->g);
#endif
#ifdef LED_COLOR_MODE_BGR
led_matrix.setPixelColor(led_id, _color->b, _color->g, _color->r);
#endif
  #ifdef LED_COLOR_MODE_BRG
led_matrix.setPixelColor(led_id, _color->b, _color->r, _color->g);
#endif
#ifdef LED_COLOR_MODE_GBR
led_matrix.setPixelColor(led_id, _color->g, _color->b, _color->r);
#endif
#ifdef LED_COLOR_MODE_GRB
led_matrix.setPixelColor(led_id, _color->g, _color->r, _color->b);
#endif
#ifdef LED_COLOR_MODE_DEFUALT
led_matrix.setPixelColor(led_id, _color->r, _color->g, _color->b);
#endif
}
void generate_output_layer(const bool _direct_show = false) {
#ifdef _SER_DEBUG_
	//Serial.println(F("gen output layer"));
#endif
    for (size_t w = 0; w < VISIBLE_MATRIX_WITH; w++) {
      for (size_t h = 0; h < VISIBLE_MATRIX_HEIGHT; h++) {
        output_layer[w][h].set_color(clear_color, clear_color_id);

        for(size_t _i = 0; _i < COUNT_OF_LAYERS; _i++){
          size_t i =  _i;//REMOVE THAT SHIT
            #if MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR == 1
	            if (layers[i][w][h] != output_layer[w][h].responding_color_id && layers[i][w][h] != clear_color_id) {
					//color_lookup_table[
					output_layer[w][h].set_color(color_lookup_table[layers[i][w][h]].r * layer_intense[i], color_lookup_table[layers[i][w][h]].g * layer_intense[i], color_lookup_table[layers[i][w][h]].b * layer_intense[i], layers[i][w][h]);
					output_layer[w][h].responding_color_id = layers[i][w][h];
              }
            #else
              if (!layers[i][w+VISIBLE_MATRIX_WITH][h+VISIBLE_MATRIX_HEIGHT].equals(output_layer[w][h]) && !layers[i][w+VISIBLE_MATRIX_WITH][h+VISIBLE_MATRIX_HEIGHT].equals(clear_color)) {
                output_layer[w][h].set_color(layers[i][w+VISIBLE_MATRIX_WITH][h+VISIBLE_MATRIX_HEIGHT].r * layer_intense[i],layers[i][w+VISIBLE_MATRIX_WITH][h+VISIBLE_MATRIX_HEIGHT].g * layer_intense[i],layers[i][w+VISIBLE_MATRIX_WITH][h+VISIBLE_MATRIX_HEIGHT].b * layer_intense[i]);
				output_layer[w][h].responding_color_id = layers[i][w][h];
				}
           #endif
         }
      }
    }
	if (!_direct_show) {
		return;
	}
	show_output_layer();

#ifdef ENABLE_FRAMERATE_OUTPUT
framerate_output_state = !framerate_output_state;
digitalWrite(FRAMERATE_OUTPUT_PIN,framerate_output_state);
#endif
}
void clear_layer(const int _id) {
	for (size_t x = 0; x < TOTAL_MATRIX_WIDHT; x++)
	{
		for (size_t y = 0; y < TOTAL_MATRIX_HEIGHT; y++)
		{
			layers[_id][x][y] = clear_color_id;
		}
	}
}
void clear_all_layers() {
  for (size_t i = 0; i < COUNT_OF_LAYERS; i++)
  {
    clear_layer(i);
  }
}
void clear_output_layer() {
  for (size_t x = 0; x < VISIBLE_MATRIX_WITH; x++)
  {
    for (size_t y = 0; y < VISIBLE_MATRIX_HEIGHT; y++)
    {
      output_layer[x][y].set_color(clear_color,clear_color_id);
    }
  }
}
void show_output_layer() {
  for (size_t x = 0; x < VISIBLE_MATRIX_WITH; x++)
  {
    for (size_t y = 0; y < VISIBLE_MATRIX_HEIGHT; y++)
    {
      set_led_color(FRM_ANCHOR(x, y), &output_layer[x][y]);
    }
  }
  led_matrix.show();
}
void  show_specific_layer(const int _id) {
  for (size_t w = 0; w < VISIBLE_MATRIX_WITH; w++) {
    for (size_t h = 0; h < VISIBLE_MATRIX_HEIGHT; h++) {
#if MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR == 1
		output_layer[w][h].set_color(color_lookup_table[layers[_id][w][h]], layers[_id][w][h]);
#else
		output_layer[w][h].set_color(color_lookup_table[layers[_id][w + VISIBLE_MATRIX_WITH][h + VISIBLE_MATRIX_HEIGHT]], layers[_id][w + VISIBLE_MATRIX_WITH][h + VISIBLE_MATRIX_HEIGHT]);
#endif
      
    }
  }
  show_output_layer();
}
inline void set_layer_color(const int _layer, const FRM_ANCHOR* _pos, const unsigned int  _color) {
#if MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR == 3
	layers[_layer][_pos->x + VISIBLE_MATRIX_WITH][_pos->y + VISIBLE_MATRIX_HEIGHT] = _color;
#else
	layers[_layer][_pos->x][_pos->y] = _color;
#endif
}
inline void set_layer_color(const int _layer, const unsigned int _x, const unsigned int _y, const unsigned int _color) {
#if MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR == 3
	layers[_layer][_x + VISIBLE_MATRIX_WITH][_y + VISIBLE_MATRIX_HEIGHT] = _color;
#else
	layers[_layer][_x ][_y ] = _color;
#endif
}


void layer_write_to_serial(unsigned int _layer_id) {
	for (size_t i = 0; i <TOTAL_MATRIX_WIDHT; i++)
	{
		for (size_t j = 0; j < TOTAL_MATRIX_HEIGHT; j++)
		{
			Serial.print(layers[_layer_id][i][j]); Serial.print(" ");
		}
		Serial.println("");
	}
}

void write_sd_animation_to_sram(const char* _path, unsigned int* _next_data_start, const byte animation_id_counter, const  unsigned int _animation_start_addr = 0) {
	//FIRST OPEN FILE TO CHECK EXISTS
	File myFile;
	myFile = SD.open(_path);

	if (!myFile) {
		Serial.println(F("FILE CANNOT BE OPEN"));
		return;
	}

	SD_FRAME_HEADER tmp_header;
	bool frame_started = false;
	unsigned int next_frame_offset = _animation_start_addr; //yeah startoffset ggf add further header offset
	unsigned int next_data_offset = next_frame_offset + sizeof(SD_FRAME_HEADER);
	if (_next_data_start != nullptr) {
		*_next_data_start = next_data_offset + (1 * 1) * 1;
	}
	int row_counter = 0;
	while (myFile.available()) {
		
		//GET CURRENT LINE
		String tmp_line = "";
		tmp_line = myFile.readStringUntil('\n');
		Serial.println(tmp_line);
		//CHECK FOR LINE TYPE
		if (tmp_line.indexOf("_") > 0) {
			tmp_header.frame_curr = getValue(tmp_line, '_', 1).toInt();
			tmp_header.frame_max = getValue(tmp_line, '_', 2).toInt();
			tmp_header.frame_data_w = getValue(tmp_line, '_', 3).toInt();
			tmp_header.frame_data_h = getValue(tmp_line, '_', 4).toInt();
			tmp_header.animation_id = animation_id_counter;
#ifdef _SER_DEBUG_
			tmp_header.print_header();
#endif

			frame_started = true;
		
			write_to_ram(next_frame_offset + 0, tmp_header.animation_id);
			write_to_ram(next_frame_offset + 1, tmp_header.frame_curr);
			write_to_ram(next_frame_offset + 2, tmp_header.frame_max);
			write_to_ram(next_frame_offset + 3, tmp_header.frame_data_w);
			write_to_ram(next_frame_offset + 4, tmp_header.frame_data_h);
     row_counter = 0;
		}
		else if (tmp_line.indexOf(",") > 0) {
			for (size_t i = 0; i < tmp_header.frame_data_w; i++)
			{
				size_t os = ((row_counter * tmp_header.frame_data_w) + i) + next_data_offset;
				unsigned char read_value = (unsigned char)getValue(tmp_line, ',', i).toInt();
				write_to_ram(os, read_value);
#ifdef _SER_DEBUG_
				Serial.print("write cell at "); Serial.print(os); Serial.print(" is "); Serial.println(read_value);
#endif
			}
			row_counter++;
		}
		else //if (tmp_line == "" || myFile.available()) //seems to be an "" line in the cs project i have checked this twice
		{
    row_counter = 0;
			frame_started = false;
			next_frame_offset += sizeof(tmp_header) + (tmp_header.frame_data_w* tmp_header.frame_data_h * sizeof(byte));
			next_data_offset = next_frame_offset + sizeof(SD_FRAME_HEADER);
			if (_next_data_start != nullptr) {
				*_next_data_start = next_data_offset + 1;
			}
	#ifdef _SER_DEBUG_
			Serial.print("FRAME FINISH next frame offset is "); Serial.print(next_frame_offset); Serial.print(" NEXT DATA OFFSET IS:"); Serial.println(next_data_offset);
    #endif
		}

	}
	myFile.close();
}
void read_frame_to_layer(unsigned int _animation_id, unsigned int _frame_id, unsigned int _layer_id, int additional_offset = 0, SD_FRAME_HEADER* _head = NULL) {
	//Serial.println("----------------------------");
	SD_FRAME_HEADER tmp;
	unsigned int  search_offset = 0;
	tmp.read_ram(search_offset);
	//tmp.print_header();

	//SEARCH FOR OFFSET
	while (true)
	{
		if (tmp.animation_id == _animation_id && tmp.frame_curr == _frame_id) {
			//Serial.println("FRAME  FOUND");
			break;
		}
		tmp.read_ram(search_offset);
		if (tmp.animation_id == _animation_id && tmp.frame_curr == _frame_id) {
		//	Serial.println("FRAME  FOUND 1");
			break;
		}
		search_offset += sizeof(SD_FRAME_HEADER) + (tmp.frame_data_w*tmp.frame_data_h * sizeof(byte));
		if (search_offset >= RAM_SIZE) {
			Serial.println("FRAME NOT FOUND");
			return;
		}


	};
	//tmp.print_header();
//	Serial.println(search_offset);
	//WRITE DATA TO LAYER
	*_head = tmp;
	int cell_offset = 0;
	for (size_t i = 0; i < tmp.frame_data_w; i++)
	{
		for (size_t j = 0; j < tmp.frame_data_h; j++)
		{
	cell_offset = search_offset+ sizeof(SD_FRAME_HEADER)+ (j*tmp.frame_data_h) + i;

	//		Serial.print(cell_offset); Serial.print("("); Serial.print(read_from_ram(cell_offset)); Serial.print(") ");
			if (cell_offset >= RAM_SIZE) {
				Serial.println("FRAME NOT FOUND");
				return;
			}
			set_layer_color(_layer_id, i, j, read_from_ram(cell_offset));
		}
	//	Serial.println();
	}


	int t = 0;
}


int anim_counter = 0;
int max_anim = 0;

unsigned long previousMillis = 0;
const long interval = 1000;


void setup()
{
  
  Serial.begin(115200);
  #ifdef _SER_DEBUG_
  Serial.println(F("INIT LED MATRIX"));
#endif
  led_matrix.begin();


#ifdef ENABLE_FRAMERATE_OUTPUT
pinMode(FRAMERATE_OUTPUT_PIN, OUTPUT);
framerate_output_state = false;
digitalWrite(FRAMERATE_OUTPUT_PIN, framerate_output_state);
  #ifdef _SER_DEBUG_
  Serial.print(F("ENABLE FRAMERATE OUTPUT AT PIN"));Serial.println(FRAMERATE_OUTPUT_PIN);
#endif
#endif




#ifdef _SER_DEBUG_
  Serial.println(F("INIT LAYERS"));
#endif
  clear_all_layers();
  clear_output_layer();
  //INIT LAYER INTENSES
  for (size_t i = 0; i < COUNT_OF_LAYERS; i++)
  {
	  layer_intense[i] = 1.0f;
  }
  //YES DIRTY
#ifdef ENABLE_OUTPUT_INTENSE
  output_layer_intense = 1.0f;
#endif // ENABLE_OUTPUT_INTENSE
  generate_output_layer();
  show_output_layer();


  delay(1000);



#ifdef _SER_DEBUG_
  Serial.println(F("INIT SD CARD"));
#endif
  init_ram();

  if (!SD.begin(SD_CARD_CS_PIN)) {
	  Serial.println("initialization failed!");
	  return;
  }
  //LOAD ANIATION TO SD CARD
  write_sd_animation_to_sram("test.txt", NULL,0, 0);


  //if a gpio is high it goes to serial mode with set,<frameoffset>,<pixelid>,<pixelcolor>
  //copy all data from eeprom to sram all bytes!
  SD_FRAME_HEADER tmp;
  read_frame_to_layer(0, 1, 0, 0, &tmp);
 // layer_write_to_serial(0);



  max_anim = tmp.frame_max;
 




 



	



  generate_output_layer();
  show_output_layer();

  delay(100);
}





void loop(){
	
	unsigned long currentMillis = millis();

if((currentMillis - previousMillis) > 100){
		// save the last time you blinked the LED
		previousMillis = currentMillis;
	//	Serial.println("TICK");
		read_frame_to_layer(0, anim_counter, 0, 0);
		anim_counter++;
		if (anim_counter >= max_anim) {
			anim_counter = 0;
		}

	generate_output_layer();
		show_output_layer();
}
	//	delay(500);

		  
}
