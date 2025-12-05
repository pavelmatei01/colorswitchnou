#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Functie helper pentru a desena un segment de cerc (folosita la obstacole)
inline void adaugaSegmentArc(sf::VertexArray& va, sf::Vector2f centru, float rInt, float rExt,
                             float startGrade, float stopGrade, sf::Color col, float rotatie) {
    float startAjustat = startGrade + rotatie;
    float stopAjustat = stopGrade + rotatie;
    float startRad = startAjustat * M_PI / 180.f;
    float stopRad = stopAjustat * M_PI / 180.f;

    int segmente = 20;
    float pas = (stopRad - startRad) / segmente;

    for (int i = 0; i <= segmente; ++i) {
        float unghi = startRad + pas * i;
        float cosA = std::cos(unghi);
        float sinA = std::sin(unghi);

        // Triangle Strip: Exterior apoi Interior
        va.append(sf::Vertex(sf::Vector2f(centru.x + rExt * cosA, centru.y + rExt * sinA), col));
        va.append(sf::Vertex(sf::Vector2f(centru.x + rInt * cosA, centru.y + rInt * sinA), col));
    }
}