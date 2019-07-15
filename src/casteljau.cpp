#include "casteljau.h"
#include <iostream>
#include <cmath>


float Casteljau::distance(Point a, Point b) {
    return std::sqrtf(std::pow(b.getX() - a.getX(), 2) + std::pow(b.getY() - a.getY(), 2));
}

void Casteljau::algorithm(std::vector<std::vector<Point>> tab, float distMin) {
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
    this->pointsToShow.push_back(tab[N-1][0]);

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

std::vector<std::vector<Point>> Casteljau::pointsTo2DVec(std::vector<Point> points) const {
    int N = points.size();
    std::vector<std::vector<Point>> tab(N, std::vector<Point>(N));
    for (int i = 0; i < N; ++i) {
        tab[0][i] = points[i];
    }

    return tab;
}

std::vector<Point> &Casteljau::getPointsToShow() {
    return this->pointsToShow;
}