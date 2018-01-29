//
//  Level.h
//  ColorCup
//
//  Created by Joey Costa on 10/28/17.
//  Copyright © 2017 Joey Costa. All rights reserved.
//
// initialize a custom world (This will also be used to "reset" the world when the player dies) currently a placeholder
#ifndef Level_h
#define Level_h

#include <stdio.h>
#include "hud.h"
#include "viewport.h"
#include "sound.h"

class Player;
class CollisionStrategy;
class SmartSprite;
class SubjectSprite;
class MultiSprite;
class World;
class MultiWorld;
class Sprite;
class Level {
public:
    Level();
    ~Level();
    Level(const Level&);
    Level& operator=(const Level&);

    void draw() const;
    void update(Uint32);
    //void useSprite(int n);
    void changeBackground();
    SubjectSprite* getPlayer(){
        return player;
    }
//    int getCurrSprite() const{
//        return currentSprite;
//    }
    
    int getCurrBack() const{
        return currentBack;
    }
    int getCurrStrategy() const {
        return currentStrategy;
    }
    SDLSound getSound() const{
        return sound;
    }
   
    void freeze();
    Hud hud;
private:
    std::vector< std::vector<World> > world; // a multi-dimensional vector array to hold world data
    std::vector< std::vector<MultiWorld *> > animatedWorlds;// a multi-dimensional vector array to hold animated world data
    SubjectSprite * player;
    std::vector<SmartSprite *> sprites;
    std::vector<Sprite *> dumbSprites;
    std::vector<MultiSprite *> animatedSprites;
    std::vector<CollisionStrategy *> strategies;
    MultiSprite *intro;
    Viewport& viewport;
    SDLSound sound;
   // int currentSprite;
    int currentBack;
    int currentStrategy;
    void checkForCollisions();
    
};

#endif /* Level_h */
