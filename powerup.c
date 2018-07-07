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

#include "powerup.h"

static powerup *first_powerup = NULL;
static powerup *last_powerup = NULL;

static SDL_Surface *powerup_sprite[POWERUP_ICON_MAX];

static void powerup_exit(void)
{
  int counter;
  
  for(counter = 0; counter < POWERUP_ICON_MAX; counter++)
    SDL_FreeSurface(powerup_sprite[counter]);
  
  free_all_powerups();
}
int init_powerups(void)
{
  int counter;
  char tmp_string[512] = {0};
  
  for(counter = 0; counter < POWERUP_ICON_MAX; counter++)
  {
    sprintf(tmp_string, "gfx/powerup/%i.png", counter);
    if(!(powerup_sprite[counter] = load_image(tmp_string)))
      return 0;
  }
  
  atexit(powerup_exit);
  return 1;
}


powerup *new_powerup(const powerup_type type, const powerup_icon icon,
                     const char *text, const double x, const double y,
                     const double dir_x, const double dir_y,
                     const double moving_speed)
{
  /* allocate memory and set new powerup to last powerup */
  if(!first_powerup)
  {
    first_powerup = (powerup *)malloc(sizeof(powerup));
    if(!first_powerup)
      return NULL;
    
    first_powerup->next = NULL;
    first_powerup->prev = NULL;
    last_powerup = first_powerup;
  }
  else
  {
    last_powerup->next = (powerup *)malloc(sizeof(powerup));
    if(!last_powerup->next)
      return NULL;
    
    last_powerup->next->prev = last_powerup;
    last_powerup->next->next = NULL;
    last_powerup = last_powerup->next;
  }
  
  /* set powerup properties */
  last_powerup->type = type;
  last_powerup->icon = icon;
  
  if(text)
    strncpy(last_powerup->text, text, P_BUFFER_LENGTH);
  else
    last_powerup->text[0] = '\0';
  
  last_powerup->x = x;
  last_powerup->y = y;
  last_powerup->dir_x = dir_x;
  last_powerup->dir_y = dir_y;
  last_powerup->moving_speed = moving_speed;
  last_powerup->last_move = SDL_GetTicks();
  
  last_powerup->speed = 0;
  last_powerup->acceleration = 0;
  
  last_powerup->wpn_type = 0;
  last_powerup->hp = 0;
  last_powerup->hp_max = 0;
  
  last_powerup->atk = 0;
  last_powerup->def = 0;
  last_powerup->fire_rate = 0;
  
  last_powerup->invert = 0;
  last_powerup->weapon_block = 0;
  last_powerup->move_block = 0;
  
  last_powerup->start_time = SDL_GetTicks();
  last_powerup->duration = 0;
  
  return last_powerup;
}
void free_powerup(powerup *object)
{
  if(!object)
    return;
  
  if(object == first_powerup)
  {
    first_powerup = object->next;
    
    if(object->next)
      object->next->prev = NULL;
    else
      last_powerup = NULL;
  }
  else if(object == last_powerup)
  {
    object->prev->next = NULL;
    last_powerup = object->prev;
  }
  else
  {
    object->next->prev = object->prev;
    object->prev->next = object->next;
  }
  
  free(object);
}
powerup *get_first_powerup(void)
{
  return first_powerup;
}

int get_powerup_w(const powerup_icon icon)
{
  return powerup_sprite[icon]->w;
}
int get_powerup_h(const powerup_icon icon)
{
  return powerup_sprite[icon]->h;
}

void move_all_powerups(void)
{
  powerup *tmp_powerup = first_powerup;
  powerup *cache_powerup = NULL;
  while(tmp_powerup)
  {
    tmp_powerup->x += tmp_powerup->dir_x * (SDL_GetTicks() - tmp_powerup->last_move) * tmp_powerup->moving_speed;
    tmp_powerup->y += tmp_powerup->dir_y * (SDL_GetTicks() - tmp_powerup->last_move) * tmp_powerup->moving_speed;
    
    tmp_powerup->last_move = SDL_GetTicks();
    
    if((tmp_powerup->x > SDL_GetVideoSurface()->w) ||
       (tmp_powerup->x < 0 - get_powerup_w(tmp_powerup->icon)) ||
       (tmp_powerup->y > SDL_GetVideoSurface()->h) ||
       (tmp_powerup->y < 0 -get_powerup_h(tmp_powerup->icon)))
    {
      cache_powerup = tmp_powerup;
      tmp_powerup = tmp_powerup->next;
      free_powerup(cache_powerup);
      continue;
    }
    
    tmp_powerup = tmp_powerup->next;
  }
}
void blit_all_powerups(SDL_Surface *dst)
{
  powerup *tmp_powerup = NULL;
  SDL_Rect tmp_rect;
  
  for(tmp_powerup = first_powerup; tmp_powerup; tmp_powerup = tmp_powerup->next)
  {
    tmp_rect.x = tmp_powerup->x;
    tmp_rect.y = tmp_powerup->y;
    
    SDL_BlitSurface(powerup_sprite[tmp_powerup->icon], NULL, dst, &tmp_rect);
  }
}
void extend_all_powerups_timestamp(const Uint32 time)
{
  powerup *tmp_powerup = NULL;
  
  for(tmp_powerup = first_powerup; tmp_powerup; tmp_powerup = tmp_powerup->next)
  {
    tmp_powerup->start_time += time;
    tmp_powerup->last_move += time;
  }
}
void free_all_powerups(void)
{
  while(first_powerup)
    free_powerup(first_powerup);
}