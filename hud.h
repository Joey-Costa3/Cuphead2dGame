//
//  hud.h
//  ColorCup
//
//  Created by Joey Costa on 10/25/17.
//  Copyright © 2017 Joey Costa. All rights reserved.
//

#ifndef hud_h
#define hud_h

#include <stdio.h>
#include <string>
#include <vector>

#include <SDL.h>
class Hud{
public:
    Hud();
    Hud(const Hud&);
    Hud& operator=(const Hud&);

    void toggleHud();
    SDL_Rect getSize();
    void resize(int w, int h);
    void draw(int color) const;
    void print() const;
    bool shouldShow() const{
        return show;
    }
private:
    std::vector<std::string> controls;
    SDL_Renderer* renderer;
    SDL_Rect size; // x y w h
    bool show;
};


#endif /* hud_h */
