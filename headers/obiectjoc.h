#pragma once
#include "culoare.h"
#include <memory>
#include <iostream>

class Player;

class ObiectJoc {
protected:
    float y_;
    Culoare culoare_;
    static int contorElemente;


    static float vitezaRotatieGlobala;

public:

    ObiectJoc(float y, Culoare c);
    virtual ~ObiectJoc();
    virtual int getPuncte() const { return 0; }
    static void setViteza(float v) { vitezaRotatieGlobala = v; }
    const Culoare& getCuloareObiect() const { return culoare_; }
    virtual std::unique_ptr<ObiectJoc> clone() const = 0;
    virtual bool interactioneaza(Player& player) = 0;
    virtual void update() = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;

    float getY() const;
    friend std::ostream& operator<<(std::ostream& os, const ObiectJoc& obj);

private:
    virtual void afisare(std::ostream& os) const = 0;
};