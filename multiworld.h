//
//  multiworld.h
//  ColorCupMake
//
//  Created by Joey Costa on 10/27/17.
//  Copyright © 2017 Joey Costa. All rights reserved.
//

#ifndef multiworld_h
#define multiworld_h

#include <stdio.h>
#include <string>
#include <vector>
#include <cmath>
#include "viewport.h"
#include "image.h"

class MultiWorld {
public:
    MultiWorld(const std::string&, int fact);
    MultiWorld(const std::string&, int fact, bool full);

    //MultiWorld(const MultiWorld&); not implemented
    //~MultiWorld() { } // the images will be deleted by the FrameFactory
    void draw() const;
    void update(Uint32 ticks);
    
    const Image* getImage() const {
        return images[currentFrame];
    }
  
    //  this may not be needed by multiworld
//    const SDL_Surface* getSurface() const {
//        return images[currentFrame]->getSurface();
//    }
    
   
protected:
    // multi specific stuff
    std::vector<Image *> images;
    int factor;
    unsigned currentFrame;
    unsigned numberOfFrames;
    unsigned frameInterval;
    float timeSinceLastFrame;
    int worldWidth;
    int worldHeight;
    unsigned imageWidth;
    void advanceFrame(Uint32 ticks);
    MultiWorld& operator=(const MultiWorld&);
    // world specific stuff
    float viewX;
    float viewY;
    const Viewport & view;
};
#endif /* multiworld_h */
