/*
  add funktion that renders the final layer
  auf jeden layer kann ich eine animation legen


  add farbpalette in excel eine 16stufen farbpalette bauen und hier einf�gen
  add funktion farbid to rgb
  add funktion find neard farb id by rgb
 

TODO


->add animation struct
->add animation frame0 -> frame1 linear
*/














//DEFINE YOUR MATRIX SIZE HERE
#define VISIBLE_MATRIX_WITH 8 //WIDTH
#define VISIBLE_MATRIX_HEIGHT 8 //HEIGHT
#define LED_COUNT (VISIBLE_MATRIX_WITH*VISIBLE_MATRIX_HEIGHT) + 0
//YOU CAN DEFINE HERE YOUR MATRIX SETUP
#define MATRIX_ORIGIN_LEFT_UP
#define MATRIX_MODE_ROW
#define COUNT_OF_LAYERS 4 //SET LAYER COUNT HERE WATCH YOUR RAM
#define LED_COLOR_MODE_RGB // DEFAULT RGB RBG BGR BRG GBR GRB
//YOU CAN HERE CREATE A LOOKUPTABLE IF YOUR LEDS NOT IN ROW OR COLLUM
//#define USE_LED_LOOKUP
#ifdef USE_LED_LOOKUP
//the lookuptable must be the size of yourled amount!!!!
const int led_id_lookup_size = 256;
const int led_id_lookup[256] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254 };
#endif
//CONST DEFINES
#define MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR 3
#define TOTAL_MATRIX_WIDHT (MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR * VISIBLE_MATRIX_WITH)
#define TOTAL_MATRIX_HEIGHT (MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR * VISIBLE_MATRIX_HEIGHT)
#define TOTAL_MATRIX_CELL_COUNT (VISIBLE_MATRIX_WITH*VISIBLE_MATRIX_HEIGHT)
#define FRAME_SIZE_WIDTH VISIBLE_MATRIX_WITH
#define FRAME_SIZE_HEIGHT VISIBLE_MATRIX_HEIGHT
//ANCHOR IST UP-LEFT IN THE VISIBLE AREA  HERE YOU CAN ADD A OFFSET
#define ANCHOR_X (1*VISIBLE_MATRIX_WITH)+0
#define ANCHOR_Y (1*VISIBLE_MATRIX_HEIGHT)+0


//COLOR STRUCT
#ifndef F_FRM_COLOR
#define F_FRM_COLOR
#define COLOR_DESC FRM_COLOR
#define FRM_COLOR_TYPE byte
//TODO IMPLEMENT OPERATOR
class FRM_COLOR
{
  public:
   
    FRM_COLOR_TYPE r;
    FRM_COLOR_TYPE g;
    FRM_COLOR_TYPE b;
    const FRM_COLOR_TYPE min_off = 0;
    const FRM_COLOR_TYPE max_on = 255;

    FRM_COLOR(FRM_COLOR_TYPE _r, FRM_COLOR_TYPE _g, FRM_COLOR_TYPE _b) {
      r = _r;
      g = _g;
      b = _b;
 
    }

    FRM_COLOR() {
      r = min_off;
      g = min_off;
      b = min_off;
  
    }



    void make_max() {
      r = max_on;
      g = max_on;
      b = max_on;
    }

    void make_min() {
      r = min_off;
      g = min_off;
      b = min_off;

    }
    //NOT SAFE !!!!
    void invert() {
      r = max_on - r;
      g = max_on - g;
      b = max_on - b;
    }

    inline void set_color(FRM_COLOR_TYPE _r, FRM_COLOR_TYPE _g, FRM_COLOR_TYPE _b) {
      r = _r;
      g = _g ;
      b = _b ;
    }

    void set_color(FRM_COLOR _c) {
   
      r = _c.r ;
      g = _c.g ;
      b = _c.b ;
    }

inline FRM_COLOR_TYPE get_r(){

  return r;
  }
  
inline FRM_COLOR_TYPE get_g(){
  return g;
  }
  
  inline FRM_COLOR_TYPE get_b(){
  return b;
  }
    bool equals(FRM_COLOR _e) {
      //return _c1.r && _c2.r && _c1.g && _c2.g && _c1.b && _c2.b;
      if (_e.r == r && _e.g == g && _e.b == b) {
        return true;
      }
      return false;
    }

    FRM_COLOR operator=(const FRM_COLOR _right ) {
      return FRM_COLOR(_right.r, _right.g, _right.b);
    }
};
#endif

#ifndef F_FRM_ANCHOR
#define F_FRM_ANCHOR
#define FRM_INT int
class FRM_ANCHOR
{
  public:
    FRM_INT x;
    FRM_INT y;

    FRM_ANCHOR() {
      x = 0;
      y = 0;
    }

    FRM_ANCHOR(const FRM_INT _x, const FRM_INT _y) {
      x = _x;
      y = _y;
    }
    ~FRM_ANCHOR() {
    }

    FRM_ANCHOR operator=(const FRM_ANCHOR _right) {
      return FRM_ANCHOR(_right.x, _right.y);
    }

  private:

};
#endif


float layer_intense[COUNT_OF_LAYERS] = {1.0f,1.0f,1.0f,1.0f};
FRM_COLOR layers[COUNT_OF_LAYERS][TOTAL_MATRIX_WIDHT][TOTAL_MATRIX_HEIGHT]; //our layers
FRM_COLOR output_layer[VISIBLE_MATRIX_WITH][VISIBLE_MATRIX_HEIGHT]; //the final layer to draw
//CONST COLORS
const FRM_COLOR clear_color = FRM_COLOR(0, 0, 0);
const FRM_ANCHOR center = FRM_ANCHOR(ANCHOR_X, ANCHOR_Y);


#ifndef FRM_ANIMATIONS
#define FRM_ANIMATIONS
class FRM_ANIMATION
{
public:
	FRM_ANIMATION();
	~FRM_ANIMATION();


	void process_animation(); //if playing play animation to layer
	void load_animation(FRM_COLOR* _dest_layer, FRM_COLOR* _frame_data, int* _frame_count);

	//VARS
	int current_animation_frame = 0;
	float fade_steps = 1.0f / 50.0f;
	float current_fade_amount = 0.0f;
	bool fade_dir = false;
	int anmimation_frames = 4; // count of frames
	int next_frame_interval = 1000; //time for next frame
	FRM_COLOR* animation_data = nullptr; //points to the FRM <animation_name>-Array
	FRM_COLOR* destination_layer = nullptr;
	bool animation_playing = false;
	FRM_ANCHOR animation_anchor = FRM_ANCHOR(VISIBLE_MATRIX_WITH, VISIBLE_MATRIX_HEIGHT);
private:

};

FRM_ANIMATION::FRM_ANIMATION()
{
}

FRM_ANIMATION::~FRM_ANIMATION()
{
}








#endif


#define
//RAIN ANIMATION
FRM_COLOR rain[4][8][8] = {{{FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0)}, {FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0)}}, {{FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0)}, {FRM_COLOR(100, 149, 237), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0)},}, {{FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0)}, {FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0)}, {FRM_COLOR(100, 149, 237), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0)}}, {{FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0)}, {FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0)}, {FRM_COLOR(100, 149, 237), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237)}, {FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0), FRM_COLOR(100, 149, 237), FRM_COLOR(0, 0, 0), FRM_COLOR(0, 0, 0)}}};
const int frame_size = 4;
unsigned long previousMillis = 0;
const long interval = 1000; 
int animation_state = 0;
float fade = 0.0f;
int loaded_animation = 0;
bool dir = false;
float steps = 1.0f / 50.0f;



/*
  /*LED VARS*/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define WS2812_PIN 14 //SETUP THE LED OUT
Adafruit_NeoPixel led_matrix = Adafruit_NeoPixel(TOTAL_MATRIX_CELL_COUNT, WS2812_PIN, NEO_GRB + NEO_KHZ800);





//GET LED
//SET LED COLOR WITH #defined LOOKUPTABLE
inline void set_led_color(FRM_ANCHOR _pos, FRM_COLOR* _color) {
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
  if (led_id > led_id_lookup_size) {
    led_id = 0;
    //ADD DEBUG SHIT
  }
  //get new id
  led_id = led_id_lookup[led_id];
#endif

  //SET COLOR
  //CHANGE IT TO HEXVALUE

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





void generate_output_layer() {
 
    for (int w = 0; w < VISIBLE_MATRIX_WITH; w++) {
      for (int h = 0; h < VISIBLE_MATRIX_HEIGHT; h++) {
        output_layer[w][h].set_color(clear_color);

   //   for (int i = 0; i < COUNT_OF_LAYERS; i++) {

   for(int _i = 0; _i < COUNT_OF_LAYERS; _i++){
    int i =  _i;//REMOVE THAT SHIT
        if (!layers[i][w+VISIBLE_MATRIX_WITH][h+VISIBLE_MATRIX_HEIGHT].equals(output_layer[w][h]) && !layers[i][w+VISIBLE_MATRIX_WITH][h+VISIBLE_MATRIX_HEIGHT].equals(clear_color)) {
          output_layer[w][h].set_color(
                                        layers[i][w+VISIBLE_MATRIX_WITH][h+VISIBLE_MATRIX_HEIGHT].r * layer_intense[i],
                                        layers[i][w+VISIBLE_MATRIX_WITH][h+VISIBLE_MATRIX_HEIGHT].g * layer_intense[i],
                                        layers[i][w+VISIBLE_MATRIX_WITH][h+VISIBLE_MATRIX_HEIGHT].b * layer_intense[i]);          
      }
   }

      }
    }
  
}



void clear_all_layers() {
  for (size_t i = 0; i < COUNT_OF_LAYERS; i++)
  {
    for (size_t x = 0; x < TOTAL_MATRIX_WIDHT; x++)
    {
      for (size_t y = 0; y < TOTAL_MATRIX_HEIGHT; y++)
      {
        layers[i][x][y] = clear_color;
      }
    }
  }
}


void clear_layer(const int _id) {
  for (size_t x = 0; x < TOTAL_MATRIX_WIDHT; x++)
  {
    for (size_t y = 0; y < TOTAL_MATRIX_HEIGHT; y++)
    {
      layers[_id][x][y] = clear_color;
    }
  }
}

void clear_output_layer() {
  for (size_t x = 0; x < VISIBLE_MATRIX_WITH; x++)
  {
    for (size_t y = 0; y < VISIBLE_MATRIX_HEIGHT; y++)
    {
      output_layer[x][y] = clear_color;
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
  for (int w = 0; w < VISIBLE_MATRIX_WITH; w++) {
    for (int h = 0; h < VISIBLE_MATRIX_HEIGHT; h++) {
      output_layer[w][h].set_color(layers[_id][w + VISIBLE_MATRIX_WITH][h + VISIBLE_MATRIX_HEIGHT]);
    }
  }
  show_output_layer();
}




void setup()
{
  Serial.begin(115200);
  led_matrix.begin();
  clear_all_layers();
  clear_output_layer();



  /* add setup code here */
  //LOAD FIRST ANIMATION
  for (size_t x = 0; x < 8; x++)
  {
	  for (size_t y = 0; y < 8; y++)
	  {
		  layers[0][x + VISIBLE_MATRIX_WITH][y + VISIBLE_MATRIX_HEIGHT].set_color(rain[loaded_animation][x][y]);
		  layers[1][x + VISIBLE_MATRIX_WITH][y + VISIBLE_MATRIX_HEIGHT].set_color(rain[loaded_animation + 1][x][y]);

	  }
  }
  generate_output_layer();
  show_output_layer();




}




void loop(){
 




	if (dir) {
		fade += steps;
	}
	else {
		fade -= steps;
	}
	

		   layer_intense[0] = 1.0f - fade;
		   layer_intense[1] = 0.0f + fade;


		   if ((fade >= 0.9f && dir) || (fade <= 0.1f && !dir)) {
			   dir = !dir;
			   
			   loaded_animation++;
			   if (loaded_animation > 2) {
				   loaded_animation = 0;
			   }
			   clear_layer(1);
			   clear_layer(0);
			   for (size_t x = 0; x < 8; x++)
			   {
				   for (size_t y = 0; y < 8; y++)
				   {
					   layers[1][x + VISIBLE_MATRIX_WITH][y + VISIBLE_MATRIX_HEIGHT].set_color(rain[loaded_animation][x][y]);
					   layers[0][x + VISIBLE_MATRIX_WITH][y + VISIBLE_MATRIX_HEIGHT].set_color(rain[loaded_animation + 1][x][y]);

				   }
			   }

		   }


		   generate_output_layer();
		   show_output_layer();

		   delay(70);
	   



  


  


}
