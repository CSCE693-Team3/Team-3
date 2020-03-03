
#include "Game.hpp"
#include <sol/sol.hpp>
#include <iostream>
#include "SDL2/SDL_image.h"
#include <string>
#include "sdl_utils.hpp"
#include "gameobjects/Chopper.hpp"
#include "gameobjects/Tank.hpp"
#include "gameobjects/Pacman.hpp"
#include <stdexcept>

SDL_Renderer* Game::renderer{};
SDL_Window* Game::window{};
//variable for sol state
sol::state lua;


Game::Game(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
   Uint32 flags{};
   if (fullscreen) {
      flags = SDL_WINDOW_FULLSCREEN;
   }

   if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
      std::cout << "Subsystems initialized..." << std::endl;
      window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
      if (window) {
         std::cout << "Window created..." << std::endl;
      }
      renderer = SDL_CreateRenderer(window, -1, 0);
      if (renderer) {
         SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
         std::cout << "Renderer created..." << std::endl;
      }
   is_running = true;
   } else {
      is_running = false;
   }

   //initialze sol::state and open libraries
   lua.open_libraries(sol::lib::base, sol::lib::package);

   //Open the script file, this one being config.lua
   //And catch any errors
   try
   {
	   lua.script_file("config.lua");
	   
   }
   catch (int e)
   {
	   throw std::out_of_range{ "Cannot open config file"};
	   this->~Game();
   }
  
  

   //Read in the gameobjs table, which is a table of key-value pairs, and the value
   //is another table of values
   sol::table gameobj = lua["gameobjs"];

   //Iterate over each key (the gameobjec["players"])
   for (const auto& key_value_pair : gameobj) {
      sol::object k = key_value_pair.first;
      sol::object v = key_value_pair.second;
      
      sol::type t = v.get_type();

      switch(t) {
         case sol::type::table: {
            //First get the value for each player, these are tables (nested in the table of "gameobjs")
            sol::table nested = key_value_pair.second;
            //Get the player's name from the key value that the iterator is on
            std::string playerName = k.as<std::string>();
            //Now get the different variables from the nested table for each player,
            //which is the kind (chopper, tank, etc), positions, and velocities
            std::string kind = static_cast<std::string>(nested["kind"]);
            float xpos = static_cast<float>(nested["xpos"]);
            float ypos = static_cast<float>(nested["ypos"]);
            float xvel = static_cast<float>(nested["xvel"]);
            float yvel = static_cast<float>(nested["yvel"]);

            //Add the player config that was read in, to the playerConfig unordered_map
            playerConfig.emplace(playerName, std::make_tuple(kind, xpos, ypos, xvel, yvel));

         break;
         }
         default:
            std::cout << "Unkown type... no players loaded." << std::endl;
            break;
      }
      
      
   }

   
}

Game::~Game()
{
   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
   SDL_Quit();
}

void Game::load_level()
{
   

   for(auto players : playerConfig) {

      //There's no current storage for the players name, but keeping this here for future expansion
      std::string name = players.first;

      //Get the kind of object from the playerConfig
      std::string kind = std::get<0>(players.second);

      //Now compare to the 3 types of game objects, and create the objects using the saved config
      //And emplace them into the game_objs vector
      if(kind == "chopper") {
         auto chopper = std::make_unique<Chopper>(std::get<1>(players.second), std::get<2>(players.second), std::get<3>(players.second), std::get<4>(players.second));
         game_objs.emplace_back(std::move(chopper));
      }
      if(kind == "tank") {
         auto tank = std::make_unique<Tank>(std::get<1>(players.second), std::get<2>(players.second), std::get<3>(players.second), std::get<4>(players.second));
         game_objs.emplace_back(std::move(tank));
      }
      if(kind == "pacman") {
         auto pacman = std::make_unique<Pacman>(std::get<1>(players.second), std::get<2>(players.second), std::get<3>(players.second), std::get<4>(players.second));
         game_objs.emplace_back(std::move(pacman));
      }
   }
   
}

void Game::handle_events()
{
   SDL_Event event;
   SDL_PollEvent(&event);
   switch (event.type) {
      case SDL_QUIT:
         is_running = false;
         break;
      default:
         break;
   }
}

void Game::update(const float dt)
{
   for (auto& i: game_objs) {
      i->update(dt);
   }
}

void Game::render()
{
   SDL_RenderClear(renderer);
   for (auto& i: game_objs) {
      i->render();
   }
   SDL_RenderPresent(renderer);
}

