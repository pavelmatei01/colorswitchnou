#pragma once
#include "obiectjoc.h"
#include <vector>

class ObstacolRotativ : public ObiectJoc {
    float razaMedie_, grosime_, rotatie_;
    std::vector<Culoare> segmenteCulori_;
public:
    explicit ObstacolRotativ(float y);
    std::unique_ptr<ObiectJoc> clone() const override;
    void update() override;
    bool interactioneaza(Player& player) override;
    void draw(sf::RenderWindow& window) const override;
private:
    void afisare(std::ostream& os) const override;
};

class ObstacolDublu : public ObiectJoc {
    float razaInt_, razaExt_, grosime_, rotInt_, rotExt_;
    std::vector<Culoare> culoriInt_, culoriExt_;
public:
    explicit ObstacolDublu(float y);
    std::unique_ptr<ObiectJoc> clone() const override;
    void update() override;
    bool interactioneaza(Player& player) override;
    void draw(sf::RenderWindow& window) const override;
private:
    void afisare(std::ostream& os) const override;
    void verificaInel(const Player& player, float raza, float rotatie, const std::vector<Culoare>& culori);
};

class Stea : public ObiectJoc {
    static sf::Texture textureStea;
    sf::Sprite sprite_;
public:
    static void incarcaTextura();
    explicit Stea(float y);
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
    explicit ObstacolPatrat(float y);
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
    explicit ObstacolDouaElice(float y);
    std::unique_ptr<ObiectJoc> clone() const override;
    void update() override;
    bool interactioneaza(Player& player) override;
    void draw(sf::RenderWindow& window) const override;
private:
    void afisare(std::ostream& os) const override;
    sf::Vector2f rotestePunct(sf::Vector2f punct, float unghiGrade) const;
    void verificaElice(const Player& p, sf::Vector2f centru, float rotatie, const std::vector<Culoare>& culori);
    void deseneazaElice(sf::RenderWindow& w, sf::Vector2f centru, float rotatie, const std::vector<Culoare>& culori) const;
};

class SchimbatorCuloare : public ObiectJoc {

    Culoare culoareNoua_;
public:
    SchimbatorCuloare(float y, TipCuloare culoareDeEvitat);
    SchimbatorCuloare(const SchimbatorCuloare& other);
    const Culoare& getCuloare() const { return culoareNoua_; }
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
    explicit ObstacolBanda(float y, bool spreDreapta);
    std::unique_ptr<ObiectJoc> clone() const override;
    void update() override;
    bool interactioneaza(Player& player) override;
    void draw(sf::RenderWindow& window) const override;
private:
    void afisare(std::ostream& os) const override;
};

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
class PowerUpMulticolor : public ObiectJoc {
    sf::Color culoareAfisare_;
public:
    explicit PowerUpMulticolor(float y);
    std::unique_ptr<ObiectJoc> clone() const override;
    void update() override;
    bool interactioneaza(Player& player) override;
    void draw(sf::RenderWindow& window) const override;
private:
    void afisare(std::ostream& os) const override;
};