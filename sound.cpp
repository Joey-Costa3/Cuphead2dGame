#include <string>
#include <iostream>
#include <ctime>
#include "sound.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
using std::string;

SDLSound::~SDLSound() {
  std::cout << "Cleaning up sounds ..." << std::endl;
  std::clock_t start = std::clock();
//  Mix_HaltMusic();
//  Mix_FreeMusic(music);
//  for (unsigned int i = 0; i < sounds.size(); ++i) {
//    Mix_FreeChunk(sounds[i]);
//  }
//    for (unsigned int i = 0; i < starting.size(); ++i) {
//        Mix_FreeChunk(starting[i]);
//    }
//    for (unsigned int i = 0; i < ending.size(); ++i) {
//        Mix_FreeChunk(ending[i]);
//    }
//    for (auto & s : sounds){
//        Mix_FreeChunk(s);
//    }
//    for (auto & s : starting){
//        Mix_FreeChunk(s);
//    }
//    for (auto & s : ending){
//        Mix_FreeChunk(s);
//    }
//    Mix_FreeChunk(shooting);
//    shooting = NULL;
//    Mix_FreeChunk(spritePop);
//    spritePop = NULL;
//  Mix_CloseAudio();
  std::clock_t duration = std::clock() - start;
  std::cout << "Clean up took " << duration << " ticks\n";
}
const std::vector<std::string> startingSounds = {
    "sfx_level_announcer_0001_a.wav",
    "sfx_level_announcer_0001_b.wav",
    "sfx_level_announcer_0001_c.wav",
    "sfx_level_announcer_0001_d.wav",
    "sfx_level_announcer_0001_e.wav"
};

SDLSound::SDLSound() :
  volume(SDL_MIX_MAXVOLUME/4),
  currentSound(-1),
  music(NULL),
  audioRate(22050), 
  audioChannels(2), 
  audioBuffers(4096),
  sounds(),
starting(),
ending(),
shooting(),
spritePop(),
  channels()
{

  if(Mix_OpenAudio(audioRate, MIX_DEFAULT_FORMAT, audioChannels, 
                   audioBuffers)){
    throw string("Unable to open audio!");
  }
    int s = rand() % 3;
    if(s == 0){
        music = Mix_LoadMUS("Sound/MUS_FieryFrolic_MixB3.wav");
    }else if(s == 1){
        music = Mix_LoadMUS("Sound/MUS_AviaryAction.wav");
    }else {
        music = Mix_LoadMUS("Sound/MUS_BotanicPanic.wav");
    }
    if (!music) throw string("Couldn't load sound")+Mix_GetError();

    startMusic();
//    sounds.emplace_back( Mix_LoadWAV("sound/MUS_FieryFrolic_MixB3.wav") );
//    sounds.emplace_back( Mix_LoadWAV("sound/MUS_AviaryAction.wav") );
//  sounds.emplace_back( Mix_LoadWAV("sound/MUS_BotanicPanic.wav") );

    for (std::string audio : startingSounds){
    std::cout << audio << std::endl;
        std::string title = "Sound/" + audio;
       starting.emplace_back( Mix_LoadWAV(title.c_str()));
    }
    ending.emplace_back(Mix_LoadWAV("Sound/sfx_player_death_01.wav"));
    ending.emplace_back(Mix_LoadWAV("Sound/sfx_player_death_02.wav"));
    //starting.push_back(Mix_LoadWAV("sound/sfx_level_announcer_0001_a.wav"));
    //playIntro();
//    std::cout << "Init channels" << std::endl;
  for (unsigned int i = 0; i < sounds.size(); ++i) channels.emplace_back(i);
    for (unsigned int i = sounds.size(); i < starting.size() + sounds.size(); ++i) channels.emplace_back(i);
    for (unsigned int i = sounds.size() + starting.size(); i < starting.size() + sounds.size() + ending.size(); ++i) channels.emplace_back(i);
    shooting = Mix_LoadWAV("Sound/sfx_player_weapon_charge_full_fireball_03.wav");
    int s2 = rand() % 3;
    if(s2 == 0){
        spritePop = Mix_LoadWAV("Sound/sfx_frogs_short_death_02.wav");
    }else if (s2 == 1){
        spritePop = Mix_LoadWAV("Sound/sfx_level_baroness_jellybean_death_01.wav");
    }else{
        spritePop = Mix_LoadWAV("Sound/sfx_level_clown_penguin_death_03.wav");

    }
    playIntro();
    std::cout << "Music and Sound is loaded : " << channels.size() << std::endl;
}
void SDLSound::playIntro(){
//    std::cout << "In play intro" << std::endl;
    int s = rand() % startingSounds.size();
    int channelNum = sounds.size() + s;
    Mix_VolumeChunk(starting[s], volume*2);

    channels[channelNum] =
    Mix_PlayChannel(channelNum, starting[s], 0);
//    std::cout << "Finished playing audio call" << std::endl;
//    Mix_ChannelFinished(&SDLSound::channelDone);

}
SDLSound::SDLSound(const SDLSound& s) :
volume(s.volume),
currentSound(s.currentSound),
music(s.music),
audioRate(s.audioRate),
audioChannels(s.audioChannels),
audioBuffers(s.audioBuffers),
sounds(s.sounds),
starting(s.starting),
ending(s.ending),
shooting(s.shooting),
spritePop(s.spritePop),
channels(s.channels)
{
}

SDLSound& SDLSound::getInstance() {
    static SDLSound instance;
    return instance;
}
// make a channelDone function
void SDLSound::channelDone(int channel){
    std::cout << "channel " << channel << " finished playing" << std::endl;
    if(channel > 0 && channel < 5){
        std::cout << "play song" << std::endl;
//        SDLSound::getInstance().next();
    }
}
void SDLSound::death(){
    Mix_HaltChannel(1);
    int x = rand() % ending.size();

//    int channelNum = sounds.size() + starting.size() + x;
//    std::cout << "Play death sound "  << x << " " << channelNum << std::endl;

    Mix_VolumeChunk(ending[x], volume*2);
    channels[1] =
    Mix_PlayChannel(1, ending[x], 0);
}
void SDLSound::shoot(){
    Mix_HaltChannel(0);
//    std::cout << "Play shooting sound " << std::endl;
    
    Mix_VolumeChunk(shooting, volume*2);
    channels[0] =
    Mix_PlayChannel(0, shooting, 0);
}
void SDLSound::spriteDeath(){
    Mix_HaltChannel(2);
//    std::cout << "Play sprite death sound " << std::endl;
    
    Mix_VolumeChunk(spritePop, volume*2);
    channels[2] =
    Mix_PlayChannel(2, spritePop, 0);
}
//void SDLSound::toggleMusic() {
//  if( Mix_Paused(currentSound) ) {
//    Mix_Resume(currentSound);
//  }
//  else {
//    Mix_Pause(currentSound);
//  }
//}
void SDLSound::toggleMusic() {
    if( Mix_PausedMusic() ) {
        Mix_ResumeMusic();
    }
    else {
        Mix_PauseMusic();
    }
}
void SDLSound::toggleStop(){
    Mix_PauseMusic();
}
void SDLSound::toggleStart(){
    Mix_ResumeMusic();
}
void SDLSound::operator[](int index) {
//    std::cout << "test" << std::endl;
  if (currentSound >= 0) Mix_HaltChannel(currentSound);
  currentSound = index;
    load();
}
void SDLSound::load(){
    Mix_VolumeChunk(sounds[currentSound], volume);
    channels[currentSound] =
    Mix_PlayChannel(currentSound, sounds[currentSound], 0);
    
}
void SDLSound::next(){
    
    if (currentSound >= 0) Mix_HaltChannel(currentSound);
//    currentSound = ((++currentSound) % sounds.size());
    int cs = ++currentSound;
    cs = cs % sounds.size();
    currentSound = cs;
//    std::cout << currentSound << " sound to play" << std::endl;
    load();
}
void SDLSound::prev(){

    if (currentSound >= 0) Mix_HaltChannel(currentSound);
//    currentSound = (((--currentSound) + sounds.size()) % sounds.size());
    int cs = --currentSound + sounds.size();
    cs = cs % sounds.size();
    currentSound = cs;
//    std::cout << currentSound << " sound to play" << std::endl;
    load();

}
void SDLSound::startMusic() {
  Mix_VolumeMusic(volume);
  Mix_PlayMusic(music, -1);
}

void SDLSound::stopMusic() {
  Mix_HaltMusic();
  Mix_FreeMusic(music);
}

