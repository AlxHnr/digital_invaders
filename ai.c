/*
  Copyright (c) 2012 Alexander Heinrich
  
  This software is provided 'as-is', without any express or implied warranty.
  In no event will the authors be held liable for any damages arising from
  the use of this software.
  
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it freely,
  subject to the following restrictions:
  
  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  
  2. Altered source versions must be plainly marked as such, and must not
     be misrepresented as being the original software.
  
  3. This notice may not be removed or altered from any source distribution.

*/

#include "ai.h"

void AI_move_to_ship_and_shot(enemy *this)
{
  if(SDL_GetTicks() - this->ai_timestop > 1250)
  {
    enemy_set_dir_to_ship(this);
    this->ai_timestop = SDL_GetTicks();
  }
  
  enemy_move(this);
  enemy_shot(this);
}
void AI_move_fast_to_ship_and_shot(enemy *this)
{
  this->dir_x = -4;
  enemy_set_dir_to_ship(this);
  this->dir_x = -1;
  
  enemy_move(this);
  enemy_shot(this);
}
void AI_move_to_ship_and_dodge(enemy *this)
{
  if(SDL_GetTicks() - this->ai_timestop > 1250)
  {
    enemy_set_dir_to_ship(this);
    this->ai_timestop = SDL_GetTicks();
  }
  
  enemy_dodge(this);
  enemy_move(this);
  enemy_shot(this);
}
void AI_move_into_screen_and_shot(enemy *this)
{
  if(this->x > SDL_GetVideoSurface()->w - get_enemy_w(this->level) - 10)
  {
    enemy_set_dir_to_ship(this);
  }
  else
  {
    this->dir_x = -4;
    enemy_set_dir_to_ship(this);
    
    this->dir_x = 0;
  }
  
  enemy_move(this);
  enemy_shot(this);
}
void AI_move_into_screen_and_dodge(enemy *this)
{
  if(this->x > SDL_GetVideoSurface()->w - get_enemy_w(this->level) - 10)
  {
    enemy_set_dir_to_ship(this);
  }
  else
  {
    if(SDL_GetTicks() - this->ai_timestop > 750)
    {
      this->dir_x = -1;
      enemy_set_dir_to_ship(this);
      this->ai_timestop = SDL_GetTicks();
    }
    
    this->dir_x = 0;
    enemy_dodge(this);
  }
  
  enemy_move(this);
  enemy_shot(this);
}
void AI_move_into_screen_and_dodge_quick(enemy *this)
{
  if(this->x > SDL_GetVideoSurface()->w - get_enemy_w(this->level) - 10)
  {
    enemy_set_dir_to_ship(this);
  }
  else
  {
    /* point 'this' to ship */
    if(SDL_GetTicks() - this->ai_timestop > 750)
    {
      this->dir_x = -1;
      enemy_set_dir_to_ship(this);
      this->ai_timestop = SDL_GetTicks();
    }
    
    /* dodge bullets with 4 x speed */
    this->dir_x = -4.0;
    enemy_dodge(this);
    
    /* set dir_x to 0 */
    this->dir_x = 0;
  }
  
  enemy_move(this);
  enemy_shot(this);
}