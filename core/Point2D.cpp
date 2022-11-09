#include "Point2D.h"


const Point2D Point2D::UP = Point2D(0, -1);
const Point2D Point2D::DOWN = Point2D(0, 1);
const Point2D Point2D::LEFT = Point2D(-1, 0);
const Point2D Point2D::RIGHT = Point2D(1, 0);


bool Point2D::operator==(const Point2D& rhs) const {
	return y == rhs.y && x == rhs.x;
}

bool Point2D::operator!=(const Point2D& rhs) const {
	return x != rhs.x || y != rhs.y;
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

std::ostream& operator<<(std::ostream& os, const Point2D& point)
{
    os << "(" << point.x << ", " << point.y << ")";
    return os;
}

Point2D &Point2D::operator=(const Point2D &rhs) {
  //Check for self-assignment
  if (this == &rhs)
    return *this;
  x = rhs.x;
  y = rhs.y;
  return *this;
}

Point2D Point2D::operator+(const Point2D& rhs) const {
	return { x + rhs.x, y + rhs.y };
}

Point2D Point2D::operator-(const Point2D& rhs) const {
	return { x - rhs.x, y - rhs.y };
}
std::string Point2D::to_string() {
  return "{" + std::to_string(x) + ", " + std::to_string(y) + "}";
}
