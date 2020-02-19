
#ifndef __GAME_HPP__
#define __GAME_HPP__
//#include "GameObject.hpp"
#include "SDL2/SDL.h"
#include <vector>
#include <memory>



class Game {
public:
   Game(const char* , int , int , int , int , bool );
   ~Game();

   void load_level();

   void handle_events();
   void update(const float dt);
   void render();

   bool running()                { return is_running; }

   static SDL_Renderer* renderer;
   static SDL_Window* window;

private:
   int counter{};
   bool is_running{};
};

#endif

