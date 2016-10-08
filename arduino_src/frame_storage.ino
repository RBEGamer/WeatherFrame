/*
 * HERE ARE ALL ANIMATIONS SAVED FRAME BY FRAME
 * 
 * 
 * 
 */

 //DEFINE YOUR MATRIX SIZE HERE
#define VISIBLE_MATRIX_WITH 8 //WIDTH
#define VISIBLE_MATRIX_HEIGHT 8 //HEIGHT

/* DONT CHANGE */
#define MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR 3
#define TOTAL_MATRIX_WIDHT (MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR * VISIBLE_MATRIX_WITH)
#define TOTAL_MATRIX_HEIGHT (MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR * VISIBLE_MATRIX_HEIGHT)

/*
 * ---8---- invisible side left/up + ---8--- visible side + ----8---- invisible side right/down
 * 
 * 
 */
#define FRAME_SIZE_WIDTH VISIBLE_MATRIX_WITH
#define FRAME_SIZE_HEIGHT VISIBLE_MATRIX_HEIGHT

#define COUNT_OF_LAYERS 4

//ANCHOR IST UP-LEFT IN THE VISIBLE AREA
#define ANCHOR_X 1*8
#define ANCHOR_Y 1*8

struct ANCHOR{
  int x;
  int y;
  ANCHOR(int _x, int _y){
    x = _x;
    y = _y;
    }
  };

struct COLOR_DESC{
  char r;
  char g;
  char b;

  COLOR_DESC(const char _r, const char _g, const char _b){
    r = _r;
    g = _g;
    b = _b;
    }
  };

  
COLOR_DESC layers[COUNT_OF_LAYERS][FRAME_SIZE_WIDTH][FRAME_SIZE_HEIGHT]; //our layers
COLOR_DESC output_layer[FRAME_SIZE_WIDTH][FRAME_SIZE_HEIGHT]; //the final layer to draw


const COLOR_DESC clear_color = COLOR_DESC(0,0,0);
/*
 * add funktion that renders the final layer
 * auf jeden layer kann ich eine animation legen
 * array für den anchro anlegen
 * in der merge den anchor der layer beachten nur die stellen die sich überlappen mergen ausrechnen...
 * add farbpalette in excel eine 16stufen farbpalette bauen und hier einfügen
 * add funktion farbid to rgb
 * add funktion find neard farb id by rgb
 * beim start alle layer anchors auf 0 setzten
 * add draw pixel and draw frame bei draw pixel auch eine pixelid lookup table erstellen
 * add an additional layer for the output and compare it with the new layer only draw on change enable it with define
 */
//TODO IMPLEMENT OPERATOR
bool check_equal_color(COLOR_DESC _c1, COLOR_DESC _c2){
  return _c1.r && _c2.r && _c1.g && _c2.g && _c1.b && _c2.b;
}

 void clear_layer(const int _id, COLOR_DESC _color){
  for(int w = 0; w < FRAME_SIZE_WIDTH; w++){
    for(int h = 0; h < FRAME_SIZE_HEIGHT; h++){
      layers[_id][w][h] = _color;
      }
    }
 }


 void generate_output_layer(){
  for(int i = 0; < COUNT_OF_LAYERS; i++){
    for(int w = 0; w < FRAME_SIZE_WIDTH; w++){
      for(int h = 0; h < FRAME_SIZE_HEIGHT; h++){
        if(!check_equal_color(layers[COUNT_OF_LAYERS - i][w][h],clear_color)){
          output_layer[w][h] =  layers[COUNT_OF_LAYERS - i][w][h];
        }
      }
    }
   }
 }
