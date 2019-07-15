#ifndef CASTELJAU_H
#define CASTELJAU_H

#include <vector>
#include "point.h"

class Casteljau {
public:
    void algorithm(std::vector<std::vector<Point>> &tab, float distMin);
    float distance(const Point &a, const Point &b);
    void pointsTo2DVec(const std::vector<Point> &points, std::vector<std::vector<Point>> &target) const;

    std::vector<float> &getVertices();

private:
    std::vector<float> vertices;
};


#endif