#include "framerate.h"

void controll_framerate()
{
  static Uint32 time_stop = 0;
  
  if(SDL_GetTicks() - time_stop < (int)(1000/FRAMERATE_LIMIT))
    SDL_Delay((int)(1000/FRAMERATE_LIMIT) - (SDL_GetTicks() - time_stop));
  
  time_stop = SDL_GetTicks();
}