#include "point.h"

Point::Point(): x(0.0f), y(0.0f){

}

Point::Point(float x, float y): x(x), y(y){
}

float Point::getX() const{
    return this->x;
}

float Point::getY() const{
    return this->y;
}

void Point::setX(float x) {
    this->x = x;
}

void Point::setY(float y) {
    this->y = y;
}

Point Point::operator+(const Point &other) {
    return Point(this->x+other.getX(), this->y+other.getY());
}

Point Point::operator*(float other){
    
    return Point(this->x*other, this->y*other);
}

bool Point::operator!=(const Point &other){
    return !(*this == other);
}

bool Point::operator==(const Point &other){
    if (this->x == other.getX() && this->y == other.getY()) {
        return true;
    }
    return false;
}