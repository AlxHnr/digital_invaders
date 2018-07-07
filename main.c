#include <stdlib.h>
#include <SDL.h>

#include "game.h"

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
  
  SDL_WM_SetCaption("Digital Invaders", NULL);
  SDL_Surface *screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE);
  if(!screen)
    exit(EXIT_FAILURE);
  
  if(TTF_Init() < 0)
    exit(EXIT_FAILURE);
  atexit(TTF_Quit);
  /* init SDL end */
  
  if(!init_game())
    exit(EXIT_FAILURE);
  
  /* main loop start */
  extern game_mode GAME_MODE;
  while(GAME_MODE != game_quit)
  {
    if(GAME_MODE == game_paused)
      game_pause();
    else if(GAME_MODE == game_running)
      game_run();
    else if(GAME_MODE == game_menu)
      game_run_menu();
    else if(GAME_MODE == game_over)
      game_run_over();
    else if(GAME_MODE == game_outro)
      game_run_outro();
    
    if(GAME_MODE == game_restart)
      GAME_MODE = game_running;
  }
  /* main loop end */
  
  SDL_FreeSurface(window_icon);
  return EXIT_SUCCESS;
}