
#include "Game.hpp"
//#include <sol/sol.hpp>
#include <iostream>
#include "SDL2/SDL_image.h"

#include "AssetManager.hpp"
#include "EntityManager.hpp"
#include "Entity.hpp"
#include "components/TransformComponent.hpp"
#include "components/SpriteComponent.hpp"

SDL_Renderer* Game::renderer{};

EntityManager entity_mgr;
AssetManager* Game::asset_manager{new AssetManager()};

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

   load_level(1);
}

void Game::initialize() {
   // call entity manager initialize
   entity_mgr.initialize();
}

Game::~Game()
{
   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
   SDL_Quit();
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
   entity_mgr.update(dt);
}

void Game::render()
{
   SDL_RenderClear(renderer);
   entity_mgr.render();
   SDL_RenderPresent(renderer);
}

void Game::load_level(const int number)
{

    //Open the script file, this one being config.lua
    lua.script_file("config.lua");

    //Add entries for each table property saved in config.lua
    sol::table entities = lua["entities"];
    sol::table assets = lua ["assets"];

    //First, get the assets from the asset table
    for (const auto& key_value_pair : assets) {
        sol::object k = key_value_pair.first;
        sol::object v = key_value_pair.second;
        sol::type t = k.get_type();

        switch(t) {
            case sol::type::string: {

                //Get the asset name, which is the key
                std::string assetID = k.as<std::string>();
                auto assetLoc = v.as<std::string>();
                //Cast from a string to a char, since asset_manager expects a filename of char
                const char* assetLocChar(assetLoc.c_str());

                //Load the asset using the asset_manager
                asset_manager->add_texture(assetID, assetLocChar);
                break;
            }
            default:
                std::cout << "Unkown type... no assets loaded." << std::endl;
                break;
        }
    }

    //Now load in the entities from the entities table in config.lua
    for (const auto& key_value_pair : entities) {
        sol::object k = key_value_pair.first;
        sol::object v = key_value_pair.second;

        sol::type t = v.get_type();

        switch(t) {
            case sol::type::table: {

                //First get table for the type of entity (such as a tank or chopper)
                sol::table nested = key_value_pair.second;
                //Use the first table, to now access the transform and sprite tables
                sol::table transform = nested["transform"];
                sol::table sprite = nested["sprite"];
                std::string entityName = k.as<std::string>();

                //Now get the different variables from the nested table for the transform
                //which is the positions, velocities, width, height, and scale
                float xpos = static_cast<float>(transform["position_x"]);
                float ypos = static_cast<float>(transform["position_y"]);
                float xvel = static_cast<float>(transform["velocity_x"]);
                float yvel = static_cast<float>(transform["velocity_y"]);
                float width = static_cast<float>(transform["width"]);
                float height = static_cast<float>(transform["height"]);
                float scale = static_cast<float>(transform["scale"]);

                //This grabs the sprite ID from the sprite table
                std::string spriteType = static_cast<std::string>(sprite["texture_id"]);

                //Now we can create the entities with the entity manager
                Entity& entity(entity_mgr.add_entity(entityName));
                entity.add_component<TransformComponent>(xpos,ypos,xvel,yvel,width,height,scale);
                entity.add_component<SpriteComponent>(spriteType);
                break;
            }
            default:
                std::cout << "Unkown type... no players loaded." << std::endl;
                break;
        }
    }

   entity_mgr.list_all_entities();
}
