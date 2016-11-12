//COLOR STRUCT
#ifndef F_FRM_COLOR
#define F_FRM_COLOR
#define COLOR_DESC FRM_COLOR
#define FRM_COLOR_TYPE  unsigned char
//TODO IMPLEMENT OPERATOR
class FRM_COLOR
{
  public:
    FRM_COLOR_TYPE r;
    FRM_COLOR_TYPE g;
    FRM_COLOR_TYPE b;
    const FRM_COLOR_TYPE min_off = 0;
    const FRM_COLOR_TYPE max_on = 255;


  unsigned int responding_color_id = 0;
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
    inline void set_color(FRM_COLOR_TYPE _r, FRM_COLOR_TYPE _g, FRM_COLOR_TYPE _b,unsigned int _color_id) {
      r = _r;
      g = _g ;
      b = _b ;
    responding_color_id = _color_id;
    }
    void set_color(FRM_COLOR _c, unsigned int _color_id) {
      r = _c.r ;
      g = _c.g ;
      b = _c.b ;
    responding_color_id = _color_id;
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
