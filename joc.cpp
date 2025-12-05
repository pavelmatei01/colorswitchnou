#include "joc.h"
#include "elemente.h"
#include "culoare.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string> // Necesar pentru std::to_string

Joc::Joc()
    : window_(sf::VideoMode(1920, 1080), "Color Switch", sf::Style::Close | sf::Style::Titlebar),
      score_(0),
      isGameOver_(false),
      isVictorie_(false)
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
        std::cerr << "EROARE: Nu s-a gasit helvetica.ttf! Asigura-te ca fisierul este in folderul proiectului." << std::endl;
    }

    scoreText_.setFont(font_);
    scoreText_.setCharacterSize(50);
    scoreText_.setFillColor(sf::Color::White);
    scoreText_.setPosition(30.f, 30.f); // Stanga sus
    scoreText_.setString("0"); // Pornim de la 0

    // --- 3. RESURSE STATICE ---
    Stea::incarcaTextura();

    // Player - Culoare initiala random
    int randomColorIndex = std::rand() % 4;
    Culoare culoareInitiala(static_cast<TipCuloare>(randomColorIndex));
    player_ = std::make_unique<Player>(800.0f, culoareInitiala);

    initializareNivel();
}

void Joc::initializareNivel() {
    int numarObstacole = 10;
    float distantaIntreObstacole = 700.0f;
    float yCurent = 300.0f;

    // Tine minte ce culoare are jucatorul (sau va avea dupa un schimbator)
    // pentru a evita generarea aceleiasi culori la urmatorul schimbator.
    TipCuloare culoarePrezisa = player_->getCuloare().getTip();

    for (int i = 0; i < numarObstacole; ++i) {
        // Generare Obstacol Random
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

        // Logică pentru elementele dintre obstacole (Stele si Schimbatoare)
        bool steaLaMijloc = (std::rand() % 2 == 0);

        // La obstacolul Banda (tip 4), preferam stea la mijloc pentru ca e greu de pus schimbator
        if (tipObstacol == 4) steaLaMijloc = true;

        if (i < numarObstacole - 1) {
            float yMijloc = yCurent - (distantaIntreObstacole / 2.0f);

            if (steaLaMijloc) {
                // Doar o stea intre obstacole
                nivel_.adaugaElement(std::make_unique<Stea>(yMijloc));
            } else {
                // Stea langa obstacol + Schimbator la mijloc
                nivel_.adaugaElement(std::make_unique<Stea>(yCurent));

                // Creăm schimbătorul folosind culoarea pe care vrem să o evităm (cea curentă)
                auto schimbator = std::make_unique<SchimbatorCuloare>(yMijloc, culoarePrezisa);

                // === FIX: Actualizăm culoarea prezisă cu CULOAREA NOUĂ din schimbător ===
                // Folosim getCuloare() din SchimbatorCuloare, nu getCuloareObiect() din bază
                culoarePrezisa = schimbator->getCuloare().getTip();

                nivel_.adaugaElement(std::move(schimbator));
            }
        } else {
            // Ultimul obstacol - punem o stea deasupra lui dacă nu a fost deja pusă
            if (!steaLaMijloc) nivel_.adaugaElement(std::make_unique<Stea>(yCurent));
        }

        yCurent -= distantaIntreObstacole;
    }

    // Linia de sosire
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
            // Iesire pe Escape
            if (event.key.code == sf::Keyboard::Escape) {
                window_.close();
            }

            // Actiune pe SPACE
            if (event.key.code == sf::Keyboard::Space) {
                if (!isGameOver_ && !isVictorie_) {
                    // Joc Activ: Saritura
                    player_->saritura();
                } else {
                    // Game Over / Victorie: Inchide jocul la apasarea SPACE
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
        // Nivelul returnează punctele acumulate în acest frame (ex: a atins o stea)
        int puncteNoi = nivel_.update(*player_);

        if (puncteNoi > 0) {
            score_ += puncteNoi;
            scoreText_.setString(std::to_string(score_));
        }

    } catch (const NotificareVictorie&) {
        gestioneazaVictorie();
    } catch (const EroareLogica& e) {
        gestioneazaInfrangere(e.what());
    }
}

void Joc::render() {
    window_.clear(sf::Color::Black); // Fundal negru

    if (isGameOver_ || isVictorie_) {
        // --- ECRAN FINAL (Game Over sau Victorie) ---

        // 1. Resetam camera pentru a desena textul static pe ecran
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
        titluText.setPosition(LATIME_FEREASTRA / 2.0f, INALTIME_FEREASTRA / 2.0f - 50.0f);

        // 3. Configuram Textul Scorului Final
        sf::Text scorFinalText;
        scorFinalText.setFont(font_);
        scorFinalText.setCharacterSize(50);
        scorFinalText.setFillColor(sf::Color::White);
        scorFinalText.setString("Scor Final: " + std::to_string(score_));

        // Centrare Scor
        sf::FloatRect scorRect = scorFinalText.getLocalBounds();
        scorFinalText.setOrigin(scorRect.left + scorRect.width/2.0f, scorRect.top + scorRect.height/2.0f);
        scorFinalText.setPosition(LATIME_FEREASTRA / 2.0f, INALTIME_FEREASTRA / 2.0f + 50.0f);

        // 4. Desenare
        window_.draw(titluText);
        window_.draw(scorFinalText);

    } else {
        // --- JOC ACTIV ---

        // 1. BACKGROUND (Static, nu se misca cu camera)
        window_.setView(window_.getDefaultView());
        window_.draw(bgSprite_);

        // 2. LUMEA JOCULUI (Camera urmareste jucatorul)
        float targetY = player_->getY();
        // Camera nu coboara sub jumatatea ecranului (doar urca)
        if (targetY > INALTIME_FEREASTRA / 2.0f) targetY = INALTIME_FEREASTRA / 2.0f;

        camera_.setCenter(LATIME_FEREASTRA / 2.0f, targetY);
        window_.setView(camera_);

        nivel_.draw(window_);
        player_->draw(window_);

        // 3. HUD SCOR (Static, deasupra lumii)
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
    isGameOver_ = true;
    std::cout << "GAME OVER: " << motiv << std::endl;
}