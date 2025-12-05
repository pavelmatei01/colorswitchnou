#include "elemente.h"
#include "player.h"
#include "obstacol.h"
#include <cmath>
#include <iostream>
#include <cstdlib>  // For std::rand()

ObstacolRotativ::ObstacolRotativ(float y)
    : ObiectJoc(y, Culoare(TipCuloare::MULTICOLOR)), razaMedie_(100.f), grosime_(20.f), rotatie_(0.f) {
    segmenteCulori_ = { Culoare(TipCuloare::ROSU), Culoare(TipCuloare::ALBASTRU), Culoare(TipCuloare::VERDE), Culoare(TipCuloare::GALBEN) };
}
std::unique_ptr<ObiectJoc> ObstacolRotativ::clone() const {
    return std::make_unique<ObstacolRotativ>(*this);
}
void ObstacolRotativ::update() {
    rotatie_ += vitezaRotatieGlobala;
    if (rotatie_ > 360.f) rotatie_ -= 360.f;
}
bool ObstacolRotativ::interactioneaza(Player& player) {
    // Verificam inelul
    float rInt = razaMedie_ - grosime_/2;
    float rExt = razaMedie_ + grosime_/2;
    float distY = std::abs(player.getY() - y_);
    // Coliziune doar daca suntem in grosimea inelului
    if (distY > rInt - player.getRaza() && distY < rExt + player.getRaza()) {
        float dy = player.getY() - y_;
        // 960 este centrul X, dar calculam atan2 doar pe dy (dx=0 virtual)
        float unghiGrade = std::atan2(dy, 0.f) * 180.f / M_PI;
        if (unghiGrade < 0) unghiGrade += 360.f;
        float unghiLocal = unghiGrade - rotatie_;
        while (unghiLocal < 0) unghiLocal += 360.f;
        while (unghiLocal >= 360.f) unghiLocal -= 360.f;
        int index = static_cast<int>(unghiLocal / 90.f) % 4;
        if (!player.getCuloare().sePotriveste(segmenteCulori_[index])) {
            throw EroareLogica("Game Over: Culoare gresita la Cerc!");
        }
    }
    return false;
}
void ObstacolRotativ::draw(sf::RenderWindow& window) const {
    sf::VertexArray va(sf::TriangleStrip);
    sf::Vector2f centru(960.f, y_);
    for(int i=0; i<4; ++i) {
        adaugaSegmentArc(va, centru, razaMedie_-grosime_/2, razaMedie_+grosime_/2,
                        i*90.f, i*90.f+91.f, segmenteCulori_[i].getSFMLColor(), rotatie_);
    }
    window.draw(va);
}
void ObstacolRotativ::afisare(std::ostream& os) const { os << "Obstacol Rotativ la " << y_; }

ObstacolDublu::ObstacolDublu(float y)
    : ObiectJoc(y, Culoare(TipCuloare::MULTICOLOR)),
      razaInt_(100.0f), razaExt_(140.0f), grosime_(15.0f), rotInt_(0.0f), rotExt_(0.0f)
{
    // Sincronizam culorile pentru efect vizual placut
    culoriInt_ = { Culoare(TipCuloare::ROSU), Culoare(TipCuloare::ALBASTRU), Culoare(TipCuloare::VERDE), Culoare(TipCuloare::GALBEN) };
    culoriExt_ = culoriInt_;
}
std::unique_ptr<ObiectJoc> ObstacolDublu::clone() const {
    return std::make_unique<ObstacolDublu>(*this);
}
void ObstacolDublu::update() {
    rotInt_ += vitezaRotatieGlobala * 0.8f; // Interior mai lent, spre dreapta
    rotExt_ -= vitezaRotatieGlobala * 1.2f; // Exterior mai rapid, spre stanga
    if(rotInt_ > 360.0f) rotInt_ -= 360.0f;
    if(rotExt_ < 0.0f) rotExt_ += 360.0f;
}
void ObstacolDublu::verificaInel(const Player& player, float raza, float rotatie, const std::vector<Culoare>& culori) {
    float rInt = raza - grosime_/2;
    float rExt = raza + grosime_/2;
    float distY = std::abs(player.getY() - y_);
    if (distY > rInt - player.getRaza() && distY < rExt + player.getRaza()) {
        float dy = player.getY() - y_;
        float unghiGrade = std::atan2(dy, 0.f) * 180.f / M_PI;
        if (unghiGrade < 0) unghiGrade += 360.f;
        float unghiLocal = unghiGrade - rotatie;
        while (unghiLocal < 0) unghiLocal += 360.f;
        while (unghiLocal >= 360.f) unghiLocal -= 360.f;
        int index = static_cast<int>(unghiLocal / 90.f) % 4;
        if (!player.getCuloare().sePotriveste(culori[index])) {
            throw EroareLogica("Game Over: Culoare gresita la Inel Dublu!");
        }
    }
}
bool ObstacolDublu::interactioneaza(Player& player) {
    verificaInel(player, razaInt_, rotInt_, culoriInt_);
    verificaInel(player, razaExt_, rotExt_, culoriExt_);
    return false;
}
void ObstacolDublu::draw(sf::RenderWindow& window) const {
    sf::VertexArray va(sf::TriangleStrip);
    sf::Vector2f c(960.f, y_);
    // Exterior
    for(int i=0; i<4; ++i) adaugaSegmentArc(va, c, razaExt_-grosime_/2, razaExt_+grosime_/2, i*90.f, i*90.f+91.f, culoriExt_[i].getSFMLColor(), rotExt_);
    window.draw(va);
    va.clear();
    // Interior
    for(int i=0; i<4; ++i) adaugaSegmentArc(va, c, razaInt_-grosime_/2, razaInt_+grosime_/2, i*90.f, i*90.f+91.f, culoriInt_[i].getSFMLColor(), rotInt_);
    window.draw(va);
}
void ObstacolDublu::afisare(std::ostream& os) const { os << "Obstacol Dublu la " << y_; }

// 1. Definirea variabilei statice (obligatoriu in .cpp)
sf::Texture Stea::textureStea;
// 2. Implementarea functiei de incarcare
void Stea::incarcaTextura() {
    // Asigura-te ca "star.png" este langa executabil!
    if (!textureStea.loadFromFile("star.png")) {
        // Fallback: Daca nu gaseste imaginea, macar nu crapa, dar va arata un patrat alb
        std::cerr << "EROARE CRITICA: Nu s-a gasit star.png!" << std::endl;
    }
    // Activeaza filtrarea liniara pentru ca steaua sa nu para pixelata cand se roteste
    textureStea.setSmooth(true);
}
Stea::Stea(float y) : ObiectJoc(y, Culoare(TipCuloare::GALBEN)) {
    sprite_.setTexture(textureStea);
    sf::Vector2u marimeImagine = textureStea.getSize();
    sprite_.setOrigin(marimeImagine.x / 2.0f, marimeImagine.y / 2.0f);
    // === MODIFICARE DIMENSIUNE ===
    // SchimbatorCuloare are raza 15.f => Diametru 30.f
    // Setam tinta la 30.0f ca sa fie identice vizual.
    float dimensiuneTinta = 50.0f;
    // Calculam scara
    float factorScara = dimensiuneTinta / static_cast<float>(marimeImagine.x);
    sprite_.setScale(factorScara, factorScara);
    sprite_.setPosition(960.f, y_);
}
std::unique_ptr<ObiectJoc> Stea::clone() const { return std::make_unique<Stea>(*this); }
void Stea::update() {
    // Efect vizual: rotim usor steaua in fiecare cadru
    sprite_.rotate(2.0f);
}
bool Stea::interactioneaza(Player& player) {
    // Coliziune simpla pe baza distantei (raza de colectare aprox 30 pixeli)
    if (std::abs(player.getY() - y_) < 35.0f) {
        std::cout << " *** Stea Colectata ***" << std::endl;
        return true;
    }
    return false;
}
void Stea::draw(sf::RenderWindow& window) const {
    // 4. Desenam sprite-ul in loc de forma geometrica veche
    window.draw(sprite_);
}
void Stea::afisare(std::ostream& os) const { os << "Stea (Sprite) la " << y_; }

ObstacolPatrat::ObstacolPatrat(float y)
    : ObiectJoc(y, Culoare(TipCuloare::MULTICOLOR)), latura_(180.0f), grosime_(20.0f), rotatie_(0.0f) {
    segmenteCulori_ = { Culoare(TipCuloare::ROSU), Culoare(TipCuloare::ALBASTRU), Culoare(TipCuloare::VERDE), Culoare(TipCuloare::GALBEN) };
}
std::unique_ptr<ObiectJoc> ObstacolPatrat::clone() const {
    return std::make_unique<ObstacolPatrat>(*this);
}
void ObstacolPatrat::update() {
    rotatie_ += vitezaRotatieGlobala;
    if (rotatie_ > 360.0f) rotatie_ -= 360.0f;
}
sf::Vector2f ObstacolPatrat::rotestePunct(sf::Vector2f punct, float unghiGrade) const {
    float rad = unghiGrade * M_PI / 180.f;
    float c = std::cos(rad);
    float s = std::sin(rad);
    return sf::Vector2f(punct.x * c - punct.y * s, punct.x * s + punct.y * c);
}
bool ObstacolPatrat::interactioneaza(Player& player) {
    // 1. Verificare bounding box general (sa nu calculam degeaba daca e departe)
    float halfL = latura_ / 2.0f;
    float maxDist = (halfL + grosime_) * 1.5f;
    if (std::abs(player.getY() - y_) > maxDist + player.getRaza()) return false;
    // 2. Aducem playerul in sistemul local al patratului (ca si cum patratul nu s-ar roti)
    float dy = player.getY() - y_;
    float dx = 0.0f; // Player e centrat pe X
    // Rotim invers pozitia jucatorului cu unghiul obstacolului
    sf::Vector2f posLocal = rotestePunct(sf::Vector2f(dx, dy), -rotatie_);
    float absX = std::abs(posLocal.x);
    float absY = std::abs(posLocal.y);
    float r = player.getRaza();
    // Limitele peretilor patratului (fara rotatie)
    float inner = halfL;
    float outer = halfL + grosime_;
    // Verificam coliziunea cu peretii "indreptati"
    bool lovit = false;
    int indexLatura = -1;
    // Indexurile laturilor in sistem local:
    // 0 = Sus (y negativ), 1 = Dreapta (x pozitiv), 2 = Jos (y pozitiv), 3 = Stanga (x negativ)
    // Dar atentie la coordonatele SFML (Y creste in jos).
    // Aici, segmenteCulori_ sunt: 0=Rosu, 1=Albastru, 2=Verde, 3=Galben.
    // Trebuie sa mapam geometria la culori.
    // Verificare pereti Verticali (Stanga/Dreapta)
    if (absY < outer + r && absX > inner - r && absX < outer + r) {
        lovit = true;
        // Determinare latura exacta
        if (posLocal.x > 0) indexLatura = 1; // Dreapta
        else indexLatura = 3; // Stanga
    }
    // Verificare pereti Orizontali (Sus/Jos)
    if (absX < outer + r && absY > inner - r && absY < outer + r) {
        lovit = true;
        if (posLocal.y > 0) indexLatura = 2; // Jos
        else indexLatura = 0; // Sus
    }
    if (lovit && indexLatura != -1) {
        if (!player.getCuloare().sePotriveste(segmenteCulori_[indexLatura])) {
             throw EroareLogica("Game Over: Coliziune cu Patrat!");
        }
    }
    return false;
}
void ObstacolPatrat::draw(sf::RenderWindow& window) const {
    sf::VertexArray va(sf::Quads);
    sf::Vector2f centru(960.f, y_);
    float H = latura_ / 2.0f;
    float G = grosime_;
    // Puncte locale pentru un segment (latura de sus, index 0)
    // Apoi rotim cu 90 grade pentru urmatoarele
    for(int i=0; i<4; ++i) {
        sf::Color col = segmenteCulori_[i].getSFMLColor();
        // Definim o latura ca un dreptunghi local
        // Latura 0 (Sus): X de la -H-G la H+G, Y de la -H-G la -H
        // Ajustam coordonatele pentru a forma "rama" corect
        // Metoda simpla: Desenam 4 dreptunghiuri rotite
        std::vector<sf::Vector2f> pct(4);
        // Coordonate pentru latura "TOP" in spatiul local
        pct[0] = {-H - G, -H - G};
        pct[1] = {H + G, -H - G};
        pct[2] = {H + G, -H};
        pct[3] = {-H - G, -H};
        // Rotim punctele cu 90 * i grade + rotatie_
        float unghiLatura = i * 90.0f;
        for(auto& p : pct) {
            // Rotire pentru pozitionarea laturii (0, 90, 180, 270)
            p = rotestePunct(p, unghiLatura);
            // Rotire globala a obiectului
            p = rotestePunct(p, rotatie_);
            // Translatie
            p += centru;
            va.append(sf::Vertex(p, col));
        }
    }
    window.draw(va);
}
void ObstacolPatrat::afisare(std::ostream& os) const { os << "Obstacol Patrat la " << y_; }

ObstacolBanda::ObstacolBanda(float y, bool spreDreapta)
    : ObiectJoc(y, Culoare(TipCuloare::MULTICOLOR)),
      latimeSegment_(400.0f), inaltime_(40.0f), offsetX_(0.0f)
{
    if (spreDreapta) {
        vitezaDirectie_ = 4.0f;
        modelCulori_ = { Culoare(TipCuloare::ROSU), Culoare(TipCuloare::VERDE), Culoare(TipCuloare::ALBASTRU), Culoare(TipCuloare::GALBEN) };
    } else {
        vitezaDirectie_ = -4.0f;
        modelCulori_ = { Culoare(TipCuloare::VERDE), Culoare(TipCuloare::ALBASTRU), Culoare(TipCuloare::ROSU), Culoare(TipCuloare::GALBEN) };
    }
}
std::unique_ptr<ObiectJoc> ObstacolBanda::clone() const { return std::make_unique<ObstacolBanda>(*this); }
void ObstacolBanda::update() {
    offsetX_ += vitezaDirectie_ * vitezaRotatieGlobala;
    float lungimeTotala = modelCulori_.size() * latimeSegment_;
    // Mentinem offsetul in interval pozitiv [0, Total]
    while (offsetX_ >= lungimeTotala) offsetX_ -= lungimeTotala;
    while (offsetX_ < 0.0f) offsetX_ += lungimeTotala;
}
bool ObstacolBanda::interactioneaza(Player& player) {
    // 1. Coliziune Y (inaltime)
    if (player.getY() + player.getRaza() < y_ - inaltime_/2.0f ||
        player.getY() - player.getRaza() > y_ + inaltime_/2.0f) {
        return false;
    }
    // 2. Calcul index culoare
    float lungimeTotala = modelCulori_.size() * latimeSegment_;
    // Pozitia virtuala pe banda infinita
    // 960 este pozitia jucatorului. Scadem offsetul benzii.
    float pozitiePeBanda = 960.0f - offsetX_;
    // Normalizare modulo
    while (pozitiePeBanda < 0.0f) pozitiePeBanda += lungimeTotala;
    while (pozitiePeBanda >= lungimeTotala) pozitiePeBanda -= lungimeTotala;
    int index = static_cast<int>(pozitiePeBanda / latimeSegment_);
    // Safety check
    if (index < 0) index = 0;
    if (index >= (int)modelCulori_.size()) index = (int)modelCulori_.size() - 1;
    if (!player.getCuloare().sePotriveste(modelCulori_[index])) {
        throw EroareLogica("Game Over: Banda Rulanta!");
    }
    return false;
}
void ObstacolBanda::draw(sf::RenderWindow& window) const {
    float lungimeTotala = modelCulori_.size() * latimeSegment_;
    // Desenam 3 copii pentru acoperire perfecta
    for (int k = -1; k <= 1; ++k) {
        float bazaX = k * lungimeTotala + offsetX_;
        // Optimizare clipping
        if (bazaX > 1920.0f || bazaX + lungimeTotala < 0.0f) continue;
        for (size_t i = 0; i < modelCulori_.size(); ++i) {
            sf::RectangleShape rect(sf::Vector2f(latimeSegment_, inaltime_));
            rect.setFillColor(modelCulori_[i].getSFMLColor());
            rect.setOrigin(0.0f, inaltime_ / 2.0f);
            rect.setPosition(bazaX + i * latimeSegment_, y_);
            window.draw(rect);
        }
    }
}
void ObstacolBanda::afisare(std::ostream& os) const { os << "Banda la " << y_; }

ObstacolDouaElice::ObstacolDouaElice(float y)
    : ObiectJoc(y, Culoare(TipCuloare::MULTICOLOR)),
      raza_(180.0f), grosime_(20.0f), rotatieStanga_(0.0f), rotatieDreapta_(0.0f)
{
    centruStanga_ = sf::Vector2f(960.0f - raza_, y);
    centruDreapta_ = sf::Vector2f(960.0f + raza_, y);
    culoriStanga_ = { Culoare(TipCuloare::ROSU), Culoare(TipCuloare::ALBASTRU), Culoare(TipCuloare::VERDE), Culoare(TipCuloare::GALBEN) };
    culoriDreapta_ = { culoriStanga_[2], culoriStanga_[1], culoriStanga_[0], culoriStanga_[3] };
}
std::unique_ptr<ObiectJoc> ObstacolDouaElice::clone() const { return std::make_unique<ObstacolDouaElice>(*this); }
void ObstacolDouaElice::update() {
    rotatieStanga_ += vitezaRotatieGlobala;
    if (rotatieStanga_ > 360.0f) rotatieStanga_ -= 360.0f;
    rotatieDreapta_ -= vitezaRotatieGlobala;
    if (rotatieDreapta_ < 0.0f) rotatieDreapta_ += 360.0f;
}
sf::Vector2f ObstacolDouaElice::rotestePunct(sf::Vector2f punct, float unghiGrade) const {
    float rad = unghiGrade * M_PI / 180.f;
    float c = std::cos(rad);
    float s = std::sin(rad);
    return sf::Vector2f(punct.x * c - punct.y * s, punct.x * s + punct.y * c);
}
void ObstacolDouaElice::verificaElice(const Player& p, sf::Vector2f centru, float rotatie, const std::vector<Culoare>& culori) {
    if (std::abs(p.getY() - centru.y) > raza_ + p.getRaza() ||
        std::abs(960.0f - centru.x) > raza_ + p.getRaza() + 200.0f) return;
    float dy = p.getY() - centru.y;
    float dx = 960.0f - centru.x;
    sf::Vector2f posLocal = rotestePunct(sf::Vector2f(dx, dy), -rotatie);
    float px = posLocal.x;
    float py = posLocal.y;
    float r = p.getRaza();
    float g = grosime_ / 2.0f;
    int indexLovit = -1;
    // Verificam cele 4 brate (intersectie dreptunghi-cerc simplificata)
    if (px > -r && px < raza_ + r && py > -g - r && py < g + r) indexLovit = 0; // Dreapta
    else if (py > -r && py < raza_ + r && px > -g - r && px < g + r) indexLovit = 1; // Jos
    else if (px < r && px > -raza_ - r && py > -g - r && py < g + r) indexLovit = 2; // Stanga
    else if (py < r && py > -raza_ - r && px > -g - r && px < g + r) indexLovit = 3; // Sus
    if (indexLovit != -1) {
        if (!p.getCuloare().sePotriveste(culori[indexLovit])) {
            throw EroareLogica("Game Over: Doua Elice!");
        }
    }
}
bool ObstacolDouaElice::interactioneaza(Player& player) {
    verificaElice(player, centruStanga_, rotatieStanga_, culoriStanga_);
    verificaElice(player, centruDreapta_, rotatieDreapta_, culoriDreapta_);
    return false;
}
void ObstacolDouaElice::deseneazaElice(sf::RenderWindow& w, sf::Vector2f centru, float rotatie, const std::vector<Culoare>& culori) const {
    sf::VertexArray va(sf::Quads);
    float g = grosime_ / 2.0f;
    float L = raza_;
    for (int i = 0; i < 4; ++i) {
        sf::Color col = culori[i].getSFMLColor();
        std::vector<sf::Vector2f> puncte = { {0.0f, -g}, {L, -g}, {L, g}, {0.0f, g} };
        float unghiTotal = (i * 90.0f) + rotatie;
        for (const auto& p : puncte) {
            sf::Vector2f pFinal = centru + rotestePunct(p, unghiTotal);
            va.append(sf::Vertex(pFinal, col));
        }
    }
    w.draw(va);
}
void ObstacolDouaElice::draw(sf::RenderWindow& window) const {
    deseneazaElice(window, centruStanga_, rotatieStanga_, culoriStanga_);
    deseneazaElice(window, centruDreapta_, rotatieDreapta_, culoriDreapta_);
}
void ObstacolDouaElice::afisare(std::ostream& os) const { os << "2 Elice la " << y_; }

SchimbatorCuloare::SchimbatorCuloare(float y, TipCuloare culoareDeEvitat)
    : ObiectJoc(y, Culoare(TipCuloare::MULTICOLOR)) {
    TipCuloare nouaCuloare;
    do { nouaCuloare = static_cast<TipCuloare>(std::rand() % 4); } while (nouaCuloare == culoareDeEvitat);
    this->culoareNoua_ = Culoare(nouaCuloare);
}
SchimbatorCuloare::SchimbatorCuloare(const SchimbatorCuloare& other)
    : ObiectJoc(other.y_, Culoare(TipCuloare::MULTICOLOR)), culoareNoua_(other.culoareNoua_) {}
std::unique_ptr<ObiectJoc> SchimbatorCuloare::clone() const { return std::make_unique<SchimbatorCuloare>(*this); }
void SchimbatorCuloare::update() {}
bool SchimbatorCuloare::interactioneaza(Player& player) {
    if (std::abs(player.getY() - y_) < 20.0f) {
        player.setCuloare(this->culoareNoua_);
        std::cout << " *** Culoare Schimbata! ***" << std::endl;
        return true;
    }
    return false;
}
void SchimbatorCuloare::draw(sf::RenderWindow& window) const {
    sf::CircleShape shape(15.f);
    shape.setFillColor(culoareNoua_.getSFMLColor());
    shape.setOrigin(15.f, 15.f);
    shape.setPosition(960.f, y_);
    window.draw(shape);
}
void SchimbatorCuloare::afisare(std::ostream& os) const { os << "Schimbator la " << y_; }

LinieSosire::LinieSosire(float y) : ObiectJoc(y, Culoare(TipCuloare::MULTICOLOR)), latime_(1920.f) {}
std::unique_ptr<ObiectJoc> LinieSosire::clone() const { return std::make_unique<LinieSosire>(*this); }
void LinieSosire::update() {}
bool LinieSosire::interactioneaza(Player& player) {
    if (player.getY() <= y_ + 20.0f) throw NotificareVictorie();
    return false;
}
void LinieSosire::draw(sf::RenderWindow& window) const {
    sf::RectangleShape line(sf::Vector2f(latime_, 10.f));
    line.setFillColor(sf::Color::White);
    line.setPosition(0.f, y_);
    window.draw(line);
}
void LinieSosire::afisare(std::ostream& os) const { os << "SOSIRE la " << y_; }