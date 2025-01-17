#ifndef POINT_H
#define POINT_H

class Point {
public:
    Point();
    Point(float x, float y);
    float getX() const;
    float getY() const;
    void setX(float);
    void setY(float);

    Point operator+(const Point &other);
    Point operator*(float other);
    bool operator!=(const Point &other);
    bool operator==(const Point &other);
private:
    float x;
    float y;
};


#endif