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

#include "scrolling_text.h"

static scrolling_text *first_text = NULL;
static scrolling_text *last_text = NULL;
static TTF_Font *scrolling_font[s_text_frames];

static void scrolling_text_exit(void)
{
  int counter;
  for(counter = 0; counter < s_text_frames; counter++)
    TTF_CloseFont(scrolling_font[counter]);
  
  free_all_scrolling_texts();
}
int init_scrolling_text()
{
  int counter = 0;
  while(counter < s_text_frames)
  {
    if(!(scrolling_font[counter] = TTF_OpenFont("gfx/arial.ttf", counter + 15)))
      return 0;
    
    counter++;
  }
  
  atexit(scrolling_text_exit);
  
  return 1;
}

scrolling_text *new_scrolling_text(const double x, const double y, const char *text,
                                   const Uint8 r, const Uint8 g, const Uint8 b)
{
  /* allocate memory and set new text to last text */
  if(!first_text)
  {
    first_text = (scrolling_text *)malloc(sizeof(scrolling_text));
    if(!first_text)
      return NULL;
    
    first_text->next = NULL;
    first_text->prev = NULL;
    last_text = first_text;
  }
  else
  {
    last_text->next = (scrolling_text *)malloc(sizeof(scrolling_text));
    if(!last_text->next)
      return NULL;
    
    last_text->next->prev = last_text;
    last_text->next->next = NULL;
    last_text = last_text->next;
  }
  
  /* set text properties */
  strncpy(last_text->text, text, TEXT_BUFFER);
  last_text->color.r = r;
  last_text->color.g = g;
  last_text->color.b = b;
  
  last_text->x = x;
  last_text->y = y;
  
  last_text->start_time = SDL_GetTicks();
  
  return last_text;
}
scrolling_text *new_scrolling_int(const double x, const double y, const int value,
                                  const Uint8 r, const Uint8 g, const Uint8 b)
{
  char tmp_string[TEXT_BUFFER] = {0};
  
  sprintf(tmp_string, "%i", value);
  
  return new_scrolling_text(x, y, tmp_string, r, g, b);
}
void free_scrolling_text(scrolling_text *scr_text)
{
  if(!scr_text)
    return;
  
  if(scr_text == first_text)
  {
    first_text = scr_text->next;
    
    if(scr_text->next)
      scr_text->next->prev = NULL;
    else
      last_text = NULL;
  }
  else if(scr_text == last_text)
  {
    scr_text->prev->next = NULL;
    last_text = scr_text->prev;
  }
  else
  {
    scr_text->next->prev = scr_text->prev;
    scr_text->prev->next = scr_text->next;
  }
  
  free(scr_text);
}

void blit_all_scrolling_texts(SDL_Surface *dst)
{
  SDL_Rect tmp_dst;
  
  SDL_Surface *cache_surface = NULL;
  
  scrolling_text *tmp_text = first_text;
  scrolling_text *cache_text = NULL;
  
  while(tmp_text)
  {
    /* remove timed out text */
    if(SDL_GetTicks() - tmp_text->start_time > s_text_duration)
    {
      cache_text = tmp_text;
      tmp_text = tmp_text->next;
      free_scrolling_text(cache_text);
      continue;
    }
    
    /* render damage text */
    cache_surface = TTF_RenderText_Blended(scrolling_font[(int)((SDL_GetTicks() - tmp_text->start_time)/(s_text_duration/s_text_frames + 1))],
                                          tmp_text->text, tmp_text->color);
    
    /* blit current damage_list element */
    tmp_dst.x = tmp_text->x - cache_surface->w/2;
    tmp_dst.y = tmp_text->y - cache_surface->h/2 - (int)((SDL_GetTicks() - tmp_text->start_time)/(s_text_duration/s_text_frames + 1)) * 1.5;
    SDL_BlitSurface(cache_surface, NULL, dst, &tmp_dst);
    
    SDL_FreeSurface(cache_surface);
    tmp_text = tmp_text->next;
  }
}
void extend_all_scrolling_texts_timestamp(const Uint32 time)
{
  scrolling_text *tmp_text = NULL;
  
  for(tmp_text = first_text; tmp_text; tmp_text = tmp_text->next)
    tmp_text->start_time += time;
}
void free_all_scrolling_texts(void)
{
  while(first_text)
    free_scrolling_text(first_text);
}