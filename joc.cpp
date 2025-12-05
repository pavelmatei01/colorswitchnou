#include "joc.h"
#include "elemente.h"
#include "culoare.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

Joc::Joc()
    : window_(sf::VideoMode(1920, 1080), "Color Switch", sf::Style::Close | sf::Style::Titlebar),
      isGameOver_(false),
      isVictorie_(false),
      score_(0) // Initializam scorul cu 0
{
    window_.setFramerateLimit(60);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Centrare fereastra
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    int x = (desktop.width - LATIME_FEREASTRA) / 2;
    int y = (desktop.height - INALTIME_FEREASTRA) / 2;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    window_.setPosition(sf::Vector2i(x, y));
    window_.requestFocus();

    camera_.setSize(LATIME_FEREASTRA, INALTIME_FEREASTRA);
    camera_.setCenter(LATIME_FEREASTRA / 2.0f, INALTIME_FEREASTRA / 2.0f);

    // --- 1. BACKGROUND ---
    if (bgTexture_.loadFromFile("background.jpg")) {
        bgSprite_.setTexture(bgTexture_);
        sf::Vector2u marimeImg = bgTexture_.getSize();
        bgSprite_.setScale(1920.0f / marimeImg.x, 1080.0f / marimeImg.y);
    }

    // --- 2. SCOR SI FONT ---
    if (!font_.loadFromFile("helvetica.ttf")) {
        std::cerr << "EROARE: Nu s-a gasit helvetica.ttf!" << std::endl;
    }

    scoreText_.setFont(font_);
    scoreText_.setCharacterSize(50);
    scoreText_.setFillColor(sf::Color::White);
    scoreText_.setPosition(30.f, 30.f); // Stanga sus
    scoreText_.setString("0"); // Pornim de la 0

    // --- 3. RESURSE STATICE ---
    Stea::incarcaTextura();

    // Player
    int randomColorIndex = std::rand() % 4;
    Culoare culoareInitiala(static_cast<TipCuloare>(randomColorIndex));
    player_ = std::make_unique<Player>(800.0f, culoareInitiala);

    initializareNivel();
}

void Joc::initializareNivel() {
    // ... (Pastreaza codul de generare random pe care l-ai avut anterior) ...
    // E identic cu cel de data trecuta, nu il mai scriu aici ca sa nu ocup spatiu
    // Daca il vrei din nou, spune-mi!

    // [LOGICA GENERARE RANDOM HERE]
    int numarObstacole = 5;
    float distantaIntreObstacole = 700.0f;
    float yCurent = 300.0f;
    TipCuloare culoarePrezisa = player_->getCuloare().getTip();

    for (int i = 0; i < numarObstacole; ++i) {
        int tipObstacol = std::rand() % 5;
        switch (tipObstacol) {
            case 0: nivel_.adaugaElement(std::make_unique<ObstacolRotativ>(yCurent)); break;
            case 1: nivel_.adaugaElement(std::make_unique<ObstacolPatrat>(yCurent)); break;
            case 2: nivel_.adaugaElement(std::make_unique<ObstacolDublu>(yCurent)); break;
            case 3: nivel_.adaugaElement(std::make_unique<ObstacolDouaElice>(yCurent)); break;
            case 4:
                bool spreDreapta = (std::rand() % 2 == 0);
                nivel_.adaugaElement(std::make_unique<ObstacolBanda>(yCurent, spreDreapta));
                break;
        }

        bool steaLaMijloc = (std::rand() % 2 == 0);
        if (tipObstacol == 4) steaLaMijloc = true;

        if (i < numarObstacole - 1) {
            float yMijloc = yCurent - (distantaIntreObstacole / 2.0f);
            if (steaLaMijloc) {
                nivel_.adaugaElement(std::make_unique<Stea>(yMijloc));
            } else {
                nivel_.adaugaElement(std::make_unique<Stea>(yCurent));
                auto schimbator = std::make_unique<SchimbatorCuloare>(yMijloc, culoarePrezisa);
                culoarePrezisa = schimbator->getCuloareObiect().getTip();
                nivel_.adaugaElement(std::move(schimbator));
            }
        } else {
            if (!steaLaMijloc) nivel_.adaugaElement(std::make_unique<Stea>(yCurent));
        }
        yCurent -= distantaIntreObstacole;
    }
    nivel_.adaugaElement(std::make_unique<LinieSosire>(yCurent - 300.0f));
}

void Joc::run() {
    while (window_.isOpen()) {
        procesareEvenimente();
        if (!isGameOver_ && !isVictorie_) update();
        render();
    }
}

void Joc::procesareEvenimente() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            // Daca apasam Escape, iesim oricand
            if (event.key.code == sf::Keyboard::Escape) {
                window_.close();
            }

            // Logica pentru SPACE
            if (event.key.code == sf::Keyboard::Space) {
                if (!isGameOver_ && !isVictorie_) {
                    // In timpul jocului: Sare
                    player_->saritura();
                } else {
                    // Daca e Game Over si apasam Space: Inchidem jocul
                    window_.close();
                }
            }
        }
    }
}

void Joc::update() {
    try {
        player_->update();

        // --- ACTUALIZARE SCOR ---
        // Primim punctele de la nivel
        int puncteNoi = nivel_.update(*player_);

        if (puncteNoi > 0) {
            score_ += puncteNoi;
            // Actualizam textul
            scoreText_.setString(std::to_string(score_));
        }

    } catch (const NotificareVictorie&) {
        gestioneazaVictorie();
    } catch (const EroareLogica& e) {
        gestioneazaInfrangere(e.what());
    }
}

void Joc::render() {
    window_.clear(sf::Color::Black); // Fundal negru implicit

    if (isGameOver_ || isVictorie_) {
        // --- ECRAN FINAL (Game Over sau Victorie) ---

        // 1. Resetam camera (sa fie fixa pe ecran)
        window_.setView(window_.getDefaultView());

        // 2. Configuram Textul Principal (Titlu)
        sf::Text titluText;
        titluText.setFont(font_);
        titluText.setCharacterSize(80);
        titluText.setStyle(sf::Text::Bold);

        if (isVictorie_) {
            titluText.setString("VICTORIE!");
            titluText.setFillColor(sf::Color::Green);
        } else {
            titluText.setString("GAME OVER");
            titluText.setFillColor(sf::Color::Red);
        }

        // Centrare Titlu
        sf::FloatRect textRect = titluText.getLocalBounds();
        titluText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
        titluText.setPosition(1920 / 2.0f, 1080 / 2.0f - 50.0f); // Putintel mai sus de centru

        // 3. Configuram Textul Scorului
        sf::Text scorFinalText;
        scorFinalText.setFont(font_);
        scorFinalText.setCharacterSize(50);
        scorFinalText.setFillColor(sf::Color::White);
        scorFinalText.setString("Scor Final: " + std::to_string(score_));

        // Centrare Scor
        sf::FloatRect scorRect = scorFinalText.getLocalBounds();
        scorFinalText.setOrigin(scorRect.left + scorRect.width/2.0f, scorRect.top + scorRect.height/2.0f);
        scorFinalText.setPosition(1920 / 2.0f, 1080 / 2.0f + 50.0f); // Putintel mai jos de centru

        // 4. Desenare
        window_.draw(titluText);
        window_.draw(scorFinalText);

    } else {
        // --- JOC ACTIV (Logica normala) ---

        // 1. BACKGROUND
        window_.setView(window_.getDefaultView());
        window_.draw(bgSprite_);

        // 2. LUMEA JOCULUI
        float targetY = player_->getY();
        if (targetY > INALTIME_FEREASTRA / 2.0f) targetY = INALTIME_FEREASTRA / 2.0f;

        camera_.setCenter(LATIME_FEREASTRA / 2.0f, targetY);
        window_.setView(camera_);

        nivel_.draw(window_);
        player_->draw(window_);

        // 3. HUD SCOR (In timpul jocului)
        window_.setView(window_.getDefaultView());
        window_.draw(scoreText_);
    }

    window_.display();
}

void Joc::gestioneazaVictorie() {
    isVictorie_ = true;
    std::cout << "VICTORIE!" << std::endl;
}

void Joc::gestioneazaInfrangere(const std::string& motiv) {
    isGameOver_=true;
    std::cout << "GAME OVER: " << motiv << std::endl;
}