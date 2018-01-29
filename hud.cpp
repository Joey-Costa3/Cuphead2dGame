//
//  hud.cpp
//  ColorCup
//
//  Created by Joey Costa on 10/25/17.
//  Copyright © 2017 Joey Costa. All rights reserved.
//

#include "hud.h"
#include "renderContext.h"
#include <SDL2/SDL.h>
#include "ioMod.h"
// hud init

Hud::Hud(const Hud &h) :
controls(h.controls),
renderer(h.renderer),
size(h.size),
show(h.show)
{}
Hud& Hud::operator=(const Hud &h){
    std::cout << h.controls[0] << std::endl;
    return *this;
}
Hud::Hud():
    controls(
             std::vector<std::string> {
                 "Left: A",
                 "Right: D",
                 "Up: W",
                 "Down: S",
                 "Shift: Shift",
                 "Reset: R",
                 "Hud: F1",
                 "Music: H"
                 
             }
         ),

 renderer( RenderContext::getInstance()->getRenderer() ),
size(SDL_Rect{10, 10, 110, 140}),
show(true)
{
    const int fontsize = (IOmod::getInstance().getFontHeight()) - 2 ;
    
    int height = 15;
//    for (auto &control: controls){
//       // (show) ? std::cout << control << std::endl : std::cout <<"hide" << std::endl;
//        height += fontsize;
//    }
    for (unsigned i = 0; i < controls.size(); i++){
        height += fontsize;
    }
    resize(size.w, height);

}


void Hud::toggleHud(){
    show = !show;
    (show) ? std::cout << "show" << std::endl : std::cout <<"hide" << std::endl;
}
void Hud::print() const{
    (show) ? std::cout << "show" << std::endl : std::cout <<"hide" << std::endl;
    std::cout << "x: " << size.x <<  " y: " << size.y << std::endl;
    std::cout << "w: " << size.w << " h: " << size.h << std::endl;
    
}
void Hud::resize(int w, int h){
    size.w = w;
    size.h = h;
}
void Hud::draw(int color) const{
    if(show){
        SDL_Color background = {70,70,70,230};
        SDL_Color border = {171,182,173,230};

        switch(color){
            case 0: // default
                background = {71,82,73,230};
                border = {171,182,173,230};

                break;
            case 1: // blue
                 background = {0,62,93,230};
                 border = {0,162,193,230};

                break;
            case 2: // orange
                 background = {97,10,10,230};
                 border = {197,60,10,230};

                break;
                
            default:
                // leave the color alone
                break;
        }
        
        const int fontsize = (IOmod::getInstance().getFontHeight()) - 2;
                // first create and draw the border
        SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
        SDL_Rect borderR = {(size.x - 5), (size.y - 5), (size.w + 10), (size.h + 10)};
        SDL_RenderFillRect(renderer, &borderR);

        SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
        SDL_Rect backgroundR = size;
        SDL_RenderFillRect(renderer, &backgroundR);
        // then write the text.
        int dist = 0;
        for (auto &control : controls){
            IOmod::getInstance().writeText(control, size.x + 10, size.y + 10 + dist);
            dist += fontsize;
            
        }
    }
}
SDL_Rect Hud::getSize(){
    return size;
}
