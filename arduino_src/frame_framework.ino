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

//DEFINE YOUR MATRIX SIZE HERE
#define VISIBLE_MATRIX_WITH 8 //WIDTH
#define VISIBLE_MATRIX_HEIGHT 8 //HEIGHT


/* DONT CHANGE */
#define MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR 3
#define TOTAL_MATRIX_WIDHT (MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR * VISIBLE_MATRIX_WITH)
#define TOTAL_MATRIX_HEIGHT (MATRIX_INVISIBLE_SIZE_MULTIPLIKATOR * VISIBLE_MATRIX_HEIGHT)


#define FRAME_SIZE_WIDTH VISIBLE_MATRIX_WITH
#define FRAME_SIZE_HEIGHT VISIBLE_MATRIX_HEIGHT

#define COUNT_OF_LAYERS 4

//ANCHOR IST UP-LEFT IN THE VISIBLE AREA  HERE YOU CAN ADD A OFFSET
#define ANCHOR_X (1*VISIBLE_MATRIX_WITH)+0
#define ANCHOR_Y (1*VISIBLE_MATRIX_HEIGHT)+0

//COLOR STRUCT
//#ifndef __F_FRM_COLOR__
//#define __F_FRM_COLOR__

//#define COLOR_DESC FRM_COLOR

//TODO IMPLEMENT OPERATOR



 class FRM_COLOR
{
public:
	FRM_COLOR(char _r, char _g, char _b) {
		r = _r;
		g = _g;
		b = _b;
	}
	
	FRM_COLOR() {
		r = 0;
		g = 0;
		b = 0;
	}
	char r;
	char g;
	char b;
	



	bool equals(FRM_COLOR _e) {
		//return _c1.r && _c2.r && _c1.g && _c2.g && _c1.b && _c2.b;
		if (_e.r == r && _e.g == g && _e.b == b) {
			return true;
		}
		return false;
	}
};





//#endif




FRM_COLOR layers[COUNT_OF_LAYERS][TOTAL_MATRIX_WIDHT][TOTAL_MATRIX_HEIGHT]; //our layers
FRM_COLOR output_layer[TOTAL_MATRIX_WIDHT][TOTAL_MATRIX_HEIGHT]; //the final layer to draw
//CONST COLORS
const FRM_COLOR clear_color = FRM_COLOR(0,0,0);








/*
void generate_output_layer() {
	for (int i = 0; i < COUNT_OF_LAYERS; i++) {
		for (int w = 0; w < FRAME_SIZE_WIDTH; w++) {
			for (int h = 0; h < FRAME_SIZE_HEIGHT; h++) {
				if (!check_equal_color(layers[COUNT_OF_LAYERS - i][w][h], clear_color)) {
					output_layer[w][h] = layers[COUNT_OF_LAYERS - i][w][h];
				}
			}
		}
	}
}
*/


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
	for (size_t x = 0; x < TOTAL_MATRIX_WIDHT; x++)
	{
		for (size_t y = 0; y < TOTAL_MATRIX_HEIGHT; y++)
		{
			output_layer[x][y] = clear_color;
		}
	}
}



void show_layer() {

}


void  show_specific_layer(const int _id) {

}

void setup()
{

	clear_all_layers();
	clear_output_layer();








  /* add setup code here */

}

void loop()
{

  /* add main program code here */

}
