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
