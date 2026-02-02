#pragma once
#include "obiectjoc.h"
#include "player.h"
#include <vector>
#include <memory>

class Nivel {
    std::vector<std::unique_ptr<ObiectJoc>> elemente;
public:
    Nivel() = default;
    Nivel(const Nivel& other);
    ~Nivel();
    Nivel& operator=(Nivel other);
    friend void swap(Nivel& first, Nivel& second) noexcept;

    void adaugaElement(std::unique_ptr<ObiectJoc> el);
    void goleste(){elemente.clear();}
    int update(Player& player);
    void draw(sf::RenderWindow& window) const;
};