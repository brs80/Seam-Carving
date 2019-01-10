#include "pixel.hpp"
#include <stdexcept>

Pixel::Pixel() : exist(false) { }

Pixel::Pixel(const int& val) : exist(true), val(val) { }

bool Pixel::hasVal() const {
    return exist;
}

int Pixel::getVal() const {
    return val;
}

void Pixel::clearVal() {
    exist = false;
  }

void Pixel::setVal(const int& val) {
    this->val = val;
    this->exist = true;
}

bool operator == (const Pixel& a, const Pixel& b) {
    return a.hasVal() == b.hasVal() && a.getVal() == b.getVal();
}


bool operator != (const Pixel& a, const Pixel& b) {
    return !(a == b);
  }


bool operator < (const Pixel& a, const Pixel& b) {
    return !b.hasVal() || (a.hasVal() && a.getVal() < b.getVal());
}