#include "de_boor.h"
#include <iostream>
#include <cmath>


float DeBoor::distance(const Point &a, const Point &b) {
    return std::sqrt(std::pow(b.getX() - a.getX(), 2.0f) + std::pow(b.getY() - a.getY(), 2.0f));
}

void DeBoor::algorithm(std::vector<std::vector<Point>> &tab, std::vector<int> knots, float degree, int t) {
    int N = tab[0].size();
    //k: index of knot interval that contains x
    //x: position
    //t: array of knot positions, needs to be padded as described above
    //c: array of control points
    //p: degree of B-spline
    std::vector<std::vector<int>> w(N , std::vector<int>(N));
    std::vector<Point> d;

    for (int j = degree; j < knots.size() + degree && j < N; ++j) {
        d.emplace_back(tab[0][j].getX(), tab[0][j].getY());
    }
    // = [c[j + k - p] for j in range(0, p+1)]

    for (int r = 1; r < degree; ++r){
        for (int j = degree; j > r-1; --j){
            int alpha = (t - knots[j]) / (knots[j+degree-(r-1)]-knots[j]);
            d[j] = d[j-1] * (1 - alpha) + d[j] * alpha;
        }
    }

    /*for r in range(1, p+1):
        for j in range(p, r-1, -1):
            alpha = (x - t[j+k-p]) / (t[j+1+k-r] - t[j+k-p])
            d[j] = (1.0 - alpha) * d[j-1] + alpha * d[j]*/
    //this->vertices.emplace_back(d[degree].getX(), d[degree].getY());
    /*std::cout << "X = " <<  d[degree].getX() << ", Y = " << d[degree].getY() << std::endl;
    this->vertices.push_back(d[degree].getX()); // x
    this->vertices.push_back(d[degree].getY()); // y
    this->vertices.push_back(0.0f); // z
    this->vertices.push_back(0.0f); // r
    this->vertices.push_back(1.0f); // g
    this->vertices.push_back(0.0f); // b*/
    this->newPoints.emplace_back(d[degree].getX(), d[degree].getY());
}

void DeBoor::fillVertices(std::vector<std::vector<Point>> &tab, float step) {
    std::vector<int> knots;
    std::cout << "filling vertices : { " << std::endl;
    for (int i = 0; i < tab[0].size(); ++i) {
        std::cout << i << ", ";
        knots.push_back(i);
    }
    std::cout << "}"<< std::endl;

    int indice = 0;
    for (float i = 0; i < tab[0].size(); ++i){
        //if (i > indice + 1)
        //    indice += 1;
        this->algorithm(tab, knots, 0, i);
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