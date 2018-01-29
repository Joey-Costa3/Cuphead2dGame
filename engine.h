#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "multiworld.h"
#include "Level.h"

class Player;
class CollisionStrategy;
class SmartSprite;
class SubjectSprite;
class MultiSprite;

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
    Engine(const Engine&) = default;
    Engine& operator=(const Engine&) = delete;
private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;

  SDL_Renderer * const renderer;

    Level *level;
    //Viewport& viewport;

    bool collision;
  bool makeVideo;
    bool jumping;
  void draw() const;
  void update(Uint32);

 
  void printScales() const;
  void checkForCollisions();
};
