#include "joc.h"
#include "elemente.h"
#include "factory.h"
#include "culoare.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>

Joc::Joc()
    : window_(sf::VideoMode(1920, 1080), "Color Switch", sf::Style::Close | sf::Style::Titlebar),
      isPaused_(false),
      scorStat_("SCOR"),
      distantaStat_("DISTANTA"),
      isGameOver_(false),
      isVictorie_(false),
      nivelCurent_(1)
{
    window_.setFramerateLimit(60);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    scorStat_.adaugaObservator(&achievementSystem_);

    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    int x = (desktop.width - LATIME_FEREASTRA) / 2;
    int y = (desktop.height - INALTIME_FEREASTRA) / 2;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    window_.setPosition(sf::Vector2i(x, y));
    window_.requestFocus();

    camera_.setSize(LATIME_FEREASTRA, INALTIME_FEREASTRA);
    camera_.setCenter(LATIME_FEREASTRA / 2.0f, INALTIME_FEREASTRA / 2.0f);

    if (bgTexture_.loadFromFile("assets/background.jpg")) {
        bgSprite_.setTexture(bgTexture_);
        sf::Vector2u marimeImg = bgTexture_.getSize();
        bgSprite_.setScale(1920.0f / marimeImg.x, 1080.0f / marimeImg.y);
    }

    if (!font_.loadFromFile("assets/helvetica.ttf")) {
        std::cerr << "EROARE: Nu s-a gasit helvetica.ttf!" << std::endl;
    }

    scoreText_.setFont(font_);
    scoreText_.setCharacterSize(50);
    scoreText_.setFillColor(sf::Color::White);
    scoreText_.setPosition(30.f, 30.f);
    scoreText_.setString("0");

    pauseText_.setFont(font_);
    pauseText_.setCharacterSize(100);
    pauseText_.setFillColor(sf::Color::White);
    pauseText_.setString("PAUZA");
    sf::FloatRect pr = pauseText_.getLocalBounds();
    pauseText_.setOrigin(pr.left + pr.width/2.0f, pr.top + pr.height/2.0f);
    pauseText_.setPosition(LATIME_FEREASTRA / 2.0f, INALTIME_FEREASTRA / 2.0f - 50.0f);

    resumeText_.setFont(font_);
    resumeText_.setCharacterSize(40);
    resumeText_.setFillColor(sf::Color(200, 200, 200));
    resumeText_.setString("Apasa ESC pentru a continua");
    sf::FloatRect rr = resumeText_.getLocalBounds();
    resumeText_.setOrigin(rr.left + rr.width/2.0f, rr.top + rr.height/2.0f);
    resumeText_.setPosition(LATIME_FEREASTRA / 2.0f, INALTIME_FEREASTRA / 2.0f + 50.0f);


    pauseOverlay_.setSize(sf::Vector2f(LATIME_FEREASTRA, INALTIME_FEREASTRA));
    pauseOverlay_.setFillColor(sf::Color(0, 0, 0, 150)); // Alpha 150

    nivelText_.setFont(font_);
    nivelText_.setCharacterSize(40);
    nivelText_.setFillColor(sf::Color::Yellow);
    nivelText_.setPosition(30.f, 90.f); // Sub scor
    nivelText_.setString("Nivel: 1");

    Stea::incarcaTextura();
    int randomColorIndex = std::rand() % 4;
    Culoare culoareInitiala(static_cast<TipCuloare>(randomColorIndex));
    player_ = std::make_unique<Player>(800.0f, culoareInitiala);

    initializareNivel();
}

void Joc::initializareNivel() {
    nivel_.goleste();
    int numarObstacole = 8 + (nivelCurent_ - 1) * 4;
    float viteza = 1.5f + (nivelCurent_ - 1) * 0.5f;
    ObiectJoc::setViteza(viteza);
    std::cout << "--- Generare Nivel " << nivelCurent_ << " ---" << std::endl;
    std::cout << "Obstacole: " << numarObstacole << " | Viteza: " << viteza << std::endl;
    nivelText_.setString("Nivel: " + std::to_string(nivelCurent_));
    player_->setPozitie(800.0f);
    camera_.setCenter(LATIME_FEREASTRA / 2.0f, INALTIME_FEREASTRA / 2.0f);
    float distantaIntreObstacole = 700.0f;
    float yCurent = 300.0f;
    TipCuloare culoarePrezisa = player_->getCuloare().getTip();
    for (int i = 0; i < numarObstacole; ++i) {
        int tipObstacol = std::rand() % 5;
        nivel_.adaugaElement(ObstacolFactory::creeazaObstacol(tipObstacol, yCurent));

        bool steaLaMijloc = (std::rand() % 2 == 0);
        if (tipObstacol == 4) steaLaMijloc = true;

        if (i < numarObstacole - 1) {
            float yMijloc = yCurent - (distantaIntreObstacole / 2.0f);
            if (steaLaMijloc) {
                nivel_.adaugaElement(std::make_unique<Stea>(yMijloc));
            } else {
                int sansaPowerUp = std::rand() % 100;
                if (sansaPowerUp<35) {
                    nivel_.adaugaElement(std::make_unique<PowerUpMulticolor>(yMijloc));
                    std::cout <<"Powerup Multicolor la " << yMijloc << std::endl;
                }
                else {
                    nivel_.adaugaElement(std::make_unique<Stea>(yCurent));
                    auto schimbator = std::make_unique<SchimbatorCuloare>(yMijloc, culoarePrezisa);
                    culoarePrezisa = schimbator->getCuloare().getTip();
                    nivel_.adaugaElement(std::move(schimbator));
                }

            }
        } else {
            if (!steaLaMijloc) nivel_.adaugaElement(std::make_unique<Stea>(yCurent));
        }
        yCurent -= distantaIntreObstacole;
    }
    nivel_.adaugaElement(std::make_unique<LinieSosire>(yCurent - 300.0f));
}

void Joc::trecereNivelUrmator() {
    nivelCurent_++;
    initializareNivel();
}

void Joc::run() {
    while (window_.isOpen()) {
        procesareEvenimente();
        if (!isGameOver_ && !isVictorie_&&!isPaused_) update();
        render();
    }
}

void Joc::procesareEvenimente() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window_.close();
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                isPaused_ = !isPaused_;
            };
            if (event.key.code == sf::Keyboard::Space) {
                if (!isGameOver_ && !isVictorie_&&!isPaused_) player_->saritura();
                else window_.close();
            }
        }
    }
}

void Joc::update() {
    try {
        player_->update();
        distantaStat_.setValoare(800.0f - player_->getY()); // Doar statistic, nu afecteaza gameplay

        int puncteNoi = nivel_.update(*player_);
        if (puncteNoi > 0) {
            scorStat_.adauga(puncteNoi);
            scoreText_.setString("Scor: " + std::to_string(scorStat_.getCurent()));
        }
    } catch (const NotificareVictorie& e) {
        // --- LOGICA DE TRANZITIE ---
        if (nivelCurent_ < MAX_NIVELE) {
            std::cout << "Nivel complet! Trecem la nivelul urmator." << std::endl;
            trecereNivelUrmator();
        } else {
            std::cout << e.what() << std::endl;
            gestioneazaVictorie();
        }
    } catch (const EroareLogica& e) {
        gestioneazaInfrangere(e.what());
    }
}

void Joc::render() {
    window_.clear(sf::Color::Black);
    if (isGameOver_ || isVictorie_) {
        window_.setView(window_.getDefaultView());
        sf::Text titluText;
        titluText.setFont(font_);
        titluText.setCharacterSize(80);
        titluText.setStyle(sf::Text::Bold);

        if (isVictorie_) {
            titluText.setString("AI CASTIGAT!");
            titluText.setFillColor(sf::Color::Green);
        } else {
            titluText.setString("GAME OVER");
            titluText.setFillColor(sf::Color::Red);
        }

        sf::FloatRect textRect = titluText.getLocalBounds();
        titluText.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
        titluText.setPosition(LATIME_FEREASTRA / 2.0f, INALTIME_FEREASTRA / 2.0f - 50.0f);

        sf::Text scorFinalText;
        scorFinalText.setFont(font_);
        scorFinalText.setCharacterSize(50);
        scorFinalText.setFillColor(sf::Color::White);
        scorFinalText.setString("Scor Final: " + std::to_string(scorStat_.getCurent()));

        sf::FloatRect scorRect = scorFinalText.getLocalBounds();
        scorFinalText.setOrigin(scorRect.left + scorRect.width/2.0f, scorRect.top + scorRect.height/2.0f);
        scorFinalText.setPosition(LATIME_FEREASTRA / 2.0f, INALTIME_FEREASTRA / 2.0f + 50.0f);

        window_.draw(titluText);
        window_.draw(scorFinalText);
    } else {
        window_.setView(window_.getDefaultView());
        window_.draw(bgSprite_);
        float targetY = player_->getY();
        if (targetY > INALTIME_FEREASTRA / 2.0f) targetY = INALTIME_FEREASTRA / 2.0f;

        // Nu lasam camera sa urce prea sus daca jucatorul a fost resetat jos
        // dar deoarece resetam coordonata Y a jucatorului la 800, targetY va fi corect.

        camera_.setCenter(LATIME_FEREASTRA / 2.0f, targetY);
        window_.setView(camera_);
        nivel_.draw(window_);
        player_->draw(window_);

        // Desenare UI
        window_.setView(window_.getDefaultView());
        window_.draw(scoreText_);
        window_.draw(nivelText_);
        if (isPaused_) {
            window_.draw(pauseOverlay_);
            window_.draw(pauseText_);
            window_.draw(resumeText_);
        }
    }
    window_.display();
}

void Joc::gestioneazaVictorie() { isVictorie_ = true; }
void Joc::gestioneazaInfrangere(const std::string& motiv) {
    isGameOver_ = true;
    std::cout << motiv << std::endl;
}