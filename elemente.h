#pragma once
#include "obiectjoc.h"
#include <vector>
// --- Obstacol Rotativ Simplu ---
class ObstacolRotativ : public ObiectJoc {
    float razaMedie_, grosime_, rotatie_;
    std::vector<Culoare> segmenteCulori_;
public:
    explicit ObstacolRotativ(float y);  // ADDED: explicit
    std::unique_ptr<ObiectJoc> clone() const override;
    void update() override;
    bool interactioneaza(Player& player) override;
    void draw(sf::RenderWindow& window) const override;
private:
    void afisare(std::ostream& os) const override;
};
// --- Obstacol Dublu Rotativ ---
class ObstacolDublu : public ObiectJoc {
    float razaInt_, razaExt_, grosime_, rotInt_, rotExt_;
    std::vector<Culoare> culoriInt_, culoriExt_;
public:
    explicit ObstacolDublu(float y);  // ADDED: explicit
    std::unique_ptr<ObiectJoc> clone() const override;
    void update() override;
    bool interactioneaza(Player& player) override;
    void draw(sf::RenderWindow& window) const override;
private:
    void afisare(std::ostream& os) const override;
    void verificaInel(const Player& player, float raza, float rotatie, const std::vector<Culoare>& culori);  // CHANGED: const Player&
};
// --- Stea (Puncte) ---
class Stea : public ObiectJoc {
    static sf::Texture textureStea;
    sf::Sprite sprite_;
public:
    static void incarcaTextura();
    explicit Stea(float y);  // ADDED: explicit
    int getPuncte() const override { return 1; }
    std::unique_ptr<ObiectJoc> clone() const override;
    void update() override;
    bool interactioneaza(Player& player) override;
    void draw(sf::RenderWindow& window) const override;
private:
    void afisare(std::ostream& os) const override;
};
class ObstacolPatrat : public ObiectJoc {
    float latura_;
    float grosime_;
    float rotatie_;
    std::vector<Culoare> segmenteCulori_;
public:
    explicit ObstacolPatrat(float y);  // ADDED: explicit
    std::unique_ptr<ObiectJoc> clone() const override;
    void update() override;
    bool interactioneaza(Player& player) override;
    void draw(sf::RenderWindow& window) const override;
private:
    void afisare(std::ostream& os) const override;
    sf::Vector2f rotestePunct(sf::Vector2f punct, float unghiGrade) const;
};
class ObstacolDouaElice : public ObiectJoc {
    float raza_;
    float grosime_;
    float rotatieStanga_;
    float rotatieDreapta_;
    sf::Vector2f centruStanga_;
    sf::Vector2f centruDreapta_;
    std::vector<Culoare> culoriStanga_;
    std::vector<Culoare> culoriDreapta_;
public:
    explicit ObstacolDouaElice(float y);  // ADDED: explicit
    std::unique_ptr<ObiectJoc> clone() const override;
    void update() override;
    bool interactioneaza(Player& player) override;
    void draw(sf::RenderWindow& window) const override;
private:
    void afisare(std::ostream& os) const override;
    sf::Vector2f rotestePunct(sf::Vector2f punct, float unghiGrade) const;
    void verificaElice(const Player& p, sf::Vector2f centru, float rotatie, const std::vector<Culoare>& culori);  // CHANGED: const Player&
    void deseneazaElice(sf::RenderWindow& w, sf::Vector2f centru, float rotatie, const std::vector<Culoare>& culori) const;
};
// --- Schimbator Culoare ---
class SchimbatorCuloare : public ObiectJoc {
    // CHANGED: Renamed to avoid dupe with base (base is MULTICOLOR, this is the new color)
    Culoare culoareNoua_;
public:
    SchimbatorCuloare(float y, TipCuloare culoareDeEvitat);
    SchimbatorCuloare(const SchimbatorCuloare& other);  // Copy ctor
    const Culoare& getCuloare() const { return culoareNoua_; }  // Accessor
    std::unique_ptr<ObiectJoc> clone() const override;
    void update() override;
    bool interactioneaza(Player& player) override;
    void draw(sf::RenderWindow& window) const override;
private:
    void afisare(std::ostream& os) const override;
};
class ObstacolBanda : public ObiectJoc {
    float latimeSegment_;
    float inaltime_;
    float offsetX_;
    float vitezaDirectie_;
    std::vector<Culoare> modelCulori_;
public:
    explicit ObstacolBanda(float y, bool spreDreapta);  // ADDED: explicit (2 args, but good practice)
    std::unique_ptr<ObiectJoc> clone() const override;
    void update() override;
    bool interactioneaza(Player& player) override;
    void draw(sf::RenderWindow& window) const override;
private:
    void afisare(std::ostream& os) const override;
};
// --- Linie Sosire ---
class LinieSosire : public ObiectJoc {
    float latime_;
public:
    explicit LinieSosire(float y);
    std::unique_ptr<ObiectJoc> clone() const override;
    void update() override;
    bool interactioneaza(Player& player) override;
    void draw(sf::RenderWindow& window) const override;
private:
    void afisare(std::ostream& os) const override;
};