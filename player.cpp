#include "player.h"
#include "renderContext.h"
#include "Level.h"
#include "ioMod.h"
#include <sstream>

void Player::advanceFrame(Uint32 ticks) {
    timeSinceLastFrame += ticks;
    if (timeSinceLastFrame > frameInterval) {
        currentFrame = (currentFrame+1) % numberOfFrames;
        timeSinceLastFrame = 0;
    }
}

Player::~Player(){
    
}
Player::Player( const std::string& name) :
Drawable(name,
         Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                  Gamedata::getInstance().getXmlInt("world/height") - 240),
         Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                  Gamedata::getInstance().getXmlInt(name+"/speedY"))
         ),
  collision(false),
  initialVelocity(getVelocity()),
bulletName( Gamedata::getInstance().getXmlStr(name+"/bullet") ),
bullets(),
freeB(),
minSpeed( (Gamedata::getInstance().getXmlInt(name+"/speedX")) + 20 ),
bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
timeSinceLastFrame(0),
timeSinceLastBullet(0),
images( RenderContext::getInstance()->getImages(name)),
state(IDLE),
direction(RIGHT),
currentFrame(0),
numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames")),
frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
full(false),
willDraw(true),
idle(RenderContext::getInstance()->getImages("cupheadidle")),
run(RenderContext::getInstance()->getImages("cupheadrun")),
shooting(RenderContext::getInstance()->getImages("cupheadshoot")),
dead(RenderContext::getInstance()->getImages("cupheaddead")),
duck(RenderContext::getInstance()->getImages("cupheadduck")),
jumps(RenderContext::getInstance()->getImages("cupheadjump")),
control(true),
drawOP(true),
color(0),
godMode(true)
{
    float deltaX = getScaledWidth();
    float deltaY = getScaledHeight()/2;
    for (int i = 0; i < 8; i++){
        Bullet bullet(bulletName);
        bullet.setPosition( getPosition() +Vector2f(deltaX, deltaY) );
        bullet.setVelocity( getVelocity() + Vector2f(minSpeed, 0) );
        freeB.emplace_back( bullet );
    }
    std::cout << "First Player Initialized" << std::endl;
    std::cout << "bullets in constructor: " << freeB.size() << std::endl;
}



Player& Player::operator=(const Player& s) {
  Drawable::operator=(s);
  collision = s.collision;
  initialVelocity = s.initialVelocity;
  return *this;
}

void Player::stop() {
    if(!control) return;
  setVelocityX(0);
}

void Player::right() {

    if(state == DUCK){
        direction = RIGHT;
        return;
    }
    if(state != JUMP){
        state = RUN;
        
    }
    if ( getX() < worldWidth-(getScaledWidth() * 2)) {
        direction = RIGHT;
        setVelocityX(initialVelocity[0]);
    }
} 
void Player::left()  {
    if(state == DUCK){
        direction = LEFT;
        return;
    }
    if(state != JUMP){
        state = RUN;
        
    }
    if ( getX() > 0) {
        direction = LEFT;
        setVelocityX(-initialVelocity[0]);
        
    }
} 
void Player::jump()    {
    if(state != DUCK){
        currJump = -300;
        state = JUMP;
        setVelocityY(currJump);
        setJumping(true);
        setFalling(false);
    }
}
void Player::down()  {
    if(state != DUCK && state != SHOOTING){
        state = DUCK;

    }
}
void Player::shoot() {
    if(state == DUCK){
        return;
    }
    if(state != JUMP){
        state = SHOOTING;
    }
    
    if ( timeSinceLastBullet < bulletInterval ) return;
    float deltaX = getScaledWidth()*10/9;
    float deltaY = getScaledHeight()/4;
    SDLSound::getInstance().shoot();
    // I need to add some minSpeed to velocity:
    if(freeB.size() >= 1 ){
        Bullet bullet = freeB.front();
        freeB.pop_front();
        if(direction == LEFT){
            bullet.setVelocity( -getVelocity() - Vector2f(minSpeed, 0) );
            bullet.setPosition( getPosition() +Vector2f(0, deltaY) );
        }else{
            bullet.setVelocity( getVelocity() + Vector2f(minSpeed, 0) );
            bullet.setPosition( getPosition() +Vector2f(deltaX, deltaY) );
        }
        bullets.push_back( bullet );
        timeSinceLastBullet = 0;
    }else{
        
    }
}
void Player::goidle(){
    if(state != JUMP){
        state = IDLE;
    }
}
void Player::die(){
    if(state != DEAD){
    std::cout << "YOU DIED" << std::endl;
    goidle();
    control = false;
    state = DEAD;
    setJumping(false);
    setFalling(false);
    for (auto bullet : bullets){
        bullet.setVelocityX(0);
    }
    setVelocityX(0);
    if ( getY() - 250  < worldHeight-getScaledHeight()) {
        setVelocityY( -100 );
    }
    SDLSound::getInstance().death();
    }
}
void Player::draw() const{
    if(!full){
        images[currentFrame]->draw(getX(), getY(), getScale());
    }else{
        images[currentFrame]->draw2(getX(), getY());
    }
    for (const Bullet& bullet : bullets ) {
        bullet.draw();
    }
    drawObjectPool();
}

void Player::drawObjectPool() const{
    if(drawOP){
        SDL_Color background = {71,82,73,230};
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
        SDL_Renderer* renderer;
        renderer = RenderContext::getInstance()->getRenderer();
        // first create and draw the border
        SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
        SDL_Rect borderR = {(130-5), (10 - 5), (180 + 10), (90 + 10)};
        SDL_RenderFillRect(renderer, &borderR);
        
        SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
        SDL_Rect backgroundR = {(130), (10), (180), (90)};
        SDL_RenderFillRect(renderer, &backgroundR);
        // then write the text.
        std::stringstream pool;
        
        pool << "Bullets in pool: " << bullets.size();
        
        std::stringstream free;
        free << "Free Bullets: " << freeB.size();
        std::stringstream time;
        time << "Time " << elapsedMinutes  << ((elapsedSeconds <= 9) ? (":0") : (":")) << elapsedSeconds ;
        
        IOmod::getInstance().writeText(time.str(), 140, 17);
        IOmod::getInstance().writeText(pool.str(), 140, 32);
        IOmod::getInstance().writeText(free.str(), 140, 47);
        std::stringstream god;
//        (godMode) ? god = "God Mode!" : "";
        if (godMode){
            god << "God Mode!";
        }else{
            god << " ";
        }
        IOmod::getInstance().writeText(god.str(), 140, 77);
    }
}

void Player::update(Uint32 ticks) {
    advanceFrame(ticks);
    timeSinceLastBullet += ticks;
    timeSinceLastFrame += ticks;
    if(state == JUMP){
//        std::cout << "JUMP" << std::endl;
        images = jumps;
        alterImage(15, 240);
    }else if(state == RUN){
//        std::cout << "RUN" << std::endl;
        images = run;
        alterImage(16, 90);
    }else if(state == SHOOTING){
//        std::cout << "SHOOTING" << std::endl;
        images = shooting;
        alterImage(7, 120);
    }else if(state == DEAD){
//        std::cout << "DEAD" << std::endl;
        images = dead;
        alterImage(18, 180);
    }else if(state == DUCK) {
//        std::cout << "DUCKING" << std::endl;
    
        images = duck;
        setDuckingImages();
        alterImage(6, 180);
        
    }else{
//        std::cout << "IDLE" << std::endl;
        alterImage(10, 120);
        images = idle;
    }
    if(state != DUCK){
        notDucking();
    }
    if(direction == LEFT){
        for (auto &image : images){
            image->setLeft(true);
        }
    }else{
        for (auto &image : images){
            image->setLeft(false);
        }
    }
  if ( !collision ) advanceFrame(ticks);

    auto it = bullets.begin();
    while ( it != bullets.end() ) {
        if ( it->goneTooFar() ) {
//            Bullet* mia = *it;
//            player[currentSprite]->detach(doa);
//            delete doa;
            it->reset();
            freeB.push_back(*it);
            it = bullets.erase(it);
        }
        else {
            it->update(ticks);
            ++it;
        }
    }
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
//    if ( getY() < 0) {
//        setVelocityY( /*fabs( getVelocityY() )*/0 );
//    }
    
    if (state == DEAD) { // above
        if(getY() < -250){
            //reset level
//            std::cout << "RESET" << std::endl;

            state = RESET;
        }
    }

    float deltaTime = ticks * 0.001;
    time+= deltaTime;
    if(time >= 1.0){
//        std::cout<< "1 Sec" << std::endl;
        elapsedSeconds += 1.0;
        time -= 1.0;
        if(elapsedSeconds == 60.0){
            elapsedSeconds -= 60.0;
            elapsedMinutes += 1.0;
        }
    }
//    std::cout << deltaTime << std::endl;
    
//    float jumpinc = (ticks) - getVelocityY();
    if(isJumping()){
        if( currJump > maxJump ){
//            std::cout /<< "Jumping " << currJump << std::endl;
            currJump -= ticks;
            setVelocityY(currJump);
            
        }else{
            setJumping(false);
            setFalling(true);
//            std::cout << "Done jumping" << std::endl;
            
        }
        
    }
    if(isFalling()){
//        std::cout << "Falling" << std::endl;
        
        currJump += ticks;
        setVelocityY(currJump);
    }
    if ( (getY()) > worldHeight-(240) && state != DUCK && state != DEAD) { // below
//        std::cout<< "below" << std::endl;
        setVelocityY( /*-fabs( getVelocityY() ) */ 0);
        setY(worldHeight - 245);
        setFalling(false);
        
        state = IDLE;
    }
    
    if ( getX() < 0) {
        setX(0);
        setVelocityX( /*fabs( getVelocityX() )*/0 );
    }
    if ( getX() > worldWidth-(getScaledWidth() * 2)) {
        setX(worldWidth-(getScaledWidth() * 2));
        setVelocityX( /*-fabs( getVelocityX() )*/0 );
    }
    
    
    if(getVelocityX() == 0 && getVelocityY() == 0){
        if(state != SHOOTING && state != DEAD && state != DUCK){
            images = idle;
            state = IDLE;
        }
    }
    
  stop();
}

