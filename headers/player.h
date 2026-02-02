#pragma once
#include "culoare.h"

class Player {
    float y_;
    float velocityY_;
    float raza_;
    Culoare culoare_;
    float timerMulticolor_;
    const float GRAVITATIE = 0.5f;
    const float FORTA_SARITURA = -8.5f;

public:
    Player(float y, Culoare c);

    void saritura();
    void update();
    void setPozitie(float y) {
        y_ = y;
        velocityY_ = 0;
    }
    void activeazaMulticolor() {
        timerMulticolor_ = 4.0f;
    }
    bool esteMulticolor() const {return timerMulticolor_ > 0.0f;}
    void draw(sf::RenderWindow& window) const;

    Culoare getCuloare() const;
    void setCuloare(Culoare c);
    float getY() const;
    float getRaza() const;
};