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

#include "bullet.h"

static SDL_Surface *bullet_sprite[BULLET_TYPE_MAX];

static bullet *first_bullet = NULL;
static bullet *last_bullet = NULL;

static void bullet_exit(void)
{
  int counter;
  for(counter = 0; counter < BULLET_TYPE_MAX; counter++)
    SDL_FreeSurface(bullet_sprite[counter]);
  
  free_all_bullets();
}
int init_bullet(void)
{
  int counter;
  char tmp_string[512] = {0};
  for(counter = 0; counter < BULLET_TYPE_MAX; counter++)
  {
    sprintf(tmp_string, "gfx/bullet/%i.png", counter);
    if(!(bullet_sprite[counter] = load_image(tmp_string)))
      return 0;
  }
  
  atexit(bullet_exit);
  return 1;
}

bullet *new_bullet(const bullet_team team, const bullet_type type, enemy *shot_by, const double x, const double y,
                   const double dir_x, const double dir_y, const int splitter, const double speed, const int power)
{
  /* allocate memory and set new bullet to last bullet */
  if(!first_bullet)
  {
    first_bullet = (bullet *)malloc(sizeof(bullet));
    if(!first_bullet)
      return NULL;
    
    first_bullet->next = NULL;
    first_bullet->prev = NULL;
    last_bullet = first_bullet;
  }
  else
  {
    last_bullet->next = (bullet *)malloc(sizeof(bullet));
    if(!last_bullet->next)
      return NULL;
    
    last_bullet->next->prev = last_bullet;
    last_bullet->next->next = NULL;
    last_bullet = last_bullet->next;
  }
  
  /* set bullet properties */
  last_bullet->type = type;
  last_bullet->team = team;
  last_bullet->shot_by = shot_by;
  
  last_bullet->x = x;
  last_bullet->y = y;
  last_bullet->dir_x = dir_x;
  last_bullet->dir_y = dir_y;
  
  last_bullet->splitter = splitter;
  
  last_bullet->speed = speed;
  last_bullet->power = power;
  
  last_bullet->last_move = SDL_GetTicks();
  
  return last_bullet;
}
void free_bullet(bullet *object)
{
  if(!object)
    return;
  
  if(object == first_bullet)
  {
    first_bullet = object->next;
    
    if(object->next)
      object->next->prev = NULL;
    else
      last_bullet = NULL;
  }
  else if(object == last_bullet)
  {
    object->prev->next = NULL;
    last_bullet = object->prev;
  }
  else
  {
    object->next->prev = object->prev;
    object->prev->next = object->next;
  }
  
  free(object);
}
bullet *get_first_bullet(void)
{
  return first_bullet;
}
int get_bullet_w(const bullet_type type)
{
  return bullet_sprite[type]->w;
}
int get_bullet_h(const bullet_type type)
{
  return bullet_sprite[type]->h;
}

void move_all_bullets(void)
{
  bullet *tmp_bullet = first_bullet;
  bullet *cache_bullet = NULL;
  
  while(tmp_bullet)
  {
    tmp_bullet->x += tmp_bullet->dir_x * (SDL_GetTicks() - tmp_bullet->last_move) * tmp_bullet->speed;
    tmp_bullet->y += tmp_bullet->dir_y * (SDL_GetTicks() - tmp_bullet->last_move) * tmp_bullet->speed;
    
    tmp_bullet->last_move = SDL_GetTicks();
    
    if((tmp_bullet->x > SDL_GetVideoSurface()->w) ||
       (tmp_bullet->x < 0 - get_bullet_w(tmp_bullet->type)) ||
       (tmp_bullet->y > SDL_GetVideoSurface()->h) ||
       (tmp_bullet->y < 0 -get_bullet_h(tmp_bullet->type)))
    {
      cache_bullet = tmp_bullet;
      tmp_bullet = tmp_bullet->next;
      free_bullet(cache_bullet);
      continue;
    }
    
    tmp_bullet = tmp_bullet->next;
  }
}
void blit_all_bullets(SDL_Surface *dst)
{
  bullet *tmp_bullet = NULL;
  SDL_Rect tmp_rect;
  
  for(tmp_bullet = first_bullet; tmp_bullet; tmp_bullet = tmp_bullet->next)
  {
    tmp_rect.x = tmp_bullet->x;
    tmp_rect.y = tmp_bullet->y;
    
    SDL_BlitSurface(bullet_sprite[tmp_bullet->type], NULL, dst, &tmp_rect);
  }
}
void extend_all_bullets_timestamp(const Uint32 time)
{
  bullet *tmp_bullet = NULL;
  
  for(tmp_bullet = first_bullet; tmp_bullet; tmp_bullet = tmp_bullet->next)
    tmp_bullet->last_move += time;
}
void free_all_bullets(void)
{
  while(first_bullet)
    free_bullet(first_bullet);
}

void splitter_bullet(const bullet *blt, enemy *shot_by, const double x, const double y, const int w, const int h, const int amount)
{
  if((!blt) || (amount <= 0))
    return;
  
  double angle;
  int counter;
  
  double dir_x, dir_y, dst_x, dst_y;
  
  for(counter = 0, angle = 0.0; counter < amount; counter++, angle += 360.0/(double)amount)
  {
    dir_x = sin(angle * M_PI/180.0);
    dir_y = cos(angle * M_PI/180.0);
    
    if(dir_x >= 0.0 && dir_y <= 0.0)
    {
      dst_x = x + w/2 + w/2 * dir_x + 1;
      dst_y = y + h/2 + h/2 * dir_y - 1;
    }
    else if(dir_x >= 0.0 && dir_y >= 0.0)
    {
      dst_x = x + w/2 + w/2 * dir_x + 1;
      dst_y = y + h/2 + h/2 * dir_y + 1;
    }
    else if(dir_x <= 0.0 && dir_y >= 0.0)
    {
      dst_x = x + w/2 + w/2 * dir_x - get_bullet_w(blt->type) - 1;
      dst_y = y + h/2 + h/2 * dir_y + 1;
    }
    else
    {
      dst_x = x + w/2 + w/2 * dir_x - get_bullet_w(blt->type) - 1;
      dst_y = y + h/2 + h/2 * dir_y - 1;
    }
    
    new_bullet(blt->team, blt->type, shot_by, dst_x, dst_y, dir_x, dir_y, blt->splitter, blt->speed, blt->power);
  }
}