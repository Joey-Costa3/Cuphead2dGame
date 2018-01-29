#ifndef IMAGE__H
#define IMAGE__H

#include <iostream>
#include <string>
#include <SDL.h>

class Image {
public:
  Image( SDL_Surface*, SDL_Texture* );
  Image(const Image&);
  Image& operator=(const Image&);

  void draw(int x, int y) const;
  void draw(int x, int y, float scale) const;
  void draw(int sx, int sy, int dx, int dy) const;
    void draw2(int x, int y) const;
  inline int getWidth()  const { return surface->w; }
  inline int getHeight() const { return surface->h; }
  SDL_Surface* getSurface() const { return surface; }
  Image* crop(SDL_Rect)const;
    void setLeft(bool t);
private:
  SDL_Renderer * renderer;
  SDL_Surface * surface;
  SDL_Texture * texture;
  SDL_Rect view;
    bool left;
  Image();
};

#endif
