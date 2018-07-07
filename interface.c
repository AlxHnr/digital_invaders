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

#include "interface.h"

static SDL_Surface *health_bar = NULL;
static TTF_Font *interface_font = NULL;

static void interface_exit(void)
{
  SDL_FreeSurface(health_bar);
  TTF_CloseFont(interface_font);
}
int init_interface(void)
{
  if(!(health_bar = load_image("gfx/health_bar.png")))
    return 0;
  
  if(!(interface_font = TTF_OpenFont("gfx/LiberationSans-Regular.ttf", 20)))
    return 0;
  
  atexit(interface_exit);
  return 1;
}
void blit_interface(SDL_Surface *dst)
{
  spaceship *ship = get_spaceship();
  
  SDL_Rect tmp_src, tmp_dst;
  
  SDL_Surface *tmp_surface = NULL;
  SDL_Color text_color = {0, 0, 0, 0};
  char tmp_string[256] = {0};
  
  /* draw healthbar */
  tmp_src.x = 0;
  tmp_src.y = 0;
  tmp_src.w = 2;
  tmp_src.h = health_bar->h;
  
  tmp_dst.x = 0;
  tmp_dst.y = 0;
  SDL_BlitSurface(health_bar, &tmp_src, dst, &tmp_dst);
  
  for(tmp_src.x += tmp_src.w, tmp_dst.x += tmp_src.w; tmp_dst.x < ship->hp_max/10; tmp_dst.x += tmp_src.w)
    SDL_BlitSurface(health_bar, &tmp_src, dst, &tmp_dst);
  
  SDL_BlitSurface(health_bar, &tmp_src, dst, &tmp_dst);
  tmp_src.x += tmp_src.w;
  tmp_dst.x += tmp_src.w;
  SDL_BlitSurface(health_bar, &tmp_src, dst, &tmp_dst);
  
  if(ship->hp > ship->hp_max * 0.5)
    tmp_src.x = 7;
  else if(ship->hp > ship->hp_max * 0.25)
    tmp_src.x = 8;
  else
    tmp_src.x = 9;
  
  for(tmp_dst.x = 3; tmp_dst.x < ship->hp/10; tmp_dst.x += 3)
    SDL_BlitSurface(health_bar, &tmp_src, dst, &tmp_dst);
  
  /* draw points */
  sprintf(tmp_string, "%i", ship->points);
  tmp_surface = TTF_RenderText_Blended(interface_font, tmp_string, text_color);;
  tmp_dst.x = 0;
  tmp_dst.y += health_bar->h;
  SDL_BlitSurface(tmp_surface, NULL, dst, &tmp_dst);
  SDL_FreeSurface(tmp_surface);
}