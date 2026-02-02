#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "player.h"
#include "nivel.h"
#include "utils.h"
#include "achievements.h"

class Joc {
private:
    sf::RenderWindow window_;
    sf::View camera_;

    sf::Texture bgTexture_;
    sf::Sprite bgSprite_;

    sf::Font font_;
    sf::Text scoreText_;
    sf::Text nivelText_;
    bool isPaused_;
    sf::Text pauseText_;
    sf::Text resumeText_;
    sf::RectangleShape pauseOverlay_;

    Statistica<int> scorStat_;
    Statistica<float> distantaStat_;

    // OBSERVER SYSTEM
    AchievementSystem achievementSystem_;

    std::unique_ptr<Player> player_;
    Nivel nivel_;

    bool isGameOver_;
    bool isVictorie_;
    int nivelCurent_;
    const int MAX_NIVELE=5;
    const int LATIME_FEREASTRA = 1920;
    const int INALTIME_FEREASTRA = 1080;

    void procesareEvenimente();
    void update();
    void render();
    void initializareNivel();
    void trecereNivelUrmator();
    void gestioneazaVictorie();
    void gestioneazaInfrangere(const std::string& motiv);

public:
    Joc();
    void run();
};