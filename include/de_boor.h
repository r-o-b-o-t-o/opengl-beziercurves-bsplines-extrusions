#ifndef DE_BOOR_H
#define DE_BOOR_H

#include <vector>
#include "point.h"

class DeBoor {
public:
    void algorithm(std::vector<std::vector<Point>> &tab, std::vector<int> &knots, float degree, float t, int lowerBound);
    float distance(const Point &a, const Point &b);
    Point deBoor(int dec, float degree, int lowerBound, float t, std::vector<int> &knots, std::vector<std::vector<Point>> &tab);
    void fillVertices(std::vector<std::vector<Point>> &tab, float step);
    std::vector<std::vector<Point>> pointsTo2DVec(const std::vector<Point> &points) const;

    std::vector<float> &getVertices();
    std::vector<Point> &getNewPoints();

private:
    std::vector<float> vertices;
    std::vector<Point> newPoints;
};


#endif