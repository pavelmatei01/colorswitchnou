#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <random>
#include <cstdlib>

using namespace std;

enum class ColorType { YELLOW, GREEN, RED, BLUE };

class Color {
    ColorType type_;
public:
    explicit Color(ColorType type) : type_(type) {
        cout << "Culoare creata " << static_cast<int>(type_) << endl;
    }
    [[nodiscard]] bool matches(const Color& other) const {
        return type_ == other.type_;
    }
    [[nodiscard]] ColorType getType() const {
        return type_;
    }
    friend ostream &operator<<(ostream &os, const Color &c) {
        switch (c.type_) {
            case ColorType::YELLOW: os << "galbena"; break;
            case ColorType::GREEN: os << "verde"; break;
            case ColorType::RED: os << "rosie"; break;
            case ColorType::BLUE: os << "albastra"; break;
        }
        return os;
    }
};

class Ball {
private:
    float radius_;
    Color color_;
    float y_;
    float velocityY_;
    const float GRAVITY = -9.81f;
    const float JUMP_FORCE = 10.0f;
public:
    Ball(float y, ColorType InitialColor) : radius_(15.0f), color_(InitialColor), y_(y), velocityY_(0.0f) {
        cout << "Minge creata, avand culoarea " << color_ << " si raza " << radius_ << ", aflandu-se la coordonata " << y_ << endl;
    }
    Ball(const Ball& other) : radius_(other.radius_), color_(other.color_), y_(other.y_), velocityY_(other.velocityY_) {
        cout << "Minge copiata la y=" << y_ << endl;
    }
    Ball& operator=(const Ball& other) {
        if (this != &other) {
            radius_ = other.radius_;
            color_ = other.color_;
            y_ = other.y_;
            velocityY_ = other.velocityY_;
        }
        cout << "Operator= apelat pentru Ball la y=" << y_ << endl;
        return *this;
    }
    ~Ball() {
        cout << "Minge distrusa." << endl;
    }
    void simulateJump() {
        velocityY_ = JUMP_FORCE;
        cout << "Am efectuat o saritura,cu viteza " << JUMP_FORCE << endl;
    }
    void updatePhysics() {
        velocityY_ += GRAVITY;
        y_ += velocityY_;
        cout << "Am actualizat pozitia mingii,y=" << y_ << ", viteza este:" << velocityY_ << endl;
    }
    void setColor(ColorType newType) {
        color_ = Color(newType);
        cout << "Culoarea mingii este: " << color_ << endl;
    }
    [[nodiscard]] float getY() const { return y_; }
    [[nodiscard]] const Color& getColor() const { return color_; }
    [[nodiscard]] float getRadius() const { return radius_; }
    friend ostream &operator<<(ostream &os, const Ball &b) {
        os << "Mingea se afla la pozitia y=" << b.y_ << " cu culoarea " << b.color_ << " si raza " << b.radius_;
        return os;
    }
};

class Segment {
private:
    Color color_;
    float startAngle_, endAngle_;
public:
    Segment(ColorType type, float start, float end) : color_(type), startAngle_(start), endAngle_(end) {}
    [[nodiscard]] const Color& getColor() const { return color_; }
    friend ostream &operator<<(ostream &os, const Segment &s) {
        os << "Segmentul obstacolului cu culoarea " << s.color_ << " (" << s.startAngle_ << "-" << s.endAngle_ << " grade)\n";
        return os;
    }
};

class Obstacle {
    vector<Segment> segments_;
    float centerY_;
    float rotation_;
    float radius_;
public:
    explicit Obstacle(float y) : centerY_(y), rotation_(0.0f), radius_(40.0f) {
        int freq[4] = { 0,0,0,0 };
        for (int i = 0; i < 4; i++) {
            int culoare;
            do {
                culoare = rand() % 4;
            } while (freq[culoare] != 0);
            freq[culoare]++;
            float startAngle = i * 90.0f;
            float endAngle = startAngle + 90.0f;
            segments_.emplace_back(static_cast<ColorType>(culoare), startAngle, endAngle);
        }
        cout << "Obstacol creat la y=" << y << " cu raza " << radius_ << endl;
    }
    void rotate() {

        std::rotate(segments_.begin(), segments_.begin() + 1, segments_.end());
        cout<<"Am rotit obstacolul cu 180 de grade"<<endl;
    }
    [[nodiscard]] bool checkCollision(const Ball& ball) const {
        float dy = abs(ball.getY() - centerY_);
        if (dy > radius_ + ball.getRadius()) {
            return false;
        }
        const Segment& activeTop = segments_.front();
        bool match = ball.getColor().matches(activeTop.getColor());
        cout << "Coliziune cu " << activeTop << ". Potrivire culoare: " << (match ? "da" : "nu") << endl;
        return !match;
    }
    [[nodiscard]] float getRadius() const { return radius_; }
    friend ostream &operator<<(ostream &os, const Obstacle &o) {
        os << "Obstacol la y=" << o.centerY_ << " cu raza " << o.radius_ << " si segmente: ";
        for (const auto& s : o.segments_) {
            os << s << " ";
        }
        return os;
    }
};

class Pickup {
private:
    Color color_;
    float radius_;
    float y_;
    bool collected_;
public:
    explicit Pickup(float y) : color_(static_cast<ColorType>(rand() % 4)), radius_(12.5f), y_(y), collected_(false) {
        cout << "Pickup creat la y=" << y << " cu raza " << radius_ << endl;
    }
    void randomizeColor() {
        ColorType newType = static_cast<ColorType>(rand() % 4);
        color_ = Color(newType);
        cout << "Pickup schimbat in culoarea: " << color_ << endl;
    }
    void setColor(ColorType type) {
        color_ = Color(type);
    }
    [[nodiscard]] ColorType getColorType() const {
        return color_.getType();
    }
    [[nodiscard]] bool isCollected(const Ball& ball) const {
        if (collected_) return false;
        float dy = abs(ball.getY() - y_);
        return dy < (radius_ + ball.getRadius());
    }
    void setCollected() {
        collected_ = true;
    }
    [[nodiscard]] float getRadius() const { return radius_; }
    [[nodiscard]] float getY() const { return y_; }
    [[nodiscard]] const Color& getColor() const { return color_; }
    friend ostream &operator<<(ostream &os, const Pickup &p) {
        os << " Pickup la y= " << p.y_ << " cu culoarea " << p.color_ << " si raza " << p.radius_;
        if (p.collected_) os << "(colectat) ";
        return os;
    }
};

class Game {
private:
    Ball ball_;
    vector<Obstacle> obstacles_;
    vector<Pickup> pickups_;
    bool gameOver_;
    int score_;
    const float INITIAL_Y = 600.0f;
    const float MIN_Y = 100.0f;
    const float TOTAL_DISTANCE = INITIAL_Y - MIN_Y;
    bool pickups_color_initialized = false;
    void incarcarejoc() {
        vector<float> obsY = { 100.0f,200.0f,300.0f,400.0f,500.0f };
        vector<float> pickupY = { 150.0f,250.0f,350.0f,450.0f,550.0f };
        for (float oy : obsY) {
            obstacles_.emplace_back(oy);
        }
        for (float py : pickupY) {
            pickups_.emplace_back(py);
        }
        cout << "Nivelul 1 a fost incarcat" << endl;
    }
public:
    [[nodiscard]] float progres() const {
        float traveled = INITIAL_Y - ball_.getY();
        float progress = (traveled / TOTAL_DISTANCE) * 100.0f;
        if (progress <= 0.0f) progress = 0.0f;
        if (progress >= 100.0f) progress = 100.0f;
        cout << "Progres: " << progress << "%" << endl;
        return progress;
    }
    Game() : ball_(600.0f, static_cast<ColorType>(rand() % 4)), gameOver_(false), score_(0) {
        incarcarejoc();
        cout << "Joc initializat in modul consola" << endl;
    }
    void updateLevel() {
        if (gameOver_) return;
        ball_.updatePhysics();
        for (auto& obs : obstacles_) {
            obs.rotate();
        }
        for (const auto& obs : obstacles_) {
            if (obs.checkCollision(ball_)) {
                gameOver_ = true;
                saveScore();
                return;
            }
        }
        for (auto& pickup : pickups_) {
            if (pickup.isCollected(ball_)) {
                ball_.setColor(pickup.getColorType());
                pickup.setCollected();
                score_ += 10;
                cout << "Ai colectat un pickup! Score nou:" << score_ << endl;
            }
        }
        if (ball_.getY() < 0.0f) {
            score_ += 100;
            gameOver_ = true;
            cout << "Ai castigat! Scor final:" << score_ << endl;
        }
        float progress = progres();
        cout << "Progres curent:" << progress << "% (y:" << ball_.getY() << ", scor:" << score_ << ",culoare:" << ball_.getColor() << ")" << endl;
    }
    static bool handleImput() {
        static int FrameCount = 0;
        ++FrameCount;
        if (FrameCount % 5 == 0) return true;
        return false;
    }
    void render() {
        cout << "\n---Starea jocului---\n";
        cout << ball_ << endl;
        cout << "Obstacole:\n";
        for (const Obstacle& obs : obstacles_) {
            cout << " " << obs << endl;
        }
        cout << "Pickup-uri:\n";
        if (!pickups_color_initialized) {
            for (Pickup& p : pickups_) {
                p.randomizeColor();
            }
            pickups_color_initialized = true;
            cout << "Am generat culorile pentru pickup-uri\n";
        }
        for (const Pickup& p : pickups_) {
            cout << " " << p << endl;
        }
        cout << "---Sfrasitul starii---\n";
    }
    void saveScore() const {
        cout << "Scor salvat:" << score_ << endl;
    }
    void triggerJump() {
        ball_.simulateJump();
    }
    [[nodiscard]] bool isGameOver() const { return gameOver_; }
    [[nodiscard]] int getScore() const { return score_; }
    friend ostream &operator<<(ostream &os, const Game &g) {
        os << "Joc: " << g.obstacles_.size() << " obstacole, scor " << g.score_ << " , " << g.ball_;
        return os;
    }
};

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    cout << "Pornire demo cu creari si apeluri publice" << endl;

    Color c1(static_cast<ColorType>(rand() % 4));
    Color c2(static_cast<ColorType>(rand() % 4));
    cout << "Culoare 1: " << c1 << ", Tip: " << static_cast<int>(c1.getType()) << endl;
    cout << "Se potrivesc? " << (c1.matches(c2) ? "Da" : "Nu") << endl;

    Ball b_demo(500.0f, ColorType::GREEN);
    cout << "Apel comenzi pentru minge: " << b_demo << endl;
    cout << "Y: " << b_demo.getY() << ", Raza: " << b_demo.getRadius() << endl;
    b_demo.simulateJump();
    b_demo.updatePhysics();
    b_demo.setColor(ColorType::YELLOW);
    cout << "Dupa update: " << b_demo << endl;
    Ball b_copy = b_demo;

    Segment s_demo(ColorType::RED, 0.0f, 90.0f);
    cout << "Apel comenzi pentru segmente: " << s_demo << ", Culoare: " << s_demo.getColor() << endl;

    Obstacle o_demo(300.0f);
    cout << "Apel comenzi pentru obstacle: " << o_demo << endl;
    cout << "Raza: " << o_demo.getRadius() << endl;

    (void)o_demo.checkCollision(b_demo);

    Pickup p_demo(400.0f);
    cout << "Apel comenzi pentru pickup " << p_demo << endl;
    cout << "Y: " << p_demo.getY() << ", Raza: " << p_demo.getRadius() << endl;
    cout << "Culoare: " << p_demo.getColor() << ", Tip: " << static_cast<int>(p_demo.getColorType()) << endl;
    p_demo.randomizeColor();
    p_demo.setColor(ColorType::GREEN);
    cout << "Colectat? " << (p_demo.isCollected(b_demo) ? "Da" : "Nu") << endl;
    p_demo.setCollected();
    cout << "Dupa colectare: " << p_demo << endl;

    cout << "\nPornire joc in modul consola" << endl;
    Game game;
    cout << game << endl;

    while (!game.isGameOver()) {
        if (Game::handleImput()) {
            game.triggerJump();
        }
        game.updateLevel();
        game.render();
        (void)game.progres();
    }

    cout << "\nJoc incheiat! Scor final: " << game.getScore() << endl;
    cout << "Jocul s-a incheiat." << endl;
    return 0;
}