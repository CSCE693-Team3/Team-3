
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

    sol::table entities = lua["entities"];
    sol::table assets = lua ["assets"];
    //Iterate over each key (the entities)
    for (const auto& key_value_pair : assets) {
        sol::object k = key_value_pair.first;
        sol::object v = key_value_pair.second;

        sol::type t = k.get_type();

        switch(t) {
            case sol::type::string: {

                //Get the player's name from the key value that the iterator is on
                std::string assetID = k.as<std::string>();
                auto assetLoc = v.as<std::string>();
                const char* assetLocChar(assetLoc.c_str());
                std::cout << "assetLoc: " << assetLoc << std::endl;
                asset_manager->add_texture(assetID, assetLocChar);

                //DEBUG STATEMENTS REMOVE
                std::cout << "assetID: " << assetID << std::endl;
                std::cout << "assetLoc: " << assetLoc << std::endl;

                break;
            }
            default:
                std::cout << "Unkown type... no players loaded." << std::endl;
                break;
        }


    }
    for (const auto& key_value_pair : entities) {
        sol::object k = key_value_pair.first;
        sol::object v = key_value_pair.second;

        sol::type t = v.get_type();

        switch(t) {
            case sol::type::table: {
                //First get the value for each player, these are tables (nested in the table of "gameobjs")
                sol::table nested = key_value_pair.second;
                sol::table transform = nested["transform"];
                sol::table sprite = nested["sprite"];
                //Get the player's name from the key value that the iterator is on

                std::string entityName = k.as<std::string>();


                //Now get the different variables from the nested table for each player,
                //which is the kind (chopper, tank, etc), positions, and velocities
                //std::string entity = static_cast<std::string>(nested["transform"]);
                float xpos = static_cast<float>(transform["position_x"]);
                float ypos = static_cast<float>(transform["position_y"]);
                float xvel = static_cast<float>(transform["velocity_x"]);
                float yvel = static_cast<float>(transform["velocity_y"]);
                float width = static_cast<float>(transform["width"]);
                float height = static_cast<float>(transform["height"]);
                float scale = static_cast<float>(transform["scale"]);

                std::string spriteType = static_cast<std::string>(sprite["texture_id"]);

                //DEBUG STATEMENTS REMOVE
                std::cout << "xpos: " << xpos << std::endl;
                std::cout << "ypos: " << ypos << std::endl;
                std::cout << "xvel: " << xvel << std::endl;
                std::cout << "yvel: " << yvel << std::endl;
                std::cout << "width: " << width << std::endl;
                std::cout << "height: " << height << std::endl;
                std::cout << "scale: " << scale << std::endl;
                std::cout << "spriteID " << spriteType << std::endl;
                //Add the player config that was read in, to the playerConfig unordered_map
                //playerConfig.emplace(playerName, std::make_tuple(kind, xpos, ypos, xvel, yvel));

                
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
   // add assets to asset manager

   //asset_manager->add_texture("tank-image", "../assets/images/tank-big-right.png");
   //asset_manager->add_texture("chopper-image", "../assets/images/chopper-spritesheet.png");

   // create entities and add components to them
   //Entity& tank_entity(entity_mgr.add_entity("tank"));
   //tank_entity.add_component<TransformComponent>(0,0,20,20,32,32,1);
   //tank_entity.add_component<SpriteComponent>("tank-image");

   //Entity& chopper_entity(entity_mgr.add_entity("chopper"));
   //chopper_entity.add_component<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
   //chopper_entity.add_component<SpriteComponent>("chopper-image");

   entity_mgr.list_all_entities();
}
