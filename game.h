#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_ttf.h>

#include "framerate.h"
#include "bullet.h"
#include "collision.h"
#include "enemy.h"
#include "powerup.h"
#include "scrolling_tile.h"
#include "scrolling_text.h"
#include "spaceship.h"
#include "spaceship_enchantments.h"
#include "interface.h"
#include "ai.h"

/* runlevel */
typedef enum
{
  game_quit,
  game_paused,
  game_running,
  game_menu,
  game_over,
  game_restart,
  game_outro,
}game_mode;

extern int init_game(void);
extern void game_pause(void);
extern void game_run(void);
extern void game_run_menu(void);
extern void game_run_over(void);
extern void game_run_outro(void);

#endif