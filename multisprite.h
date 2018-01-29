#ifndef MULTISPRITE__H
#define MULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class MultiSprite : public Drawable {
public:
  MultiSprite(const std::string&, bool full);
  MultiSprite(const MultiSprite&, bool full);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }
     void isLastFrame() {
        if(currentFrame + 1 == numberOfFrames) { willDraw = false;}
    }
    bool shouldDraw() {
        return willDraw;
    }
protected:
  std::vector<Image *> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  void advanceFrame(Uint32 ticks);
  MultiSprite& operator=(const MultiSprite&);
    bool full;
    bool willDraw;
};
#endif
