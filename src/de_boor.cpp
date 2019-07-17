#include "de_boor.h"
#include <iostream>
#include <cmath>


float DeBoor::distance(const Point &a, const Point &b) {
    return std::sqrt(std::pow(b.getX() - a.getX(), 2.0f) + std::pow(b.getY() - a.getY(), 2.0f));
}

Point DeBoor::deBoor(int dec, float degree, int lowerBound, float t, std::vector<int> &knots, std::vector<std::vector<Point>> &tab) {
    if (dec == 0) {
        return tab[0][lowerBound];
    } else {
        double alpha = (t - knots[lowerBound]) /
                       (knots[lowerBound + degree + 1 - dec] - knots[lowerBound]);
        return deBoor(dec - 1, degree, lowerBound - 1, t, knots, tab) * (1 - alpha) +
               deBoor(dec - 1, degree, lowerBound, t, knots, tab) * alpha;
    }
}

void DeBoor::algorithm(std::vector<std::vector<Point>> &tab, std::vector<int> &knots, float degree, float t, int lowerBound) {
    Point point = deBoor(degree, degree, lowerBound, t, knots, tab);

    if (point.getX() > 1 || point.getX() < -1 || point.getY() > 1 || point.getY() < -1) {
        return;
    }
    this->vertices.push_back(point.getX()); // x
    this->vertices.push_back(point.getY()); // y
    this->vertices.push_back(0.0f); // z
    this->vertices.push_back(0.0f); // r
    this->vertices.push_back(1.0f); // g
    this->vertices.push_back(0.0f); // b
}

void DeBoor::fillVertices(std::vector<std::vector<Point>> &tab, float step) {
    std::vector<int> knots;
    for (unsigned int i = 0; i < tab[0].size() + 4; ++i) {
        knots.push_back(i);
    }

    int degree = knots.size() - tab[0].size() - 1;
    int indice = degree;
    for (float i = degree; i < knots.size()-degree; i += step) {
        if (i > indice + 1) {
            indice += 1;
        }
        this->algorithm(tab, knots, degree, i, indice);
    }
}

std::vector<std::vector<Point>> DeBoor::pointsTo2DVec(const std::vector<Point> &points) const {
    int N = points.size();
    std::vector<std::vector<Point>> tab(N, std::vector<Point>(N));

    for (int i = 0; i < N; ++i) {
        tab[0][i] = points[i];
    }

    return tab;
}

std::vector<float> &DeBoor::getVertices() {
    return this->vertices;
}

std::vector<Point> &DeBoor::getNewPoints() {
    return this->newPoints;
}