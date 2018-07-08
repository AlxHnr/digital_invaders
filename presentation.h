#ifndef PRESENTATION_H
#define PRESENTATION_H

#include <stdbool.h>
#include <stdint.h>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_rotozoom.h>

#include "gui_engine.h"
#include "gui_theme.h"
#include "framerate.h"
#include "graphic.h"
#include "bullet.h"
#include "enemy.h"
#include "interface.h"
#include "powerup.h"
#include "scrolling_text.h"
#include "scrolling_tile.h"
#include "spaceship.h"

#define PREV_BUTTON_POS 10, 570
#define PREV_BUTTON_TEXT "<"
#define NEXT_BUTTON_POS 765, 570
#define NEXT_BUTTON_TEXT ">"

typedef enum
{
  init,
  clean_up,
  run
}fun_call_type;

typedef enum
{
  prev,
  next,
}pres_button_type;

typedef void (*page_fun)(const fun_call_type);

extern int init_presentation(void);
extern void run_presentation(SDL_Surface *screen);

#endif