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

#include "scrolling_tile.h"

static SDL_Surface *tile = NULL;
static double pos_x, pos_y, speed_x, speed_y;
static Uint32 last_scroll;

static void scrolling_map_exit(void)
{
  SDL_FreeSurface(tile);
}
int init_scrolling_map(void)
{
  if(!(tile = load_image("gfx/tile.png")))
    return 0;
  
  reset_scrolling_map();
  
  atexit(scrolling_map_exit);
  return 1;
}
void reset_scrolling_map(void)
{
  pos_x = 0;
  pos_y = 0;
  speed_x = 0;
  speed_y = 0;
  
  last_scroll = SDL_GetTicks();
}

void set_scrolling_speed(const double x, const double y)
{
  speed_x = x;
  speed_y = y;
}
double get_scrolling_speed_x(void)
{
  return speed_x;
}
double get_scrolling_speed_y(void)
{
  return speed_y;
}

void increase_scrolling_speed(const double x, const double y)
{
  speed_x += x;
  speed_y += y;
}
void scroll_map(void)
{
  pos_x += speed_x * (SDL_GetTicks() - last_scroll);
  pos_y += speed_y * (SDL_GetTicks() - last_scroll);
  
  last_scroll = SDL_GetTicks();
}
void blit_scrolling_map(SDL_Surface *dst)
{
  SDL_Rect tile_dst, shift_dst;
  
  for(tile_dst.y = -tile->h; tile_dst.y <= dst->h; tile_dst.y += tile->h)
  {
    for(tile_dst.x = -tile->w; tile_dst.x <= dst->w + tile->w; tile_dst.x += tile->w)
    {
      shift_dst = tile_dst;
      shift_dst.x += (int)pos_x % tile->w;
      shift_dst.y += (int)pos_y % tile->h;
      
      SDL_BlitSurface(tile, NULL, dst, &shift_dst);
    }
  }
}
void extend_scrolling_map_timestamp(const Uint32 time)
{
  last_scroll += time;
}