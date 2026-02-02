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
    if (timerMulticolor_ > 0.0f) {
        timerMulticolor_ -= 1.0f/60.0f;
    }
}

void Player::draw(sf::RenderWindow& window) const {
    sf::CircleShape forma(raza_);
    if (timerMulticolor_>0.0f) {
        static int frame=0; frame++;
        switch ((frame/5)%4) {
            case 0:forma.setFillColor(sf::Color::Red);break;
            case 1:forma.setFillColor(sf::Color::Blue);break;
            case 2:forma.setFillColor(sf::Color::Green);break;
            case 3:forma.setFillColor(sf::Color::Yellow);break;
        }

    }
    else {
        forma.setFillColor(culoare_.getSFMLColor());
    }

    forma.setOrigin(raza_, raza_);
    forma.setPosition(960.f, y_);
    window.draw(forma);
}

Culoare Player::getCuloare() const {
    if (timerMulticolor_>0.0f) {
        return Culoare(TipCuloare::MULTICOLOR);
    }
    return culoare_;
}
void Player::setCuloare(Culoare c) { culoare_ = c; }
float Player::getY() const { return y_; }
float Player::getRaza() const { return raza_; }