
#ifndef __Game_HPP__
#define __Game_HPP__

#include "SDL2/SDL.h"
#include "GameObject.hpp"

#include <vector>
#include <memory>
#include <unordered_map>
#include <tuple>


class Game
{
public:
   Game(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
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
   std::vector<std::unique_ptr<GameObject>> game_objs;
   //storage for reading in config.lua file
   std::unordered_map<std::string, std::tuple<std::string, float, float, float, float>> playerConfig;
};

#endif

