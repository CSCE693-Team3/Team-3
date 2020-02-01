#define SOL_ALL_SAFETIES_ON 1
#include "Game.hpp"
#include <sol/sol.hpp>
#include <iostream>
#include <memory>

extern "C" {
#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>
}

int main() {


   std::cout << "Creating game" << std::endl;
   auto game = std::make_unique<Game>("1st Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

   std::cout << "Starting game loop" << std::endl;
   while (game->running()) {
      game->handle_events();
      game->update();
      game->render();
   }

   return 0;
}

