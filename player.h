#ifndef PLAYER__H
#define PLAYER__H

#include "multisprite.h"
#include <list>
#include "bullet.h"
enum State {IDLE, RUN, DUCK, SHOOTING, JUMP ,DEAD, RESET};
enum Direction {RIGHT, LEFT};
class Player : public Drawable {
public:
  Player(const std::string&);
  //Player(const Player&);
~Player();
  virtual void update(Uint32 ticks);
    //using MultiSprite::draw;
  virtual void draw() const;
  void collided() { collision = true; }
  void missed() { collision = false; }
  Player& operator=(const Player&);
    void shoot();
    void die();
    State getState() const {
        return state;
    }
    void goidle();
    void isLastFrame() {
        if(currentFrame + 1 == numberOfFrames) { willDraw = false;}
    }
    bool shouldDraw() {
        return willDraw;
    }
    virtual const Image* getImage() const {
        return images[currentFrame];
    }
    int getScaledWidth()  const {
        return getScale()*images[currentFrame]->getWidth();
    }
    bool canControl() const{
        return control;
    }
    
    int getScaledHeight()  const {
        return getScale()*images[currentFrame]->getHeight();
    }
    virtual const SDL_Surface* getSurface() const {
        return images[currentFrame]->getSurface();
    }
    void alterImage(unsigned n, unsigned x){
        if(numberOfFrames != n){
            currentFrame = 0;
            numberOfFrames = n;
            frameInterval = x;
        }
    }
    void notDucking(){
        hasChangedDuck = false;
    }
    void setDuckingImages(){
        if(!hasChangedDuck){
            setY(getY() + 100);
            hasChangedDuck = true;
        }
    }
  void right();
  void left();
  void jump();
  void down();
  void stop();
    void drawObjectPool() const;
    void toggleOP(){
        drawOP = !drawOP;
    }
    void changeColor(int n){
        color = n;
    }
    int bulletCount(){
        return bullets.size();
    }
    Bullet getBullet(int n){
        auto front = bullets.begin();
        std::advance(front, n);
        return *front;
    }
    bool canDie() const {
        return godMode;
    }
    void toggleImmortality(){
        godMode = !godMode;
    }
private:
  bool collision;
  Vector2f initialVelocity;
    std::string bulletName;
    std::list<Bullet> bullets;
    std::list<Bullet> freeB;
    float minSpeed;
    float bulletInterval;
    float timeSinceLastFrame;
    float timeSinceLastBullet;

    std::vector<Image *> images;
    State state;
    Direction direction;
    unsigned currentFrame;
    unsigned numberOfFrames;
    unsigned frameInterval;
    int worldWidth;
    int worldHeight;
    void advanceFrame(Uint32 ticks);
    bool full;
    bool willDraw;
//    std::vector<Image *> frame;
    std::vector<Image *> idle;
    std::vector<Image *> run;
    std::vector<Image *> shooting;
    std::vector<Image *> dead;
    std::vector<Image *> duck;
    std::vector<Image *> jumps;
    bool control;
    
    bool drawOP;
    int color;
    const float maxJump = -500;
    float currJump = 0;
    float time = 0;
    float elapsedSeconds = 0;
    float elapsedMinutes = 0;
    bool godMode;
    bool hasChangedDuck = false;
};
#endif
