#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <random>
#include <cstdlib>

enum class ColorType{ YELLOW, GREEN, RED, BLUE };

class Color {
    ColorType type_;
public:
    explicit Color(ColorType type) : type_(type) {
        std::cout << "Culoare creata " << static_cast<int>(type_) << std::endl;
    }
    [[nodiscard]] bool matches(const Color& other) const {
        return type_ == other.type_;
    }
    [[nodiscard]] ColorType getType() const {
        return type_;
    }
    friend std::ostream &operator<<(std::ostream &os, const Color &c) {
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
    Ball(float y, ColorType InitialColor) : radius_(15.0f), color_(InitialColor), y_(y), velocityY_(0.0f)
    {
        std::cout << "Minge creata, avand  culoarea " << color_ << " si raza " << radius_ << ", aflandu-se la coordonata " << y_ << std::endl;
    }
    Ball(const Ball& other) : radius_(other.radius_), color_(other.color_), y_(other.y_), velocityY_(other.velocityY_)
    {
        std::cout << "Minge copiata la y=" << y_ << std::endl;
    }
    Ball& operator=(const Ball& other) {
        if (this != &other) {
            radius_ = other.radius_;
            color_ = other.color_;
            y_ = other.y_;
            velocityY_ = other.velocityY_;
        }
        return *this;
    }
    ~Ball() { std::cout << "Minge distrusa." << std::endl; }
    void simulateJump() {
        velocityY_ = JUMP_FORCE;
        std::cout << "Am efectuat o saritura,cu viteza " << JUMP_FORCE << std::endl;
    }
    void updatePhysics() {
        velocityY_ += GRAVITY;
        y_ += velocityY_;
        std::cout << "Am actualizat pozitia mingii,y=" << y_ << ", viteza este:" << velocityY_ << std::endl;
    }
    void setColor(ColorType newType) {
        color_ = Color(newType);
        std::cout << "Culoarea mingii este:  " << color_ << std::endl;
    }
    [[nodiscard]] float getY() const { return y_; }
    [[nodiscard]] const Color& getColor() const { return color_; }
    [[nodiscard]] float getRadius() const { return radius_; }
    friend std::ostream &operator<<(std::ostream &os, const Ball &b) {
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
    friend std::ostream &operator<<(std::ostream &os, const Segment &s) {
        os << "Segmentul obstacolului cu culoarea " << s.color_ << " (" << s.startAngle_ << "-" << s.endAngle_ << " grade)";
        return os;
    }
};

class Obstacle {
    std::vector<Segment> segments_;
    float centerY_;
    float rotation_;
    float radius_;
public:
    explicit Obstacle(float y) : centerY_(y), rotation_(0.0f), radius_(40.0f) {
        int freq[4] = { 0,0,0,0 };
        for (int i = 0; i < 4; i++) {
            int culoare;
            do {
                culoare = std::rand() % 4;
            } while (freq[culoare] != 0);
            freq[culoare]++;
            float startAngle = i * 90.0f;
            float endAngle = startAngle + 90.0f;
            segments_.emplace_back(static_cast<ColorType>(culoare), startAngle, endAngle);
        }
        std::cout << "Obstacol creat la y=" << y << " cu raza " << radius_ << std::endl;
    }
    void rotate(float deltaTime) {
        rotation_ += 180.0f * deltaTime;
        if (rotation_ >= 360.0f)
            rotation_ = 0.0f;
        std::rotate(segments_.begin(), segments_.begin() + 1, segments_.end());
        std::cout << "Obstacol rotit cu " << rotation_ << " grade" << std::endl;
    }
    [[nodiscard]] bool checkCollision(const Ball& ball) const {
        float dy = std::abs(ball.getY() - centerY_);
        if (dy > radius_ + ball.getRadius()) {
            return false;
        }
        const Segment& activeTop = segments_.front();
        bool match = ball.getColor().matches(activeTop.getColor());
        std::cout << "Coliziune cu " << activeTop << ". Potrivire culoare: " << (match ? "da" : "nu") << std::endl;
        return !match;
    }
    [[nodiscard]] float getRadius() const { return radius_; }
    friend std::ostream &operator<<(std::ostream &os, const Obstacle &o) {
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
    explicit Pickup(float y) : color_(static_cast<ColorType>(std::rand() % 4)), radius_(12.5f), y_(y), collected_(false)
    {
        std::cout << "Pickup creat la y=" << y << " cu raza " << radius_ << std::endl;
    }
    void randomizeColor() {
        ColorType newType = static_cast<ColorType>(std::rand() % 4);
        color_ = Color(newType);
        std::cout << "Pickup schimbat in culoarea: " << color_ << std::endl;
    }
    void setColor(ColorType type) { color_ = Color(type); }
    [[nodiscard]] ColorType getColorType() const { return color_.getType(); }
    [[nodiscard]] bool isCollected(const Ball& ball) const {
        if (collected_)
            return false;
        float dy = std::abs(ball.getY() - y_);
        return dy < (radius_ + ball.getRadius());
    }
    void setCollected() { collected_ = true; }
    [[nodiscard]] float getRadius() const { return radius_; }
    [[nodiscard]] float getY() const { return y_; }
    [[nodiscard]] const Color& getColor() const { return color_; }
    friend std::ostream &operator<<(std::ostream &os, const Pickup &p) {
        os << " Pickup la y= " << p.y_ << " cu culoarea " << p.color_ << " si raza " << p.radius_;
        if (p.collected_)
            os << "(colectat) ";
        return os;
    }
};

class Game {
private:
    Ball ball_;
    std::vector<Obstacle> obstacles_;
    std::vector<Pickup> pickups_;
    bool gameOver_;
    int  score_;
    const float INITIAL_Y = 600.0f;
    const float MIN_Y = 100.0f;
    const float TOTAL_DISTANCE = INITIAL_Y - MIN_Y;
    bool pickups_color_initialized = false;
    void incarcarejoc() {
        std::vector<float> obsY = { 100.0f,200.0f,300.0f,400.0f,500.0f };
        std::vector<float> pickupY = { 150.0f,250.0f,350.0f,450.0f,550.0f };
        for (float oy : obsY) {
            obstacles_.emplace_back(oy);
        }
        for (float py : pickupY) {
            pickups_.emplace_back(py);
        }
        std::cout << "Nivelul 1 a fost incarcat" << std::endl;
    }
    [[nodiscard]] float progres() const {
        float traveled = INITIAL_Y - ball_.getY();
        float progress = (traveled / TOTAL_DISTANCE) * 100.0f;
        if (progress <= 0.0f) progress = 0.0f;
        if (progress >= 100.0f) progress = 100.0f;
        std::cout << "Progres: " << progress << "%" << std::endl;
        return progress;
    }
public:
    Game() : ball_(600.0f, static_cast<ColorType>(std::rand() % 4)), gameOver_(false), score_(0) {
        incarcarejoc();
        std::cout << "Joc initializat in modul consola" << std::endl;
    }
    void updateLevel() {
        if (gameOver_) return;
        ball_.updatePhysics();
        for (auto& obs : obstacles_) {
            obs.rotate(1.0f / 60.0f);
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
                std::cout << "Ai colectat un pickup! Score nou:" << score_ << std::endl;
            }
        }
        if (ball_.getY() < 0.0f) {
            score_ += 100;
            gameOver_ = true;
            std::cout << "Ai castigat! Scor final:" << score_ << std::endl;
        }
        float progress = progres();
        std::cout << "Progres curent:" << progress << "% (y:" << ball_.getY() << ", scor:" << score_ << ",culoare:" << ball_.getColor() << ")" << std::endl;
    }
    static bool handleImput() {
        static int FrameCount = 0;
        ++FrameCount;
        if (FrameCount % 5 == 0) return true;
        return false;
    }
    void render() {
        std::cout << "\n---Starea jocului---\n";
        std::cout << ball_ << std::endl;
        std::cout << "Obstacole:\n";
        for (const Obstacle& obs : obstacles_) {
            std::cout << " " << obs << std::endl;
        }
        std::cout << "Pickup-uri:\n";
        if (!pickups_color_initialized) {
            for (Pickup& p : pickups_) {
                p.randomizeColor();
            }
            pickups_color_initialized = true;
            std::cout << "Am generat culorile pentru pickup-uri\n";
        }
        for (const Pickup& p : pickups_) {
            std::cout << " " << p << std::endl;
        }
        std::cout << "---Sfrasitul starii---\n";
    }
    void saveScore() const {
        std::cout << "Scor salvat:" << score_ << std::endl;
    }
    void triggerJump() {
        ball_.simulateJump();
    }
    [[nodiscard]] bool isGameOver() const { return gameOver_; }
    [[nodiscard]] int getScore() const { return score_; }
    friend std::ostream &operator<<(std::ostream &os, const Game &g) {
        os << "Joc: " << g.obstacles_.size() << " obstacole, scor " << g.score_ << " , " << g.ball_;
        return os;
    }
};

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    Game game;
    std::cout << "Pornire joc in modul consola: " << game << std::endl;
    while (!game.isGameOver()) {
        if (game.handleImput()) {
            game.triggerJump();
        }
        game.updateLevel();
        game.render();
    }
    std::cout << "\n Joc incheiat! Scor final: " << game.getScore() << std::endl;
    std::cout << "Jocul s-a incheiat." << std::endl;
    return 0;
}