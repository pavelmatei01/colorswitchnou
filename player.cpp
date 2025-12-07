#include "player.h"

Player::Player(float y, Culoare c) : y_(y), velocityY_(0), raza_(15.0f), culoare_(c) {}

void Player::saritura() {
    velocityY_ = FORTA_SARITURA;
}

void Player::update() {
    velocityY_ += GRAVITATIE;
    y_ += velocityY_;

    if (y_ > 800) {
        y_ = 800;
        velocityY_ = 0;
    }
}

void Player::draw(sf::RenderWindow& window) const {
    sf::CircleShape forma(raza_);
    forma.setFillColor(culoare_.getSFMLColor());
    forma.setOrigin(raza_, raza_);
    forma.setPosition(960.f, y_);
    window.draw(forma);
}

Culoare Player::getCuloare() const { return culoare_; }
void Player::setCuloare(Culoare c) { culoare_ = c; }
float Player::getY() const { return y_; }
float Player::getRaza() const { return raza_; }