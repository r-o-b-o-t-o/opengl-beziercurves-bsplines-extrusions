#include "de_boor.h"
#include <iostream>
#include <cmath>


float DeBoor::distance(const Point &a, const Point &b) {
    return std::sqrt(std::pow(b.getX() - a.getX(), 2.0f) + std::pow(b.getY() - a.getY(), 2.0f));
}

Point DeBoor::deBoor(int dec, float degree, int lowerBound, float t, std::vector<int> &knots, std::vector<std::vector<Point>> &tab){
    if(dec == 0)
        return tab[0][lowerBound];
    else
    {
        double alpha = (t-knots[lowerBound])/(knots[lowerBound+degree+1-dec]-knots[lowerBound]);
        return (deBoor(dec-1,degree, lowerBound-1, t, knots, tab)*(1-alpha )+deBoor(dec-1,degree, lowerBound, t, knots, tab)*alpha );
    }
}

void DeBoor::algorithm(std::vector<std::vector<Point>> &tab, std::vector<int> &knots, float degree, float t, int lowerBound) {
    int N = tab[0].size();
    //k: index of knot interval that contains x
    //x: position
    //t: array of knot positions, needs to be padded as described above
    //c: array of control points
    //p: degree of B-spline
    //std::vector<std::vector<int>> w(N , std::vector<int>(N));
    //std::vector<std::vector<Point>> d(N , std::vector<Point>(N));

    /*std::cout << "Pushing control points" << std::endl;
    for (int j = 0; j <= degree && j < N; ++j) {
        d[0][j] = tab[0][lowerBound + j - degree];
    }
    // = [c[j + k - p] for j in range(0, p+1)]

    /*for (int r = 1; r < degree+1; ++r){
        for (int j = degree; j > r-1; --j){
            int alpha = (t - knots[j]) / (knots[j+degree-(r-1)]-knots[j]);
            d[j] = d[j-1] * (1 - alpha) + d[j] * alpha;
        }
    }*/

    Point point = deBoor(degree, degree, lowerBound, t, knots, tab);

    /*std::cout << "Applying algo" << std::endl;

    for (int j = 1; j < degree; ++j){
        for (int i = 1; i <= degree-j; ++i){
            int alpha = (t - knots[lowerBound+i-(degree+j)]) / (knots[lowerBound+j]-knots[lowerBound+i-(degree+j)]);
            d[j][i] = d[j-1][i] * (1 - alpha) + d[j-1][i+1] * alpha;
        }
    }

    /*for (int r = 1; r < degree; ++r){
        for (int j = i−degree+r; j < ; ++i){
            int alpha = (t - knots[lowerBound+i-(degree+j)]) / (knots[lowerBound+j]-knots[lowerBound+i-(degree+j)]);
            d[j][i] = d[j-1][i] * (1 - alpha) + d[j-1][i+1] * alpha;
        }
    }*/



    //std::cout << "Inserting results" << std::endl;
    /*for r in range(1, p+1):
        for j in range(p, r-1, -1):
            alpha = (x - t[j+k-p]) / (t[j+1+k-r] - t[j+k-p])
            d[j] = (1.0 - alpha) * d[j-1] + alpha * d[j]*/
    //this->vertices.emplace_back(d[degree].getX(), d[degree].getY());
    if ( point.getX() > 1 || point.getX() < -1 || point.getY() > 1 || point.getY() < -1 )
        return;
    std::cout << "X = " <<  point.getX() << ", Y = " << point.getY() << std::endl;
    this->vertices.push_back(point.getX()); // x
    this->vertices.push_back(point.getY()); // y
    this->vertices.push_back(0.0f); // z
    this->vertices.push_back(0.0f); // r
    this->vertices.push_back(1.0f); // g
    this->vertices.push_back(0.0f); // b
    //this->newPoints.emplace_back(d[degree].getX(), d[degree].getY());
}

void DeBoor::fillVertices(std::vector<std::vector<Point>> &tab, float step) {
    std::vector<int> knots;
    std::cout << "filling vertices : { " << std::endl;
    for (int i = 0; i < tab[0].size() + 4; ++i) {
        std::cout << i << ", ";
        knots.push_back(i);
    }
    std::cout << "}"<< std::endl;
    int degree = knots.size() - tab[0].size() - 1;
    //this->algorithm(tab, knots, 2, 2);
    int indice = degree;
    for (float i = degree; i < knots.size()-degree; i+=step){
        if (i > indice + 1)
            indice += 1;
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