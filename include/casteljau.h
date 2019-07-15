#ifndef CASTELJAU_H
#define CASTELJAU_H

#include <vector>
#include "point.h"

class Casteljau {
public:
    void algorithm(std::vector<std::vector<Point>> tab, float distMin);
    float distance(Point a, Point b);
    std::vector<std::vector<Point>> pointsTo2DVec(std::vector<Point> points) const;

    std::vector<Point> &getPointsToShow();

private:
    std::vector<Point> pointsToShow;
};


#endif