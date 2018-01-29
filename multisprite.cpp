#include "multisprite.h"
#include "gamedata.h"
#include "renderContext.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

MultiSprite::MultiSprite( const std::string& name, bool full) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                     Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( RenderContext::getInstance()->getImages(name) ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
full(full),
willDraw(true)
{ }

MultiSprite::MultiSprite(const MultiSprite& s, bool full) :
  Drawable(s), 
  images(s.images),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
full(full),
willDraw(true)
  { }

MultiSprite& MultiSprite::operator=(const MultiSprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
    full = (s.full);
    willDraw = (s.willDraw);
  return *this;
}

void MultiSprite::draw() const {
    if(!full){
        images[currentFrame]->draw(getX(), getY(), getScale());
    }else{
        images[currentFrame]->draw2(getX(), getY());
    }
}

void MultiSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
      
    setVelocityY( /*fabs( getVelocityY() )*/0 );
  }
  if ( getY() > worldHeight-(getScaledHeight() * 2)) {
 
    setVelocityY( /*-fabs( getVelocityY() ) */ 0);
  }

  if ( getX() < 0) {
      setX(0);
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-(getScaledWidth() * 2)) {
      //setX(worldWidth-(getScaledWidth() * 2));
    setVelocityX( -fabs( getVelocityX() ) );
  }  

}
