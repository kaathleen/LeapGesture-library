#ifndef GLCOLOR
#define GLCOLOR

struct GLColor 
{
  GLColor() : r(1), g(1), b(1), a(1) {}

  GLColor( float _r, float _g, float _b, float _a=1 )
    : r(_r), g(_g), b(_b), a(_a)
  {}

  explicit GLColor( const Colour& juceColor ) 
    : r(juceColor.getFloatRed()), 
      g(juceColor.getFloatGreen()),
      b(juceColor.getFloatBlue()),
      a(juceColor.getFloatAlpha())
  {}

  operator const GLfloat*() const { return &r; }

  GLfloat r, g, b, a; 
};

#endif  // GLCOLOR