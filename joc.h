#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "player.h"
#include "nivel.h"

class Joc {
private:
    sf::RenderWindow window_;
    sf::View camera_;


    sf::Texture bgTexture_;
    sf::Sprite bgSprite_;

    sf::Font font_;
    sf::Text scoreText_;
    int score_;

    std::unique_ptr<Player> player_;
    Nivel nivel_;

    bool isGameOver_;
    bool isVictorie_;

    const int LATIME_FEREASTRA = 1920;
    const int INALTIME_FEREASTRA = 1080;

    void procesareEvenimente();
    void update();
    void render();
    void initializareNivel();
    void gestioneazaVictorie();
    void gestioneazaInfrangere(const std::string& motiv);

public:
    Joc();
    void run();
};