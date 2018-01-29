//
//  Level.cpp
//  ColorCup
//
//  Created by Joey Costa on 10/28/17.
//  Copyright © 2017 Joey Costa. All rights reserved.
//

#include "Level.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "player.h"
#include "frameGenerator.h"
#include "collisionStrategy.h"
#include "smartSprite.h"
#include "subjectSprite.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
const std::vector<std::string> animalOrbs = {
    "Beholder",
    "Bipolarbear",
    "Biteybat",
    "Burlybear",
    "Cardinal",
    "Chicken",
    "Dragon",
    "Frogglet",
    "Giraffey",
    "GoldenWhale",
    "Hawkster",
    "Installball",
    "Meowburt",
    "Monkeyface",
    "Mrbuddy",
    "Owlet",
    "Pazzo",
    "Pelter",
    "Piggy",
    "Rammy",
    "Scratchpaw",
    "Seahorse",
    "Sherbert",
    "Snailburt",
    "Snoot",
    "Spiny",
    "Troll",
    "Yeti",
    "Zebra"
};
Level::~Level(){
    
    delete player;
    for (auto &multiVec : animatedWorlds){
        for(auto &animated : multiVec){
            delete animated;
        }
    }
    for (auto &sprite : sprites){
        delete sprite;
    }
    for (auto &sprite : dumbSprites){
        delete sprite;
    }
    for (auto &animSprite : animatedSprites){
        delete animSprite;
    }
    for (auto &strategy : strategies){
        delete strategy;
    }
    delete intro;
    std::cout << "Delete level data" << std::endl;
}

Level::Level(const Level& l) :
hud(l.hud),
world(l.world),
animatedWorlds(l.animatedWorlds),
player(l.player),
sprites(l.sprites),
dumbSprites(l.dumbSprites),
animatedSprites(l.animatedSprites),
strategies(l.strategies),
intro(l.intro),
viewport(l.viewport),
sound(l.sound),
currentBack(l.currentBack),
currentStrategy(l.currentStrategy)
{ }
Level& Level::operator=(const Level& l){
    std::cout << "=level" << l.world.size() << std::endl;
    return *this;
}
Level::Level() :
hud ( Hud() ),
world(
      std::vector< std::vector<World> > {
      /* 0 background */    { World(std::string("default"),  Gamedata::getInstance().getXmlInt("default/factor")),
              World(std::string("lightblue"),  Gamedata::getInstance().getXmlInt("lightblue/factor")),
              World(std::string("orange"),  Gamedata::getInstance().getXmlInt("orange/factor")) },
      /* 1 middleback */     { World(std::string("middlebackdefault"), Gamedata::getInstance().getXmlInt("middlebackdefault/factor")),
              World(std::string("middlebackblue"), Gamedata::getInstance().getXmlInt("middlebackblue/factor")),
              World(std::string("middlebackorange"), Gamedata::getInstance().getXmlInt("middlebackorange/factor"))},
       /* 2 mountain */    { World(std::string("middleground"), Gamedata::getInstance().getXmlInt("middleground/factor") )},
       /* 3 foreground */    { World(std::string("foregrounddefault"),  Gamedata::getInstance().getXmlInt("foregrounddefault/factor")),
              World(std::string("foregroundsnow"),  Gamedata::getInstance().getXmlInt("foregroundsnow/factor")),
              World(std::string("foregroundfire"),  Gamedata::getInstance().getXmlInt("foregroundfire/factor")) }
      }
),
animatedWorlds(
//              std::vector< std::vector<MultiWorld *> > {
//                  { new MultiWorld("fire" ,1) }
//              }
),
player(
           new SubjectSprite("cupheadidle")
),
sprites(),
dumbSprites(),
animatedSprites( {
    new MultiSprite("icesprite", false),
    new MultiSprite("firesprite", false)
}),
strategies (
            std::vector<CollisionStrategy *> {
                new PerPixelCollisionStrategy
            }
),
intro (
       new MultiSprite("intro", true)
),
viewport( Viewport::getInstance() ),
sound(SDLSound::getInstance()),
currentBack(0),
currentStrategy(0)
{
    
        srand(time(NULL));
      //  hud.draw(currentBack);
    int n = Gamedata::getInstance().getXmlInt("collectibles");
        sprites.reserve(n);
        Vector2f pos = player->getPosition();
        int w = player->getScaledWidth();
        int h = player->getScaledHeight();
        int enemyS = rand() % 29;

    std::cout << "Watch out for the sprite: " << animalOrbs[enemyS] << std::endl;
    for (int i = 0; i < Gamedata::getInstance().getXmlInt("noncollectibles"); i++){
        dumbSprites.push_back(new Sprite(animalOrbs[enemyS]));
    }
        for (int i = 0; i < n; ++i) {
            int s = rand() % 29;
            while (s == enemyS){
                s = rand() % 29;
            }
           // std::cout << "[" << s << "] " << animalOrbs[s] << std::endl;
            sprites.push_back( new SmartSprite(animalOrbs[s], pos, w, h) );
            player->attach( sprites[i] );
        }
    
    std::cout << "Level Initialized >:-/" <<  std::endl;
}

void Level::freeze(){
    for (auto &sprite : sprites){
        //sprite->setVelocity(Vector2f(0, 0)); // causes weird death animation
        sprite->explode();
    }
}


void Level::checkForCollisions() {
    // bullet hit sprite = sprite die
    
    for (int i = 0; i < getPlayer()->bulletCount(); i++){
        auto itB = sprites.begin();
        while ( itB != sprites.end() ) {
            if(strategies[currentStrategy]->execute(getPlayer()->getBullet(i), **itB)){
                SmartSprite* doa = *itB;
                player->detach(doa);
                delete doa;
                itB = sprites.erase(itB);
                SDLSound::getInstance().spriteDeath();
                //std::cout << "Bullet hit sprite" << std::endl;
            }
            else ++itB;
            //++it;
        }
    }
   
    if(!getPlayer()->canDie() && getPlayer()->getState() != DEAD){
   
    // player hit animated sprite -> kill the player
        if(currentBack == 1){
            auto ita = animatedSprites.begin();
            if (strategies[currentStrategy]->execute(*player, **ita)){
//                std::cout << "Player hit ice" << std::endl;
                // ita = animatedSprites.erase(ita);
                getPlayer()->die();
            }
        }
        if(currentBack == 2){
            auto ita = animatedSprites.begin();
            ++ita;
            if (strategies[currentStrategy]->execute(*player, **ita)){
//                std::cout << "Player hit fire" << std::endl;
                //ita = animatedSprites.erase(ita);
                getPlayer()->die();
            }
        }
    // player hit sprite = you die
    auto it = sprites.begin();
    while ( it != sprites.end() ) {
    
        if ( strategies[currentStrategy]->execute(*player, **it) ) {
            SmartSprite* doa = *it;
            player->detach(doa);
            delete doa;
            it = sprites.erase(it);
            getPlayer()->die();

        }
        else ++it;
    }
        auto itd = dumbSprites.begin();
        while (itd != dumbSprites.end()){
            if(strategies[currentStrategy]->execute(*player, **itd)){
                delete *itd;
                itd = dumbSprites.erase(itd);
//                std::cout <<"You hit the dumb sprite" << std::endl;
                getPlayer()->die();
            }else ++itd;
        }
        
    }
//
}
void Level::changeBackground(){
    ++currentBack;
    currentBack = currentBack % world[0].size();
}


void Level::draw() const {
    // first draw the world data
    for( auto &worldVec : world){
        if(worldVec.size() > 1){
            worldVec[currentBack].draw(); // draw the version of the world needed
        }else{
            worldVec[0].draw(); // draw the first one cause its the only one there
        }
    }
    if (currentBack == 1){
        animatedSprites[0]->draw();
    }
    if (currentBack == 2){
        animatedSprites[1]->draw();
    }
    for (auto &animated : animatedWorlds){
        animated[0]->draw();
    }
    // then draw the sprites and the player
    for (auto &sprite : sprites){
        sprite->draw();
    }
    for (auto &sprite : dumbSprites){
        sprite->draw();
    }
    player->draw();
    strategies[currentStrategy]->draw();
    
    //draw the hud
    hud.draw(currentBack);
    std::stringstream spriteWrite;
    
    spriteWrite << "Animal Orbs left: " << sprites.size();
    if(hud.shouldShow()){
        IOmod::getInstance().writeText(spriteWrite.str(), 140, 62);
    }
   
    intro->isLastFrame();
    if(intro->shouldDraw()){
        intro->draw();
    }
    viewport.draw();
    
}
void Level::update(Uint32 ticks){
    checkForCollisions();
    // update in reverse order of draw
    player->update(ticks);
    if(sprites.size() == 0 && getPlayer()->getState() != DEAD && getPlayer()->getState() != RESET){
        getPlayer()->die();
    }
    for (auto &sprite : dumbSprites){
        sprite->update(ticks);
    }
    for (auto &sprite : sprites){
        sprite->update(ticks);
    }
    for (auto &animated : animatedWorlds){
        animated[0]->update(ticks);
    }
//    if(currentBack == 1){
        animatedSprites[0]->update(ticks); // always update the drawing but dont draw it if it shouldnt be shown
//    }
//    if(currentBack == 2){
        animatedSprites[1]->update(ticks);
//    }
    for( auto &worldVec : world){
        if(worldVec.size() > 1){
            worldVec[currentBack].update(); // update the version of the world needed
        }else{
            worldVec[0].update(); // update the first one cause its the only one there
        }
    }
    if(intro->shouldDraw()){
        intro->update(ticks);
    }
    viewport.update();
}
