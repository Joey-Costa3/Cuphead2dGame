#include "drawable.h"
#include "image.h"
#include "ioMod.h"
#include "viewport.h"
#include "renderContext.h"


Image::Image( SDL_Surface* surf, SDL_Texture* tex ) : 
  renderer(RenderContext::getInstance()->getRenderer()),
  surface( surf ),
  texture( tex ),
  view{0,0,surf->w,surf->h},
left(false)
{ }

Image::Image( const Image& image ) :
  renderer(image.renderer),
  surface(image.surface), 
  texture(image.texture),
  view(image.view),
left(false)
{ }


Image& Image::operator=(const Image& rhs) {
  renderer = rhs.renderer;
  texture = rhs.texture;
  view = rhs.view;
  return *this;
}

void Image::draw(int x, int y) const {
  draw(x,y,1.0f);
}
void Image::setLeft(bool t){
    left = t;
}
void Image::draw(int x, int y, float scale) const {
  x -= Viewport::getInstance().getX();
  y -= Viewport::getInstance().getY();
  int tempHeight = scale*view.h;
  int tempWidth =  scale*view.w;
  SDL_Rect dest =  {x, y, tempWidth, tempHeight};
    if (left)
  SDL_RenderCopyEx(renderer, texture, &view, &dest, 0, NULL, SDL_FLIP_HORIZONTAL);
    else
     SDL_RenderCopy(renderer, texture, &view, &dest);
}

void Image::draw(int sx, int sy, int dx, int dy) const {
  SDL_Rect src = { sx, sy, view.w, view.h };    
  SDL_Rect dst = { dx, dy, getWidth(), getHeight() };
  SDL_RenderCopy(renderer, texture, &src, &dst);
}
// this function draws the entire image based on the viewport
void Image::draw2(int x, int y) const {
    x -= Viewport::getInstance().getX();
    y -= Viewport::getInstance().getY();
    SDL_Rect dest =  {-10, -10, Viewport::getInstance().getWidth(), Viewport::getInstance().getHeight()};
    SDL_RenderCopy(renderer, texture, &view, &dest);
}
Image* Image::crop(SDL_Rect sub)const{
  if(sub.x+sub.w > view.w
  || sub.y+sub.h > view.h){
    std::cerr << "Attempted to crop image with invalid geometry."
              << std::endl
              << "(0,0 + "<<view.w << "x"<<view.h<<")"
              << " --> "
              << "("<<sub.x<<","<<sub.y<<" + "<<sub.w << "x"<<sub.h<<")"
              << std::endl;
    return nullptr;
  }

  Image* cloned = new Image(*this);
  cloned->view = sub;

  return cloned;
}

