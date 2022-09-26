#include "Point2D.h"

bool Point2D::operator==(const Point2D& rhs) const {
    return y == rhs.y && x==rhs.x;
}

bool Point2D::operator!=(const Point2D& rhs) const {
  return x != rhs.x || y!=rhs.y;
}

bool Point2D::operator<(const Point2D& rhs) const { 
    if (x + y < rhs.x + rhs.y) {
        return true;
    } 
    return false;
}
bool Point2D::operator>(const Point2D& rhs) const {
    if (x + y > rhs.x + rhs.y) {
        return true;
    } 
    return false;
}

Point2D &Point2D::operator=(const Point2D &rhs) {
  //Check for self-assignment
  if (this == &rhs)
    return *this;
  x = rhs.x;
  y = rhs.y;
  return *this;
}
