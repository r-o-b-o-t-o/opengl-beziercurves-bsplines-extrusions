#ifndef CASTELJAU_H
#define CASTELJAU_H

#include <vector>
#include "point.h"

class Casteljau {
public:
    void algorithm(std::vector<std::vector<Point>> tab);
    std::vector<std::vector<Point>> pointsTo2DVec(std::vector<Point> points) const;

    std::vector<Point> getPointsToShow() const;
    
private:
    std::vector<Point> pointsToShow;
};


#endif