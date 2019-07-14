#include "casteljau.h"

void Casteljau::algorithm(std::vector<std::vector<Point>> tab) const {
    int N = tab[0].size();
    for (int i = 0; i < N; ++i){
        if (tab[0][0] != tab[0][i]) //si le critère d'arrêt est vérifié on s'arrête
            return;
    }
    // Calcul des points intermédiaires
    for (int i = 1; i < N; ++i){
        std::vector<Point> newTab;
        for (int j = 0; j < N - i; ++j){
            tab[i][j] = Point((tab[i-1][j].getX()+tab[i-1][j+1].getX())/2, (tab[i-1][j].getY()+tab[i-1][j+1].getY())/2);
            //T[i][j] = milieu de T[i-1][j] T[i-1][j+1]
        }
    }

    std::vector<std::vector<Point>> tab2(N, std::vector<Point>(N));
    std::vector<std::vector<Point>> tab3(N, std::vector<Point>(N));

    //Afficher T[N][0] // Afficher (ou stocker) le point milieu

    // Construction des courbes restreintes
    for (int i = 0; i < N; ++i){
        tab2[i][0] = tab[i][0];
        tab3[i][0] = tab[N-i][i];
        //T'[i] = T[i][0]
        //T"[i] = T[N-i][i]
    }

    //Appel récursif
    //Casteljau(T')
    //Casteljau(T")
    algorithm(tab2);
    algorithm(tab3);
}

std::vector<std::vector<Point>> Casteljau::pointsTo2DVec(std::vector<Point> points) const {
    int N = points.size();
    std::vector<std::vector<Point>> tab(N, std::vector<Point>(N));
    for (int i = 0; i < N; ++i) {
        tab[0][i] = points[i];
    }

    return tab;
}