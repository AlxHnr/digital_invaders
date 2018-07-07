#ifndef INTERFACE_H
#define INTERFACE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "graphic.h"
#include "spaceship.h"

extern int init_interface(void);
extern void blit_interface(SDL_Surface *dst);

#endif