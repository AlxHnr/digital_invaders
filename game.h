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