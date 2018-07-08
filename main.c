#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "presentation.h"

#ifdef __WIN32__
  int WinMain(void)
#else
  int main(void)
#endif
{
  /* init SDL start */
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    exit(EXIT_FAILURE);
  atexit(SDL_Quit);
  
  /* window icon */
  SDL_Surface *window_icon = SDL_LoadBMP("gfx/window_icon.bmp");
  SDL_WM_SetIcon(window_icon, NULL);
  
  SDL_WM_SetCaption("Digital Invaders Presentation", NULL);
  SDL_Surface *screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);
  if(!screen)
    exit(EXIT_FAILURE);
  
  if(TTF_Init() < 0)
    exit(EXIT_FAILURE);
  atexit(TTF_Quit);
  /* init SDL end */
  
  if(!init_presentation())
    exit(EXIT_FAILURE);
  
  run_presentation(screen);
  
  SDL_FreeSurface(window_icon);
  return EXIT_SUCCESS;
}