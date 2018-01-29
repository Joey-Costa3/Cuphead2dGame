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

//const SDL_Color white = {255, 255, 255, 1};

Engine::~Engine() {
    delete level;
   // delete rc;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),

level( new Level() ),
//viewport( Viewport::getInstance() ),

collision(false),
  makeVideo( false ),
jumping(false)
{

//    strategies.push_back( new PerPixelCollisionStrategy );
    Viewport::getInstance().setObjectToTrack(level->getPlayer());
    std::cout << "Game engine initialized" << std::endl;
}

void Engine::draw() const {

    level->draw();
    //viewport.draw();
  SDL_RenderPresent(renderer);
    
}
void Engine::update(Uint32 ticks) {

    level->update(ticks);
  //viewport.update(); // always update viewport last
}



void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;
//    bool right = true;
//    bool left = false;
    bool flag = false;
    bool toggle = false;
    bool music = false;
//    bool nextSong = false;
//    bool prevSong = false;
   // bool shooting = false;
  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
       
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
            if ( clock.isPaused() ) {clock.unpause();SDLSound::getInstance().toggleStart();}
            else {clock.pause(); SDLSound::getInstance().toggleStop(); }
        }
        if ( keystate[SDL_SCANCODE_R] && level->getPlayer()->canControl()) {
            level->getPlayer()->die();
        }
          if(keystate[SDL_SCANCODE_F1] && !toggle && level->getPlayer()->canControl()){
              level->hud.toggleHud();
              level->getPlayer()->toggleOP();
              
              toggle = true;
          }
          if(keystate[SDL_SCANCODE_G]){
              level->getPlayer()->toggleImmortality();
          }
          
//          if ( keystate[SDL_SCANCODE_L] ) {
//              level = new Level();
//              Viewport::getInstance().setObjectToTrack(level->getPlayer());
//              SDLSound::getInstance().toggleStop();
//              SDLSound::getInstance().playIntro();
//          }
          if(keystate[SDL_SCANCODE_LSHIFT] && !flag && level->getPlayer()->canControl()){
              level->changeBackground();
              level->getPlayer()->changeColor(level->getCurrBack());

              flag = true;
          }
          if ( keystate[SDL_SCANCODE_H] && !music) {
              SDLSound::getInstance().toggleMusic();
              music = true;
          }
//          if ( keystate[SDL_SCANCODE_B] && !prevSong && level->getPlayer()->canControl()) {
//              SDLSound::getInstance().prev();
//              prevSong = true;
//          }
//    
//          if ( keystate[SDL_SCANCODE_N] && !nextSong && level->getPlayer()->canControl()) {
//              SDLSound::getInstance().next();
//              nextSong = true;
//          }
          if ( keystate[SDL_SCANCODE_SPACE] && level->getPlayer()->canControl() && !level->getPlayer()->isJumping() && !level->getPlayer()->isFalling()) {
              level->getPlayer()->shoot();
             // shooting = true;
          }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
        if(event.type == SDL_KEYUP){

            if ( event.key.keysym.sym == SDLK_LSHIFT ) {
                flag = false;
            }
            if ( event.key.keysym.sym == SDLK_SPACE  && level->getPlayer()->canControl() && level->getPlayer()->getState() != DUCK) {
                level->getPlayer()->goidle();
            }
            if ( event.key.keysym.sym == SDLK_s  && level->getPlayer()->canControl() && !level->getPlayer()->isFalling() && !level->getPlayer()->isJumping()) {
                level->getPlayer()->goidle();
            }
            if ( event.key.keysym.sym == SDLK_F1 ) {
                toggle = false;
            }
            if (event.key.keysym.sym == SDLK_h){
                music = false;
            }
//            if (event.key.keysym.sym == SDLK_b){
//                prevSong = false;
//            }
//            if (event.key.keysym.sym == SDLK_n){
//                nextSong = false;
//            }
        }
    }

    // In this section of the event loop we allow key bounce:
      if(level->getPlayer()->getState() == DEAD){
          level->freeze();
      }
      if(level->getPlayer()->getState() == RESET){
          delete level;
          level = new Level();
          std::cout << "New Level" << std::endl;
          Viewport::getInstance().setObjectToTrack(level->getPlayer());
//          SDLSound::getInstance().toggleStop();
          SDLSound::getInstance().playIntro();
      }
    ticks = clock.getElapsedTicks();

    if ( ticks > 0 ) {
      clock.incrFrame();
        
       
        if (keystate[SDL_SCANCODE_W] && level->getPlayer()->canControl() && !level->getPlayer()->isJumping() && !level->getPlayer()->isFalling()) {
            level->getPlayer()->jump();
        }
        if (keystate[SDL_SCANCODE_S] && level->getPlayer()->canControl() && !level->getPlayer()->isJumping() && !level->getPlayer()->isFalling()) {
            level->getPlayer()->down();
        }
        if (keystate[SDL_SCANCODE_A] && level->getPlayer()->canControl()) {
            level->getPlayer()->left();
        }
        if (keystate[SDL_SCANCODE_D] && level->getPlayer()->canControl()) {
            level->getPlayer()->right();
        }
//        if(level->getPlayer()->getVelocityX() == 0 && level->getPlayer()->getVelocityY() == 0 && !shooting){
//
//        }
        
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
        
    }
  }
}
