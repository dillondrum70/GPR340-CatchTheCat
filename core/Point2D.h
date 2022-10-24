#ifndef POINT2D_H
#define POINT2D_H

#include <iostream>

struct Point2D {
public:
    Point2D() {  x = 0; y = 0; }
    Point2D(int x, int y): x(x), y(y){};
    Point2D() = default;
    int x, y;
    bool operator== (const Point2D& rhs) const;
    bool operator!= (const Point2D& rhs) const;
    bool operator<(const Point2D& rhs) const;
    bool operator>(const Point2D& rhs) const;
    friend std::ostream& operator<<(std::ostream& os, const Point2D& point);
    Point2D& operator= (const Point2D& rhs);
};

#endif //POINT2D_H
