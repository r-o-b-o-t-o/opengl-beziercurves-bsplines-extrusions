#ifndef DE_BOOR_H
#define DE_BOOR_H

#include <vector>
#include "point.h"

class DeBoor {
public:
    void algorithm(std::vector<std::vector<Point>> &tab, std::vector<int> knots, float degree, int t);
    float distance(const Point &a, const Point &b);
    void fillVertices(std::vector<std::vector<Point>> &tab, float step);
    std::vector<std::vector<Point>> pointsTo2DVec(const std::vector<Point> &points) const;

    std::vector<float> &getVertices();
    std::vector<Point> &getNewPoints();

private:
    std::vector<float> vertices;
    std::vector<Point> newPoints;
};


#endif