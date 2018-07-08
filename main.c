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