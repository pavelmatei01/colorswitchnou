#pragma once
#include "obiectjoc.h"
#include <vector>

// --- Obstacol Rotativ Simplu ---
class ObstacolRotativ : public ObiectJoc {
    float razaMedie_, grosime_, rotatie_;
    std::vector<Culoare> segmenteCulori_;
public:
    ObstacolRotativ(float y);
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
    ObstacolDublu(float y);
    std::unique_ptr<ObiectJoc> clone() const override;
    void update() override;
    bool interactioneaza(Player& player) override;
    void draw(sf::RenderWindow& window) const override;
private:
    void afisare(std::ostream& os) const override;
    void verificaInel(Player& player, float raza, float rotatie, const std::vector<Culoare>& culori);
};


// --- Stea (Puncte) ---
// --- Stea (Puncte) ---
class Stea : public ObiectJoc {
    // STATIC: Se incarca o singura data pentru TOATE stelele
    static sf::Texture textureStea;

    // Sprite-ul individual al acestei instante (pozitie, rotatie proprie)
    sf::Sprite sprite_;

public:
    // Functie statica pentru a incarca imaginea la pornirea jocului
    static void incarcaTextura();

    Stea(float y);
    int getPuncte() const override { return 1; }
    std::unique_ptr<ObiectJoc> clone() const override;
    void update() override; // Vom folosi update pentru a o roti putin
    bool interactioneaza(Player& player) override;
    void draw(sf::RenderWindow& window) const override;
private:
    void afisare(std::ostream& os) const override;
};
class ObstacolPatrat : public ObiectJoc {
    float latura_;
    float grosime_; // Grosimea liniei
    float rotatie_;
    std::vector<Culoare> segmenteCulori_; // Cele 4 culori
public:
    ObstacolPatrat(float y); // Constructor simplificat
    std::unique_ptr<ObiectJoc> clone() const override;
    void update() override;
    bool interactioneaza(Player& player) override;
    void draw(sf::RenderWindow& window) const override;
private:
    void afisare(std::ostream& os) const override;
    // Funcție helper internă pentru rotirea punctelor
    sf::Vector2f rotestePunct(sf::Vector2f punct, float unghiGrade) const;
};
class ObstacolDouaElice : public ObiectJoc {
    float raza_;     // Lungimea unei pale
    float grosime_;
    float rotatieStanga_;
    float rotatieDreapta_;

    sf::Vector2f centruStanga_;
    sf::Vector2f centruDreapta_;

    std::vector<Culoare> culoriStanga_;
    std::vector<Culoare> culoriDreapta_;

public:
    ObstacolDouaElice(float y);
    std::unique_ptr<ObiectJoc> clone() const override;
    void update() override;
    bool interactioneaza(Player& player) override;
    void draw(sf::RenderWindow& window) const override;

private:
    void afisare(std::ostream& os) const override;
    sf::Vector2f rotestePunct(sf::Vector2f punct, float unghiGrade) const;

    // Helper pentru a verifica coliziunea cu o singura elice
    void verificaElice(Player& p, sf::Vector2f centru, float rotatie, const std::vector<Culoare>& culori);

    // Helper pentru desenare
    void deseneazaElice(sf::RenderWindow& w, sf::Vector2f centru, float rotatie, const std::vector<Culoare>& culori) const;
};
// --- Schimbator Culoare ---
class SchimbatorCuloare : public ObiectJoc {
public:
    // MODIFICARE AICI: Adaugam parametrul 'culoareDeEvitat'
    SchimbatorCuloare(float y, TipCuloare culoareDeEvitat);

    std::unique_ptr<ObiectJoc> clone() const override;
    void update() override;
    bool interactioneaza(Player& player) override;
    void draw(sf::RenderWindow& window) const override;
private:
    void afisare(std::ostream& os) const override;
};
class ObstacolBanda : public ObiectJoc {
    float latimeSegment_; // Cat de lat este un dreptunghi de culoare
    float inaltime_;      // Grosimea benzii (pe Y)
    float offsetX_;       // Deplasarea curenta pe X
    float vitezaDirectie_; // Viteza de baza (+ pentru dreapta, - pentru stanga)
    std::vector<Culoare> modelCulori_; // Ordinea culorilor

    public:
    // Parametrul 'spreDreapta' decide directia si setul de culori
    ObstacolBanda(float y, bool spreDreapta);

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
    LinieSosire(float y);
    std::unique_ptr<ObiectJoc> clone() const override;
    void update() override;
    bool interactioneaza(Player& player) override;
    void draw(sf::RenderWindow& window) const override;
private:
    void afisare(std::ostream& os) const override;
};