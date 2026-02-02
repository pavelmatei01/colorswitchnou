#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
const double PI_VAR=3.14159265358979323846;
inline void adaugaSegmentArc(sf::VertexArray& va, sf::Vector2f centru, float rInt, float rExt,
                             float startGrade, float stopGrade, sf::Color col, float rotatie) {
    float startAjustat = startGrade + rotatie;
    float stopAjustat = stopGrade + rotatie;
    float startRad = startAjustat * PI_VAR/ 180.f;
    float stopRad = stopAjustat * PI_VAR/180.f;

    int segmente = 20;
    float pas = (stopRad - startRad) / segmente;

    for (int i = 0; i <= segmente; ++i) {
        float unghi = startRad + pas * i;
        float cosA = std::cos(unghi);
        float sinA = std::sin(unghi);


        va.append(sf::Vertex(sf::Vector2f(centru.x + rExt * cosA, centru.y + rExt * sinA), col));
        va.append(sf::Vertex(sf::Vector2f(centru.x + rInt * cosA, centru.y + rInt * sinA), col));
    }
}