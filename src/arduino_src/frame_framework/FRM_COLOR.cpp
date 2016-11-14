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



    FRM_COLOR(const FRM_COLOR_TYPE _r,const  FRM_COLOR_TYPE _g, const FRM_COLOR_TYPE _b) {
      r = _r;
      g = _g;
      b = _b;
    }
    FRM_COLOR() {
      r = 0;
      g = 0;
      b = 0;
    }


 
    void set_color(const FRM_COLOR_TYPE _r,const FRM_COLOR_TYPE _g,const FRM_COLOR_TYPE _b) {
      r = _r ;
      g = _g ;
      b = _b ;

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
