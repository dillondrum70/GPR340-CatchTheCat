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
    else if (x + y == rhs.x + rhs.y && x < rhs.x) //if sums equivalent, could be comparing something like (3,2) and (2,3)
    {
        return true; //x coordinate will be deciding factor of whether it is less, lower x coordinate means lower point
        //otherwise, the other x is greater or they are the same point so return false
    }
    return false;
}
bool Point2D::operator>(const Point2D& rhs) const {
    if (x + y > rhs.x + rhs.y) {
        return true;
    }
    else if (x + y == rhs.x + rhs.y && x > rhs.x) //if sums equivalent, could be comparing something like (3,2) and (2,3)
    {
        return true; //x coordinate will be deciding factor of whether it is greater, higher x coordinate means higher point
        //otherwise, the other x is less or they are the same point so return false
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
