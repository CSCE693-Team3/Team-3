

#include "Tank.hpp"
#include "../sdl_utils.hpp"
#include "../Game.hpp"

#include <iostream>

Tank::Tank(const float xpos, const float ypos,
           const float xvel, const float yvel)
: xpos(xpos), ypos(ypos), xvel(xvel), yvel(yvel)
{
   texture = sdl_utils::load_texture("../assets/images/tank-big-down.png");
}

Tank::~Tank()
{
   SDL_DestroyTexture(texture);
}

void Tank::update(const float dt)
{
   xpos += xvel * dt;
   ypos += yvel * dt;

   src_rect.h = 32;
   src_rect.w = 32;
   src_rect.x = 0;
   src_rect.y = 0;

   dest_rect.x = static_cast<int>(xpos);
   dest_rect.y = static_cast<int>(ypos);
   dest_rect.w = src_rect.w * 2;
   dest_rect.h = src_rect.h * 2;
}

void Tank::render()
{
   SDL_RenderCopy(Game::renderer, texture, &src_rect, &dest_rect);
}


