
#include "obiectjoc.h"

int ObiectJoc::contorElemente = 0;

float ObiectJoc::vitezaRotatieGlobala = 1.0f;


ObiectJoc::ObiectJoc(float y, Culoare c) : y_(y), culoare_(c) {
    contorElemente++;
}

ObiectJoc::~ObiectJoc() {
    contorElemente--;
}

float ObiectJoc::getY() const { return y_; }


std::ostream& operator<<(std::ostream& os, const ObiectJoc& obj) {
    obj.afisare(os);
    return os;
}