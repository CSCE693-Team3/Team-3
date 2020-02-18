
#ifndef __GameObject_HPP__
#define __GameObject_HPP__

#include "SDL2/SDL.h"

//Abstract Class
class GameObject {
public:
   GameObject(const char*, const float, const float, const float, const float);
   ~GameObject();


   void update(const float dt);
   void render();

private:
   float xpos{}, ypos{};
   float xvel{}, yvel{};

   SDL_Texture* texture{};
   SDL_Rect src_rect, dest_rect;
};

class Tank: public GameObject {
public:
    Tank(const char* Tank_img, const float xpos, const float ypos,
         const float xvel, const float yvel) : GameObject(Tank_img, xpos, ypos, xvel, yvel) {}
    //~Tank();

};

class Chopper: public GameObject {
public:
    Chopper(const char* Chopper_img, const float xpos, const float ypos,
         const float xvel, const float yvel) : GameObject(Chopper_img, xpos, ypos, xvel, yvel) {}

};

class Pacman: public GameObject {
public:
    Pacman(const char* Pacman_img, const float xpos, const float ypos,
            const float xvel, const float yvel) : GameObject(Pacman_img, xpos, ypos, xvel, yvel) {}
};
#endif

