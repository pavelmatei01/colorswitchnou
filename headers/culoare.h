#pragma once
#include <iostream>
#include <string>
#include <exception>
#include <SFML/Graphics.hpp>
class EroareJoc : public std::exception {
protected:
    std::string mesaj;
public:
    explicit EroareJoc(const std::string& msg) : mesaj(msg) {}
    const char* what() const noexcept override { return mesaj.c_str(); }
};

class EroareLogica : public EroareJoc {
public:
    explicit EroareLogica(const std::string& msg) : EroareJoc("Eroare Logica: " + msg) {}
};

class NotificareVictorie : public std::exception {
public:
    const char* what() const noexcept override { return "Victorie!"; }
};


enum class TipCuloare { GALBEN, VERDE, ROSU, ALBASTRU, MULTICOLOR };

class Culoare {
    TipCuloare tip_;
public:
    Culoare(TipCuloare tip = TipCuloare::GALBEN) : tip_(tip) {}

    bool sePotriveste(const Culoare& other) const {
        if (tip_ == TipCuloare::MULTICOLOR || other.tip_ == TipCuloare::MULTICOLOR) return true;
        return tip_ == other.tip_;
    }

    TipCuloare getTip() const { return tip_; }

    sf::Color getSFMLColor() const {
        switch (tip_) {
            case TipCuloare::GALBEN: return sf::Color::Yellow;
            case TipCuloare::VERDE: return sf::Color(39, 139, 34);
            case TipCuloare::ROSU: return sf::Color(220,0,0);
            case TipCuloare::ALBASTRU: return sf::Color(0,0,220);
            default: return sf::Color::White;
        }
    }
};