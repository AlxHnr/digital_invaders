#include "graphic.h"

SDL_Surface *load_image(const char *image_path)
{
  SDL_Surface *tmp_surface = IMG_Load(image_path);
  SDL_Surface *cache_surface = SDL_DisplayFormatAlpha(tmp_surface);
  SDL_FreeSurface(tmp_surface);
  
  return cache_surface;
}