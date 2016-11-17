//WeatherFrame Framework C 2016 Marcel Ochsendorf github.com/RBEGamer marcel.ochsendorf@gmail.com marcelochsendorf.com
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <SPI.h>
#include <SD.h>

#include "CONFIGURATION.h"
#include "FRM_COLOR.cpp"
#include "helper_funcs.cpp"



//SYSTEM DEFINES




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



#ifdef ENABLE_FRAMERATE_OUTPUT
bool framerate_output_state = false;
#endif

#ifdef USE_LED_LOOKUP
const int led_id_lookup[LED_COUNT] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254 };
#else
#ifdef USE_LED_LOOKUP_PROGMEM
const int led_id_lookup[LED_COUNT] PROGMEM = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254 };
#endif
#endif

//DEMO RAM
#ifndef DEMO_RAM
#define DEMO_RAM
#define DEMO_RAM_SIZE 65536
#define DEMO_RAM_INIT_CHAR 0
#define DEMO_RAM_ERROR_CHAR 0

unsigned char demo_ram[DEMO_RAM_SIZE];

inline void write_to_ram(unsigned int _addr, unsigned char _value) {
	if (_addr > DEMO_RAM_SIZE - 1) {
#ifdef _SER_DEBUG_
		Serial.println(F("DEMO RAM - OUT OF RANGE"));
#endif
	}
	else {
		demo_ram[_addr] = _value;
	}
}
inline unsigned char read_from_ram(unsigned int _addr) {
	if (_addr > DEMO_RAM_SIZE - 1) {
#ifdef _SER_DEBUG_
		Serial.println(F("DEMO RAM - OUT OF RANGE"));
#endif
		return DEMO_RAM_ERROR_CHAR;
	}
	return demo_ram[_addr];
}
void init_ram() {
	for (int i = 0; i < DEMO_RAM_SIZE; i++) {
		demo_ram[i] = DEMO_RAM_INIT_CHAR;
	}
}
#endif // !DEMO_RAM





//func deklare
void show_output_layer();

//_--------------------VARS


void print_color(FRM_COLOR _col) {
	Serial.print("COLOR R:"); Serial.print(_col.r);
	Serial.print(" G:"); Serial.print(_col.g);
	Serial.print(" B:"); Serial.println(_col.b);
}



String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {
    0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}


float layer_intense[COUNT_OF_LAYERS];
#ifdef ENABLE_OUTPUT_INTENSE
float output_layer_intense = 1.0f;
#endif // ENABLE_OUTPUT_INTENSE


byte layers[COUNT_OF_LAYERS][TOTAL_MATRIX_WIDHT][TOTAL_MATRIX_HEIGHT]; //our layers
FRM_COLOR output_layer[VISIBLE_MATRIX_WITH][VISIBLE_MATRIX_HEIGHT]; //the final layer to draw
//CONST COLORS
const FRM_COLOR clear_color = FRM_COLOR(0, 0, 0);
const unsigned int clear_color_id = 0; //see clearcolor at lookup table


//LED DEFINES
Adafruit_NeoPixel led_matrix = Adafruit_NeoPixel(TOTAL_MATRIX_CELL_COUNT, WS2812_PIN, NEO_GRB + NEO_KHZ800);






//FRAME PARSE INFO HRADER V2 7 Byte
struct SD_FRAME_HEADER {
	unsigned char animation_id;
	unsigned char frame_curr;
	unsigned char  frame_max;
	unsigned char frame_data_w;
	unsigned char frame_data_h;
	unsigned char frame_visibility;
	unsigned char frame_delay;

	void print_header() {
		Serial.print(F("FRAME_HEADER FOR ANIMATION:")); Serial.print(animation_id); Serial.print(F(" CURR:")); Serial.print(frame_curr); Serial.print(F(" FROM : ")); Serial.print(frame_max); Serial.print(F(" W : ")); Serial.print(frame_data_w); Serial.print(F(" H : ")); Serial.print(frame_data_h); Serial.print(F(" DELAY : ")); Serial.print(frame_delay); Serial.print(F(" VISIBLE : ")); Serial.println(frame_visibility);
	}

	void read_ram(const int _start_addr) {
		animation_id = read_from_ram(_start_addr + 0);
		frame_curr = read_from_ram(_start_addr + 1);
		frame_max = read_from_ram(_start_addr + 2);
		frame_data_w = read_from_ram(_start_addr + 3);
		frame_data_h = read_from_ram(_start_addr + 4);
		frame_visibility = read_from_ram(_start_addr + 5);
		frame_delay = read_from_ram(_start_addr + 6);
	}

};



//GET LED
inline void set_led_color( int _x, int _y,  FRM_COLOR* _color) {
  unsigned int led_id = 0;
  //CHECK IF POINT IS IN RANGE
#ifdef MATRIX_ORIGIN_LEFT_UP
  //NO CHANGE
#endif
#ifdef MATRIX_ORIGIN_LEFT_DOWN
  _y = (VISIBLE_MATRIX_HEIGHT - 1) - _y;
  _x = (VISIBLE_MATRIX_WITH - 1) - _x;
#endif

#ifdef MATRIX_ORIGIN_RIGHT_UP
  _x = (VISIBLE_MATRIX_WITH - 1) - _x;
#endif

#ifdef MATRIX_ORIGIN_RIGHT_DOWN
  _y = (VISIBLE_MATRIX_HEIGHT - 1) - _y;
#endif
#ifdef MATRIX_MODE_COLLUM
  if (_x % 2) //x gerade
    led_id = (_x * VISIBLE_MATRIX_HEIGHT) + _y;
  else //x ungerade
    led_id = (_x * VISIBLE_MATRIX_HEIGHT) + ((VISIBLE_MATRIX_HEIGHT - 1) - _y);
#endif
#ifdef MATRIX_MODE_ROW
  if (_y % 2) //y gerade
    led_id =  _x + (_y * VISIBLE_MATRIX_WITH);
  else //y ungerade
    led_id = ((VISIBLE_MATRIX_WITH - 1) - _x) + (_y * VISIBLE_MATRIX_WITH);
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
	clear_output_layer();

    for (size_t w = 0; w < VISIBLE_MATRIX_WITH; w++) {
      for (size_t h = 0; h < VISIBLE_MATRIX_HEIGHT; h++) {
       

        for(size_t _i = 0; _i < COUNT_OF_LAYERS; _i++){
          size_t i =  _i;//REMOVE THAT SHIT
            #if MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR == 1
	            if (layers[i][w][h] != clear_color_id) {
					//color_lookup_table[
					output_layer[w][h].set_color(
						read_from_ram(RAM_COLOR_TABLE_OFFSET + (layers[i][w][h] * 3) + 0)  ,
						read_from_ram(RAM_COLOR_TABLE_OFFSET + (layers[i][w][h] * 3) + 1) ,
						read_from_ram(RAM_COLOR_TABLE_OFFSET + (layers[i][w][h] * 3) + 2) );
					
		
					
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
void clear_output_layer() {
	for (size_t x = 0; x < VISIBLE_MATRIX_WITH; x++)
	{
		for (size_t y = 0; y < VISIBLE_MATRIX_HEIGHT; y++)
		{
			output_layer[x][y].set_color(clear_color.r, clear_color.g, clear_color.b);
		}
	}
}
void clear_all_layers() {
  for (size_t i = 0; i < COUNT_OF_LAYERS; i++)
  {
    clear_layer(i);
  }
  clear_output_layer();
}
void show_output_layer() {
  for (size_t x = 0; x < VISIBLE_MATRIX_WITH; x++)
  {
    for (size_t y = 0; y < VISIBLE_MATRIX_HEIGHT; y++)
    {
      set_led_color(x, y, &output_layer[x][y]);
    }
  }
  led_matrix.show();
#ifdef ENABLE_FRAMERATE_OUTPUT
  framerate_output_state = !framerate_output_state;
  digitalWrite(FRAMERATE_OUTPUT_PIN, framerate_output_state);
#endif
}
inline void set_layer_color(const int _layer, const unsigned int _x, const unsigned int _y, const unsigned int _color) {
#if MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR == 3
	layers[_layer][_x + VISIBLE_MATRIX_WITH][_y + VISIBLE_MATRIX_HEIGHT] = _color;
#else
	layers[_layer][_x][_y] = _color;
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
		tmp_line = myFile.readStringUntil(SD_FILE_NEW_LINE_CHAR);
		//Serial.println(tmp_line);
		//CHECK FOR LINE TYPE
		if (tmp_line.indexOf(SD_FILE_FRAME_HEADER_SEPERATOR) > 0) {
			tmp_header.frame_curr = getValue(tmp_line, SD_FILE_FRAME_HEADER_SEPERATOR, 1).toInt();
			tmp_header.frame_max = getValue(tmp_line, SD_FILE_FRAME_HEADER_SEPERATOR, 2).toInt();
			tmp_header.frame_data_w = getValue(tmp_line, SD_FILE_FRAME_HEADER_SEPERATOR, 3).toInt();
			tmp_header.frame_data_h = getValue(tmp_line, SD_FILE_FRAME_HEADER_SEPERATOR, 4).toInt();
			tmp_header.frame_visibility = getValue(tmp_line, SD_FILE_FRAME_HEADER_SEPERATOR, 5).toInt();
			tmp_header.frame_delay = getValue(tmp_line, SD_FILE_FRAME_HEADER_SEPERATOR, 6).toInt();
			tmp_header.animation_id = animation_id_counter;
#ifdef _SER_DEBUG_
		//	tmp_header.print_header();
#endif
//LOAD ONLY RIGHT SIZE ANIMATION
#ifdef SD_LOAD_ONLY_RIGHT_SIZE_ANIMATIONS
if(tmp_header.frame_data_w > TOTAL_MATRIX_WIDHT){
  tmp_header.frame_data_w = TOTAL_MATRIX_WIDHT;
  }
  if(tmp_header.frame_data_h > TOTAL_MATRIX_HEIGHT){
  tmp_header.frame_data_h = TOTAL_MATRIX_HEIGHT;
  }
#endif
			frame_started = true;
			write_to_ram(next_frame_offset + 0, tmp_header.animation_id);
			write_to_ram(next_frame_offset + 1, tmp_header.frame_curr);
			write_to_ram(next_frame_offset + 2, tmp_header.frame_max);
			write_to_ram(next_frame_offset + 3, tmp_header.frame_data_w);
			write_to_ram(next_frame_offset + 4, tmp_header.frame_data_h);
			write_to_ram(next_frame_offset + 5, tmp_header.frame_visibility);
			write_to_ram(next_frame_offset + 6, tmp_header.frame_delay);

			row_counter = 0;
		}
		else if (tmp_line.indexOf(SD_FILE_FRAME_DATA_SEPERATOR) > 0) {
    if(row_counter >= tmp_header.frame_data_h){
      continue;
      }
      
			for (size_t i = 0; i < tmp_header.frame_data_w; i++)
			{
				size_t os = ((row_counter * tmp_header.frame_data_w) + i) + next_data_offset;
				unsigned char read_value = (unsigned char)getValue(tmp_line, SD_FILE_FRAME_DATA_SEPERATOR, i).toInt();
				write_to_ram(os, read_value);
#ifdef _SER_DEBUG_
				//Serial.print("write cell at "); Serial.print(os); Serial.print(" is "); Serial.println(read_value);
#endif
			}
			row_counter++;
      
		}
		else
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
void read_frame_to_layer(unsigned int _frame_id, unsigned int _layer_id,unsigned int additional_offset = 0, SD_FRAME_HEADER* _head = nullptr) {
	//Serial.println("----------------------------");
	SD_FRAME_HEADER tmp;
	unsigned int  search_offset = additional_offset;
	tmp.read_ram(search_offset);
	//tmp.print_header();
	
	//SEARCH FOR OFFSET
	while (true)
	{
		if ( tmp.frame_curr == _frame_id) {
			//Serial.println("FRAME  FOUND");
			break;
		}
		tmp.read_ram(search_offset);
		if ( tmp.frame_curr == _frame_id) {
		//Serial.println("FRAME  FOUND 1");
			break;
		}
		//break at end of animation
		if (search_offset >= RAM_SIZE || search_offset >= ((tmp.frame_data_h* tmp.frame_data_w * sizeof(byte) * tmp.frame_max + 1) + (sizeof(SD_FRAME_HEADER)* tmp.frame_max))+ additional_offset) {
			Serial.println("FRAME NOT FOUND SKIP FRAME");
			return;
		}
		search_offset += sizeof(SD_FRAME_HEADER) + (tmp.frame_data_w*tmp.frame_data_h * sizeof(byte));

	};

	
	//Serial.println();
	*_head = tmp;
	if (tmp.frame_visibility >= 0.0f) {
		layer_intense[_layer_id] = (1.0f / (float)tmp.frame_visibility);
	}
	else {
		Serial.println(F("READ FRAME TO LAYER - DIV 0"));
		layer_intense[_layer_id] = 1.0f;
	}
	int cell_offset = 0;
	for (size_t i = 0; i < tmp.frame_data_w; i++)
	{
		for (size_t j = 0; j < tmp.frame_data_h; j++)
		{
	cell_offset = search_offset+ sizeof(SD_FRAME_HEADER)+ (j*tmp.frame_data_h) + i;

	//		Serial.print(cell_offset); Serial.print("("); Serial.print(read_from_ram(cell_offset)); Serial.print(") ");
			if (cell_offset >= RAM_SIZE) {
				Serial.println("FRAME NOT FOUND - SKIP FRAME");
				return;
			}
			set_layer_color(_layer_id, i, j, read_from_ram(cell_offset));
		}
	//	Serial.println();
	}


	int t = 0;
}

//RAM POS VARS
unsigned int written_colors = 0;
unsigned int next_free_after_color_table = 0;

bool load_color_table(const char* _path, unsigned int _write_offset = 0, unsigned int* _next_data_start = nullptr, unsigned int* _loaded_colors = nullptr) {
	File myFile;
	myFile = SD.open(_path);
	if (!myFile) {
		Serial.println(F("FILE CANNOT BE OPEN"));
		return false;
	}
	unsigned int next_data_pos = _write_offset;
	unsigned int lines = 0;
	if (_loaded_colors != nullptr) {
		*_loaded_colors = 0;
	}
	FRM_COLOR tmp_col;
	while (myFile.available()) {
				String tmp_line = "";
				tmp_line = myFile.readStringUntil(SD_FILE_NEW_LINE_CHAR);
				//Serial.println(tmp_line);
				write_to_ram(next_data_pos + 0, (unsigned char)getValue(tmp_line, SD_FILE_CSV_SEPERATION_CHAR, 0).toInt());
				write_to_ram(next_data_pos + 1, (unsigned char)getValue(tmp_line, SD_FILE_CSV_SEPERATION_CHAR, 1).toInt());
				write_to_ram(next_data_pos + 2, (unsigned char)getValue(tmp_line, SD_FILE_CSV_SEPERATION_CHAR, 2).toInt());
				//Serial.println(next_data_pos);		
				next_data_pos += 3;
				lines++;
	}
	

	if (_next_data_start != nullptr) {
		*_next_data_start = next_data_pos;
	}
	if (_loaded_colors != nullptr) {
		*_loaded_colors = lines;
	}
	myFile.close();
	return true;
}


struct CONFIG_HEADER
{

};

bool load_config_file(const char* _path) {

	return true;
}


int anim_counter = 0;
int max_anim = 0;

unsigned long previousMillis = 0;



SD_FRAME_HEADER* animation_frame = nullptr;



void setup()
{
	animation_frame = new SD_FRAME_HEADER();
	animation_frame->frame_delay = 100;

  Serial.begin(SERIAL_BAUD_RATE);
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
  for (size_t i = 0; i < COUNT_OF_LAYERS; i++){layer_intense[i] = 1.0f;}
  //YES DIRTY
#ifdef ENABLE_OUTPUT_INTENSE
  output_layer_intense = 1.0f;
#endif // ENABLE_OUTPUT_INTENSE



#ifdef _SER_DEBUG_
  Serial.println(F("INIT RAM"));
#endif
  init_ram();


#ifdef _SER_DEBUG_
  Serial.println(F("INIT SD CARD"));
#endif
 

  if (!SD.begin(SD_CARD_CS_PIN)) {
	  Serial.println("SD INI FAILED");
	  return;
  }

  //LOAD COLOR TABLE
  if (!load_color_table(SD_FILE_DEF_COLOR_TABLE, RAM_COLOR_TABLE_OFFSET,&next_free_after_color_table, &written_colors)) {
	  Serial.println("color table >COLORS.csv<  loading error");
	  return;
  }


  //LOAD ANIATION TO SD CARD //WRITE ANIMATION AT THE POSTION AFTER THE COLOR TABLE
  write_sd_animation_to_sram("TEST.ANI", NULL, 0, next_free_after_color_table);




  read_frame_to_layer(anim_counter, 0, next_free_after_color_table, animation_frame);




  max_anim = animation_frame->frame_max;
 
  generate_output_layer();
  show_output_layer();
  layer_write_to_serial(0);
}

void loop(){

	unsigned long currentMillis = millis();

if((currentMillis - previousMillis) > animation_frame->frame_delay*3){
	
		previousMillis = currentMillis;
		Serial.println(animation_frame->frame_delay);
		read_frame_to_layer(anim_counter, 0, next_free_after_color_table, animation_frame);
		anim_counter++;
		if (anim_counter >= max_anim) {
			anim_counter = 0;
		}

	generate_output_layer();
		show_output_layer();
}
	//	delay(500);

		  
}
