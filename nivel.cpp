#include "nivel.h"
#include <algorithm>

void swap(Nivel& first, Nivel& second) noexcept {
    using std::swap;
    swap(first.elemente, second.elemente);
}

Nivel::Nivel(const Nivel& other) {
    for (const auto& el : other.elemente) {
        elemente.push_back(el->clone());
    }
}

Nivel& Nivel::operator=(Nivel other) {
    swap(*this, other);
    return *this;
}

Nivel::~Nivel() {}

void Nivel::adaugaElement(std::unique_ptr<ObiectJoc> el) {
    elemente.push_back(std::move(el));
}

int Nivel::update(Player& player) {
    int puncteFrame = 0;
    auto it = elemente.begin();
    while (it != elemente.end()) {
        (*it)->update();
        bool deSters = (*it)->interactioneaza(player);
        if (deSters) {
            // Adunam punctele (trebuie sa existe getPuncte in ObiectJoc)
            puncteFrame += (*it)->getPuncte();
            it = elemente.erase(it);
        } else {
            ++it;
        }
    }
    return puncteFrame; // Returnam punctele catre Joc

}

void Nivel::draw(sf::RenderWindow& window) const {
    for (const auto& el : elemente) {
        el->draw(window);
    }
}