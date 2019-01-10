#ifndef PIXEL_HPP
#define PIXEL_HPP

class Pixel {
  bool exist;
  int val;
public:

  Pixel();
  Pixel(const int& val);
  bool hasVal() const;
  int getVal() const;
  void clearVal();
  void setVal(const int& val);

};

bool operator == (const Pixel&, const Pixel&);
bool operator != (const Pixel&, const Pixel&);
bool operator < (const Pixel&, const Pixel&);

#include "Pixel.cpp"
#endif