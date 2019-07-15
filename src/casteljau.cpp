#include "casteljau.h"
#include <iostream>
#include <cmath>


float Casteljau::distance(const Point &a, const Point &b) {
    return std::sqrt(std::pow(b.getX() - a.getX(), 2.0f) + std::pow(b.getY() - a.getY(), 2.0f));
}

void Casteljau::algorithm(std::vector<std::vector<Point>> &tab, float distMin) {
    int N = tab[0].size();
    bool ret = true;
    for (int i = 0; i < N-1; ++i) {
        if (distance(tab[0][i],tab[0][i+1]) > distMin) { 
            ret = false;
        }
    }

    if (ret)//si le critère d'arrêt est vérifié on s'arrête
        return;
        
    // Calcul des points intermédiaires
    for (int i = 1; i < N; ++i){
        for (int j = 0; j < N - i; ++j){
            if (j >= N || j+1 >= N)
                continue;
            float x = (tab[i-1][j].getX()+tab[i-1][j+1].getX())/2;
            float y = (tab[i-1][j].getY()+tab[i-1][j+1].getY())/2;
            tab[i][j] = Point(x, y);
            //T[i][j] = milieu de T[i-1][j] T[i-1][j+1]
        }
    }

    std::vector<std::vector<Point>> tab2(N, std::vector<Point>(N));
    std::vector<std::vector<Point>> tab3(N, std::vector<Point>(N));


    //std::vector<std::vector<Point>> tab2 = tab;
    //std::vector<std::vector<Point>> tab3 = tab;
    //Afficher T[N][0] // Afficher (ou stocker) le point milieu
    this->vertices.push_back(tab[N-1][0].getX()); // x
    this->vertices.push_back(tab[N-1][0].getY()); // y
    this->vertices.push_back(0.0f); // z
    this->vertices.push_back(0.0f); // r
    this->vertices.push_back(1.0f); // g
    this->vertices.push_back(0.0f); // b

    // Construction des courbes restreintes
    for (int i = 0; i < N; ++i) {
        tab2[0][i] = tab[i][0];
        if (N-1-i >= 0)
            tab3[0][i] = tab[N-1-i][i];
        //T'[i] = T[i][0]
        //T"[i] = T[N-i][i]
    }

    //Appel récursif
    //Casteljau(T')
    //Casteljau(T")
    algorithm(tab2, distMin);
    algorithm(tab3, distMin);
}

void Casteljau::pointsTo2DVec(const std::vector<Point> &points, std::vector<std::vector<Point>> &target) const {
    int N = points.size();
    for (int i = 0; i < N; ++i) {
        target[0][i] = points[i];
    }
}

std::vector<float> &Casteljau::getVertices() {
    return this->vertices;
}