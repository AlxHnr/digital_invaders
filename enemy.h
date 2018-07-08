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

#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <SDL_image.h>

#include "graphic.h"
#include "spaceship.h"
#include "weapon.h"
#include "bullet.h"
#include "ai.h"

#define ENEMY_LEVEL_MAX 19

/* typedef enemy is in "bullet.h" */
struct enemy
{
  int level;
  weapon_type wpn_type;
  
  double x, y;
  double dir_x, dir_y;
  double speed;
  Uint32 last_move;
  Uint32 last_shot;
  int next_dodge;
  
  unsigned int points;
  int hp, hp_max;
  int atk, def;
  double fire_rate;
  
  void (*ai_function)(enemy *);
  Uint32 ai_timestop;
  
  enemy *next;
  enemy *prev;
};

extern int init_enemys(void);

extern enemy *new_enemy(const int level, const unsigned int points, void (*ai_function)(enemy *),
                        const weapon_type wpn_type, const double x, const double y,
                        const double speed, const int hp_max, const int atk, const int def, const double fire_rate);
extern void free_enemy(enemy *object);
extern enemy *get_first_enemy(void);
extern int get_enemy_w(const int level);
extern int get_enemy_h(const int level);

extern void handle_all_enemys(void);
extern void blit_all_enemys(SDL_Surface *dst);
extern void extend_all_enemys_timestamp(const Uint32 time);
extern void free_all_enemys(void);

/* base functions */
extern void enemy_set_dir_to_ship(enemy *this);
extern void enemy_dodge(enemy *this);
extern void enemy_move(enemy *this);
extern void enemy_shot(enemy *this);

extern enemy *spawn_enemy(const int lvl);

#endif