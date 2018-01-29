//
//  multiworld.cpp
//  ColorCupMake
//
//  Created by Joey Costa on 10/27/17.
//  Copyright © 2017 Joey Costa. All rights reserved.
//

#include "multiworld.h"
#include "gamedata.h"
#include "renderContext.h"
//
void MultiWorld::advanceFrame(Uint32 ticks) {
    timeSinceLastFrame += ticks;
    if (timeSinceLastFrame > frameInterval) {
        currentFrame = (currentFrame+1) % numberOfFrames;
        timeSinceLastFrame = 0;
    }
}
MultiWorld::MultiWorld(const std::string& name, int fact):
images(RenderContext::getInstance()->getImages(name)),
factor(fact),
currentFrame(0),
numberOfFrames(Gamedata::getInstance().getXmlInt(name+"/frames")),
frameInterval(Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
timeSinceLastFrame(0),
worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
imageWidth(images[0]->getWidth()),
viewX(0.0), viewY(0.0),
view(Viewport::getInstance())
{

}



void MultiWorld::draw() const {
    images[currentFrame]->draw(0,0,-viewX,-viewY);
    images[currentFrame]->draw(0,0,imageWidth-viewX,-viewY);
     
}

void MultiWorld::update(Uint32 ticks) {
    advanceFrame(ticks);

    viewX = view.getX();
    viewY = view.getY();

}

