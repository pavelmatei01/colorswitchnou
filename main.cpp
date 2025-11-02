#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <algorithm>

enum class ColorType{ YELLOW,GREEN,RED, BLUE};
class Color {
    ColorType type_;
public:
    Color(ColorType type) : type_(type) {
        std::cout<<"Culoare creata "<<static_cast<int>(type_)<<std::endl;
    }
    bool matches(const Color& other)const {
        return type_ == other.type_;}
    ColorType getType()const {
        return type_;
    }
    friend std::ostream &operator<<(std::ostream &os, const Color &c) {
        switch (c.type_) {
            case ColorType::YELLOW:os<<"galbena";break;
            case ColorType::GREEN:os<<"verde";break;
            case ColorType::RED:os<<"rosie";break;
            case ColorType::BLUE:os<<"albastra";break;

        }
        return os;
    }

};
class Ball {
private: float radius_;
         Color color_;
         float y_;
         float velocityY_;
         const float GRAVITY=-9.81;
         const float JUMP_FORCE=10.0;
    public:
    Ball(float y, ColorType InitialColor): radius_(15.0),color_(InitialColor),y_(y),velocityY_(0.0)
    {std::cout<<"Minge creata, avand  culoarea "<<color_<<" si raza "<<radius_<<", aflandu-se la coordonata "<< y_<<std::endl;}
    Ball (const Ball&other):radius_(other.radius_),color_(other.color_),y_(other.y_),velocityY_(other.velocityY_)
    {std::cout<<"Minge copiata la y="<<y_<<std::endl;}
    Ball& operator=(const Ball& other) {
        if (this!=&other) {
            radius_ = other.radius_;
            color_ = other.color_;
            y_ = other.y_;
            velocityY_ = other.velocityY_;
        }
        return *this;
    }
    ~Ball() {std::cout<<"Minge distrusa."<<std::endl;}
    void simulateJump() {
        velocityY_=JUMP_FORCE;
        std::cout<<"Am efectuat o saritura,cu viteza "<<JUMP_FORCE<<std::endl;
    }
    void updatePhysics() {
        velocityY_+=GRAVITY;
        y_+=velocityY_;
        std::cout<<"Am actualizat pozitia mingii,y="<<y_<<", viteza este:"<<velocityY_<<std::endl;
    }
    /*void changeColor() {
        ColorType newType=static_cast<ColorType>(rand()%4);
        color_ = Color(newType);
        std::cout<<"Culoarea mingii este acum "<<color_<<std::endl;
    }*/
    void setColor(ColorType newType) {
        color_=Color(newType);
        std::cout<<"Culoarea mingii este:  "<<color_<<std::endl;
    }
    float getY()const {return y_;}
    const Color& getColor()const {return color_;}
    float getRadius()const {return radius_;}
    friend std::ostream &operator<<(std::ostream &os, const Ball &b) {
        os<<"Mingea se afla la pozitia y="<<b.y_<<" cu culoarea "<<b.color_<<" si raza "<<b.radius_;
            return os;
    }

};
class Segment {
private:
    Color color_;
    float startAngle_,endAngle_;
public:
    Segment(ColorType type,float start, float end) : color_(type), startAngle_(start),endAngle_(end){}
    const Color& getColor()const {return color_;}
    float getStartAngle()const {return startAngle_;}
    float getEndAngle()const {return endAngle_;}
    friend std::ostream &operator<<(std::ostream &os, const Segment &s) {
        os<<"Segmentul obstacolului cu culoarea "<<s.color_<<" ("<<s.startAngle_<<"-"<<s.endAngle_<<" grade)";
        return os;
    }
};
class Obstacle {
    std::vector<Segment> segments_;
    float centerY_;
    float rotation_;
    float radius_;
    public:
    Obstacle (float y):centerY_(y),rotation_(0.0),radius_(40.0) {
        int freq[4]={0,0,0,0};
        for (int i=0;i<4;i++) {
            int culoare;
            do
            {culoare=rand()%4;}
            while (freq[culoare]!=0);
            freq[culoare]++;
            float startAngle=i*90.0;
            float endAngle=startAngle+90.0;
            segments_.emplace_back(static_cast<ColorType>(culoare),startAngle,endAngle);
        }
        std::cout<<"Obstacol creat la y="<<y<<" cu raza "<<radius_<<std::endl;
    }
    void rotate(float deltaTime) {
        rotation_+=180.0*deltaTime;
        if (rotation_>=360.0)
            rotation_=0.0;
        std::rotate(segments_.begin(),segments_.begin()+1,segments_.end());
        std::cout<<"Obstacol rotit cu "<<rotation_<<" grade"<<std::endl;

    }
    bool checkCollision(const Ball&ball)const {
        float dy=std::abs(ball.getY()-centerY_);
        if (dy>radius_+ball.getRadius()) {
            return false;
        }
        const Segment&activeTop=segments_.front();
        bool match=ball.getColor().matches(activeTop.getColor());
        std::cout<<"Coliziune cu "<<activeTop<<". Potrivire culoare: "<<(match? "da":"nu")<<std::endl;
        return !match;

    }
    float getCenterY()const {return centerY_;}
    float getRadius()const {return radius_;}
    friend std::ostream &operator<<(std::ostream &os, const Obstacle &o) {
        os<<"Obstacol la y="<<o.centerY_<<" cu raza "<< o.radius_<<" si segmente: ";
        for (const auto& s :o.segments_)
        {os<<s<<" ";}
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
    Pickup (float y):color_(static_cast<ColorType>(rand()%4)),y_(y),radius_(12.5),collected_(false)
    {std::cout<<"Pickup creat la y="<<y<<" cu raza "<<radius_<<std::endl;}
    void randomizeColor() {
        ColorType newType= static_cast<ColorType>(rand()%4);
        color_=Color(newType);
        std::cout<<"Pickup schimbat in culoarea: "<<color_<<std::endl;
    }
    void setColor(ColorType type) {color_=Color(type);}
    ColorType getColorType() const{return color_.getType();}
    bool isCollected(const Ball&ball) const {
        if (collected_)
            return true;
        float dy=std::abs(ball.getY()-y_);
        return dy<(radius_+ball.getRadius());
    }
    void setCollected() {collected_=true;}
    bool getCollected() const {return collected_;}
    float getRadius() const {return radius_;}
    float getY() const {return y_;}
    const Color& getColor() const {return color_;}
    friend std::ostream &operator<<(std::ostream &os, const Pickup &p) {
        os<<" Pickup la y= "<<p.y_<<" cu culoarea "<<p.color_<<" si raza " <<p.radius_;
        if (p.collected_)
            os<<"(colectat) ";
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
    const float INITIAL_Y=600.0;
    const float MIN_Y=100.0;
    const float TOTAL_DISTANCE=INITIAL_Y-MIN_Y;
    bool pickups_color_initialized=false;
    void incarcarejoc() {
        std::vector<float>obsY={100.0,200.0,300.0,400.0,500.0};
        std::vector<float>pickupY={150.0,250.0,350.0,450.0,550.0};
        for (float oy:obsY)
        {obstacles_.emplace_back(oy);}
        for (float py:pickupY)
            {pickups_.emplace_back(py);}
        std::cout<<"Nivelul 1 a fost incarcat"<<std::endl;
    }
    float progres() const {
        float traveled=INITIAL_Y-ball_.getY();
        float progress=(traveled/TOTAL_DISTANCE)*100.0;
        if (progress<=0.0) progress=0.0;
        if (progress>=100.0)progress=100.0;
        std::cout<<"Progres: "<<progress<<"%"<<std::endl;
        return progress;
    }
public:
    Game():ball_(600.0f,static_cast<ColorType>(rand()%4)),gameOver_(false),score_(0) {
        incarcarejoc();
        std::cout<<"Joc initializat in modul consola"<<std::endl;
    }
    void updateLevel() {
        if (gameOver_) return;
        ball_.updatePhysics();
        for (auto&obs:obstacles_)
            obs.rotate(1.0/60.0);
        for (const auto&obs:obstacles_)
            if (obs.checkCollision(ball_)) {
                gameOver_=true;
                saveScore();
                return;
            }
        for (auto& pickup:pickups_)
        {if (pickup.isCollected(ball_))
            {ball_.setColor(pickup.getColorType());
            pickup.setCollected();
            score_+=10;
            std::cout<<"Ai colectat un pickup! Score nou:"<<score_<<std::endl;
            }
        }
        if (ball_.getY()<0) {
            score_+=100;
            gameOver_=true;
            std::cout<<"Ai castigat! Scor final:"<<score_<<std::endl;
        }
        float progress=progres();
        std::cout<<"Progres curent:"<<progress<<"% (y:"<<ball_.getY()<<", scor:"<<score_<<",culoare:"<<ball_.getColor()<<")"<<std::endl;
    }
    bool handleImput() {
        static int FrameCount=0;
        ++FrameCount;
        if (FrameCount%5==0) return true;
        return false;
    }
    void render() {
        std::cout<<"\n---Starea jocului---\n";
        std::cout<<ball_<<std::endl;
        std::cout<<"Obstacole:\n";
        for (const Obstacle&obs:obstacles_)
            std::cout<<" "<<obs<<std::endl;
        std::cout<<"Pickup-uri:\n";
        if (!pickups_color_initialized) {
            for (Pickup& p:pickups_)
                p.randomizeColor();
            pickups_color_initialized=true;
            std::cout<<"Am generat culorile pentru pickup-uri\n";
        }
        for (const Pickup&p:pickups_)
            std::cout<<" "<<p<<std::endl;
        std::cout<<"---Sfrasitul starii---\n";
    }
    void saveScore() {
        std::cout<<"Scor salvat:"<<score_<<std::endl;
    }
    void triggerJump()
    {ball_.simulateJump();}
    bool isGameOver()const {return gameOver_;}
    int getScore()const {return score_;}
    friend std::ostream &operator<<(std::ostream &os, const Game &g) {
        os<<"Joc: "<<g.obstacles_.size()<<" obstacole, scor "<<g.score_<<" , "<<g.ball_;
        return os;

    }

};
int main() {
  srand(static_cast<unsigned int>(time(nullptr)));
    Game game;
    std::cout<<"Pornire joc in modul consola: "<<game<<std::endl;
    while (!game.isGameOver()) {
        {
            if (game.handleImput()) {
                game.triggerJump();
            }
            game.updateLevel();
            game.render();
        }
    }
    std::cout<<"\n Joc incheiat! Scor final: "<<game.getScore()<<std::endl;
    std::cout<<"Jocul s-a incheiat."<<std::endl;
    return 0;
}
