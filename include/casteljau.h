#ifndef CASTELJAU_H
#define CASTELJAU_H

#include <vector>
#include "point.h"

class Casteljau {
public:
    void algorithm(std::vector<std::vector<Point>> tab) const;
    std::vector<std::vector<Point>> pointsTo2DVec(std::vector<Point> points) const;
    
private:
};


#endif