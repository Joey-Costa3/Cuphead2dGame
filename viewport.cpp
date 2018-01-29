#include <sstream>
#include "viewport.h"
#include "ioMod.h"
#include "clock.h"
Viewport& Viewport::getInstance() {
  static Viewport viewport;
  return viewport;
}

Viewport::Viewport() : 
  gdata(Gamedata::getInstance()),
  position(0, 0),
  worldWidth(gdata.getXmlInt("world/width")),
  worldHeight(gdata.getXmlInt("world/height")),
  viewWidth(gdata.getXmlInt("view/width")), 
  viewHeight(gdata.getXmlInt("view/height")),
  objWidth(0), objHeight(0),
  objectToTrack(NULL) 
{}

void Viewport::setObjectToTrack(const Drawable *obj) { 
  objectToTrack = obj; 
  objWidth = objectToTrack->getScaledWidth();
  objHeight = objectToTrack->getScaledHeight();
}

void Viewport::draw() const {
    SDL_Color white = {255, 255, 255, 1};
    SDL_Color black = {0, 0, 0, 1};

  IOmod::getInstance().changeColor(black);
    //IOmod::getInstance().writeText("We resemble but are legally distinct from MDHR...", 30, 30);
 //IOmod::getInstance().writeText("Tracking:"+objectToTrack->getName(), 30, 30);
    IOmod::getInstance().changeColor(white);
    IOmod::getInstance().writeText("jacosta", 30, Viewport::viewHeight - 90);
    std::string fps = std::to_string(Clock::getInstance().getFps());
    std::stringstream s;
    s << fps;
   // IOmod::getInstance().writeText("fps: "+s.str(), 30, Viewport::viewHeight - 60);
    IOmod::getInstance().writeText("fps: "+fps, 30, Viewport::viewHeight - 60);
}

void Viewport::update() {
  const float x = objectToTrack->getX();
  const float y = objectToTrack->getY();
    //std::cout << x << " " << y << std::endl;
  position[0] = (x + objWidth/2) - viewWidth/2;
  position[1] = (y + objHeight/2) - viewHeight/2;

  if (position[0] < 0) position[0] = 0;
  if (position[1] < 0) position[1] = 0;
  if (position[0] > (worldWidth - viewWidth)) {
    position[0] = worldWidth-viewWidth;
  }
  if (position[1] > (worldHeight - viewHeight)) {
    position[1] = worldHeight-viewHeight;
  }
    //std::cout << position[0] << " " << position[1] << std::endl;
}
