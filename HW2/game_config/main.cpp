
#include "Game.hpp"
#include <iostream>
#include <memory>

//Lua written in C so use extern
extern "C" {
#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>
}

int main() {

   std::cout << "Creating game" << std::endl;
   // read Lua script to determine initial window size infomation

   //create a new LUA state
   lua_State *lua_state = luaL_newstate();

   //load all LUA libs
   luaL_openlibs(lua_state);

   //use loadfile to load config.lua, and pcall for errors
   luaL_loadfile(lua_state, "config.lua");
   lua_pcall(lua_state, 0, 0, 0);

   //get the global variables from config.lua and push onto stack
   lua_getglobal(lua_state, "xpos");
   lua_getglobal(lua_state, "ypos");
   lua_getglobal(lua_state, "width");
   lua_getglobal(lua_state, "height");
   lua_getglobal(lua_state, "fullscreen");


   //get global variable from lua stack and put them in variables in c++
   bool fullscreen = lua_toboolean(lua_state, -1);
   int height = lua_tonumber(lua_state, -2);
   int width = lua_tonumber(lua_state, -3);
   int ypos = lua_tonumber(lua_state, -4);
   int xpos = lua_tonumber(lua_state, -5);


   //For DEBUG only, uncomment to see changes in terminal
   std::cout << "fullscreen is: " << fullscreen << std::endl;
   std::cout << "height is: " << height << std::endl;
   std::cout << "width is: " << width << std::endl;
   std::cout << "xpos is: " << xpos << std::endl;
   std::cout << "ypos is: " << ypos << std::endl;

   auto game = std::make_unique<Game>("1st Game", xpos, ypos, width, height, fullscreen);

   std::cout << "Starting game loop" << std::endl;
   while (game->running()) {
      game->handle_events();
      game->update();
      game->render();
   }

  //close LUA state
  lua_close(lua_state);
  return 0;
}

