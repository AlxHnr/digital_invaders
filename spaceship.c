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

#include "spaceship.h"

static SDL_Surface *spaceship_sprite = NULL;

static spaceship ship;

static void spaceship_exit(void)
{
  SDL_FreeSurface(spaceship_sprite);
  free_all_spaceship_enchants();
}
int init_spaceship(void)
{
  if(!(spaceship_sprite = load_image("gfx/spaceship.png")))
    return 0;
  
  reset_spaceship();
  
  atexit(spaceship_exit);
  return 1;
}
spaceship *get_spaceship(void)
{
  return &ship;
}
int get_spaceship_w(void)
{
  return spaceship_sprite->w;
}
int get_spaceship_h(void)
{
  return spaceship_sprite->h;
}

void reset_spaceship(void)
{
  free_all_spaceship_enchants();
  
  ship.x = spaceship_sprite->w;
  ship.y = SDL_GetVideoSurface()->h/2 - spaceship_sprite->h/2;
  
  ship.movement_x = 0;
  ship.movement_y = 0;
  
  ship.last_move = SDL_GetTicks();
  ship.last_shot = SDL_GetTicks();
  
  ship.hp = 250;
  ship.hp_max = 250;
  
  ship.points = 0;
  ship.speed = 0.2;
  ship.acceleration = 0.0007;
  
  ship.wpn_type = needle_gun + rand() % LOW_GRADE_WEAPONS;
  
  ship.fire_rate = 1.0;
  ship.atk = 20;
  ship.def = 15;
  
  ship.invert = 0;
  ship.weapon_block = 0;
  ship.move_block = 0;
}
void accelerate_spaceship(const int dir_x, const int dir_y)
{
  int x, y;
  
  if(ship.move_block)
  {
    x = 0;
    y = 0;
  }
  else if(ship.invert)
  {
    x = -1 * dir_x;
    y = -1 * dir_y;
  }
  else
  {
    x = dir_x;
    y = dir_y;
  }
  
  if(x != 0)
  {
    ship.movement_x += x * ship.acceleration * (SDL_GetTicks() - ship.last_move);
    
    /* limit movement to speed */
    if(ship.movement_x < 0 && ship.movement_x < -ship.speed)
      ship.movement_x = -ship.speed;
    else if(ship.movement_x > 0 && ship.movement_x > ship.speed)
      ship.movement_x = ship.speed;
  }
  else
  {
    if(ship.movement_x > 0)
      ship.movement_x -= SLOW_DOWN * (SDL_GetTicks() - ship.last_move);
    else if(ship.movement_x < 0)
      ship.movement_x += SLOW_DOWN * (SDL_GetTicks() - ship.last_move);
  }
  
  if(y != 0)
  {
    ship.movement_y += y * ship.acceleration * (SDL_GetTicks() - ship.last_move);
    
    /* limit movement to speed */
    if(ship.movement_y < 0 && ship.movement_y < -ship.speed)
      ship.movement_y = -ship.speed;
    else if(ship.movement_y > 0 && ship.movement_y > ship.speed)
      ship.movement_y = ship.speed;
  }
  else
  {
    if(ship.movement_y > 0)
      ship.movement_y -= SLOW_DOWN * (SDL_GetTicks() - ship.last_move);
    else if(ship.movement_y < 0)
      ship.movement_y += SLOW_DOWN * (SDL_GetTicks() - ship.last_move);
  }
}
void handle_spaceship(void)
{
  /* move ship */
  ship.x += ship.movement_x * (SDL_GetTicks() - ship.last_move);
  ship.y += ship.movement_y * (SDL_GetTicks() - ship.last_move);
  
  /* correct possition */
  if(ship.x < 0)
    ship.x = 0;
  else if(ship.x + spaceship_sprite->w > SDL_GetVideoSurface()->w)
    ship.x = SDL_GetVideoSurface()->w - spaceship_sprite->w;
  
  if(ship.y < 0)
    ship.y = 0;
  else if(ship.y + spaceship_sprite->h > SDL_GetVideoSurface()->h)
    ship.y = SDL_GetVideoSurface()->h - spaceship_sprite->h;
  
  /* correct values */
  if(ship.hp_max > SPACESHIP_HP_MAX)
    ship.hp_max = SPACESHIP_HP_MAX;
  
  if(ship.hp > ship.hp_max)
    ship.hp = ship.hp_max;
  
  if(ship.speed > SPACESHIP_SPEED_MAX)
    ship.speed = SPACESHIP_SPEED_MAX;
  else if(ship.speed < 0.0)
    ship.speed = 0.0;
  
  if(ship.acceleration > SPACESHIP_ACCELLERATION_MAX)
    ship.acceleration = SPACESHIP_ACCELLERATION_MAX;
  else if(ship.acceleration < 0.0)
    ship.acceleration = 0.0;
  
  if(ship.fire_rate < 0.0)
    ship.fire_rate = 0.0;
  if(ship.atk < 0.0)
    ship.atk = 0.0;
  if(ship.def < 0.0)
    ship.def = 0.0;
  
  ship.last_move = SDL_GetTicks();
}
void spaceship_shot(void)
{
  if(!ship.weapon_block)
    weapon_shot(player_team, ship.wpn_type, NULL, &ship.last_shot, ship.fire_rate,
                ship.x + spaceship_sprite->w, ship.y + spaceship_sprite->h/2, 1, ship.atk);
}
void blit_spaceship(SDL_Surface *dst)
{
  SDL_Rect tmp_dst = {ship.x, ship.y, 0, 0};
  
  SDL_BlitSurface(spaceship_sprite, NULL, dst, &tmp_dst);
}
void extend_spaceship_timestamp(const Uint32 time)
{
  ship.last_move += time;
  ship.last_shot += time;
}