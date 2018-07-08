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

#ifndef BULLET_H
#define BULLET_H

#include <math.h>
#include <SDL.h>
#include <SDL_image.h>

#include "graphic.h"

typedef enum
{
  minus_blue,
  minus_black,
  minus_red,
  minus_big_brown,
  
  ball_blue,
  ball_black,
  ball_red,
  
  blue_one,
  blue_zero,
  
  red_one,
  red_zero,
  
  plus_red,
  plus_big_brown,
  
  bullet_z,
  
  greycode,
  venn_elipse,
  
  dot_black,
  dot_blue,
  
  e_blue,
  
  BULLET_TYPE_MAX
}bullet_type;

typedef enum
{
  player_team,
  enemy_team
}bullet_team;

typedef struct enemy enemy;
typedef struct bullet bullet;
struct bullet
{
  bullet_type type;
  bullet_team team;
  
  /* needed to prevent collision between enemy and his own bullet */
  enemy *shot_by;
  
  double x, y;
  double dir_x, dir_y;
  
  int splitter;
  
  double speed;
  Uint32 last_move;
  
  int power;
  
  bullet *next;
  bullet *prev;
};

extern int init_bullet(void);

extern bullet *new_bullet(const bullet_team team, const bullet_type type, enemy *shot_by, const double x, const double y,
                          const double dir_x, const double dir_y, const int splitter, const double speed, const int power);
extern void free_bullet(bullet *object);
extern bullet *get_first_bullet(void);
extern int get_bullet_w(const bullet_type type);
extern int get_bullet_h(const bullet_type type);

extern void move_all_bullets(void);
extern void blit_all_bullets(SDL_Surface *dst);
extern void extend_all_bullets_timestamp(const Uint32 time);
extern void free_all_bullets(void);

extern void splitter_bullet(const bullet *blt, enemy *shot_by, const double x, const double y, const int w, const int h, const int amount);

#endif