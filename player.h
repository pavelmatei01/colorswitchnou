#pragma once
#include "culoare.h"

class Player {
    float y_;
    float velocityY_;
    float raza_;
    Culoare culoare_;
    const float GRAVITATIE = 0.5f;
    const float FORTA_SARITURA = -8.5f;

public:
    Player(float y, Culoare c);

    void saritura();
    void update();
    void draw(sf::RenderWindow& window) const;

    Culoare getCuloare() const;
    void setCuloare(Culoare c);
    float getY() const;
    float getRaza() const;
};