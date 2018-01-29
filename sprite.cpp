#include <cmath>
#include <random>
#include <functional>
#include "sprite.h"
#include "gamedata.h"
#include "renderContext.h"

Vector2f Sprite::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx-50,vx+50);
  float newvy = Gamedata::getInstance().getRandFloat(vy-50,vy+50);
  newvx *= [](){ if(rand()%2) return -1; else return 1; }();
  newvy *= [](){ if(rand()%2) return -1; else return 1; }();
  return Vector2f(newvx, newvy);
}

Vector2f Sprite::makeStartLoc(int vx, int vy) const {
    float newvx = Gamedata::getInstance().getRandFloat(vx-150,vx+150);
    float newvy = Gamedata::getInstance().getRandFloat(vy-150,vy+150);
    newvx *= [](){ if(rand()%2) return -1; else return 1; }();
    newvy *= [](){ if(rand()%2) return -1; else return 1; }();
    return Vector2f(newvx, newvy);
}
Sprite::Sprite(const string& n, const Vector2f& pos, const Vector2f& vel,
                Image* img):
  Drawable(n, pos, vel), 
  image( img ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
left(false)
{ }

Sprite::Sprite(const std::string& name) :
  Drawable(name,
           makeStartLoc(Gamedata::getInstance().getXmlInt("animalOrbs/startLoc/x"),
                    Gamedata::getInstance().getXmlInt("animalOrbs/startLoc/y")),
           makeVelocity(
                    Gamedata::getInstance().getXmlInt("animalOrbs/speedX"),
                    Gamedata::getInstance().getXmlInt("animalOrbs/speedY")) 
           ),
  image( RenderContext::getInstance()->getImage(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
left(false)
{

}

Sprite::Sprite(const Sprite& s) :
  Drawable(s), 
  image(s.image),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
left(s.left)
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {
  Drawable::operator=( rhs );
  image = rhs.image;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

inline namespace{
  constexpr float SCALE_EPSILON = 2e-7;
}

void Sprite::draw() const {
  if(getScale() < SCALE_EPSILON) return;
    if(getVelocityX() > 0){
        image->setLeft(false);
    }else{
        image->setLeft(true);
    }
image->draw(getX(), getY(), getScale());

    
}

void Sprite::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( std::abs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-(getScaledHeight() * 2)) {
    setVelocityY( -std::abs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( std::abs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-(getScaledWidth() * 2)) {
    setVelocityX( -std::abs( getVelocityX() ) );

  }
}
