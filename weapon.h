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

#ifndef WEAPON_H
#define WEAPON_H

#include <math.h>
#include <SDL.h>
#include <SDL_image.h>

#include "bullet.h"

#define LOW_GRADE_WEAPONS 4
#define MID_GRADE_WEAPONS 4
#define HIGH_GRADE_WEAPONS 4
#define SUPERIOR_GRADE_WEAPONS 5
#define ULTRA_GRADE_WEAPONS 5

typedef enum
{
  /* low grade weapons */
  needle_gun,
  dot_gun,
  blue_dot_gun,
  blue_laser,
  
  red_laser,
  
  BOSS_needle_blaster,
  BOSS_red_laser_thrower,
  
  /* mid grade weapons */
  needle_thrower,
  dot_thrower,
  blue_dot_shooter,
  blue_double_laser,
  
  red_double_laser,
  
  BOSS_dark_blaster,
  BOSS_red_plus_thrower,
  
  /* high grade weapons */
  blue_tripple_laser,
  blue_energy_gun,
  blue_needle_thrower,
  e_gun,
  
  red_plus_shooter,
  red_energy_gun,
  
  BOSS_plus_blaster,
  BOSS_dark_cannon,
  BOSS_minus_thrower,
  
  /* superior grade weapon */
  binary_gun,
  blue_quadro_laser,
  blue_energy_blaster,
  dark_energy_blaster,
  heavy_minus_blaster,
  
  red_binary_gun,
  red_plus_blaster,
  red_energy_blaster,
  blue_blaster,
  dark_blaster,
  
  BOSS_z_cannon,
  BOSS_energy_blaster,
  BOSS_plus_thrower,
  
  /* ultra weapons */
  ultimate_blue_laser,
  ultraheavy_minus_blaster,
  ultraheavy_plus_blaster,
  greycode_deathblaster,
  venn_shockwaver,
  
  WEAPON_TYPE_MAX
}weapon_type;

typedef struct
{
  bullet_type blt_type;
  int bullets;
  int splitter;
  
  int atk;
  double fire_rate;
  double scatter_angle;
  double bullet_speed;
}weapon;

extern int init_weapons(void);

extern void weapon_shot(const bullet_team team, const weapon_type type, enemy *shot_by, Uint32 *last_shot,
                        const double fire_rate, const double x, const double y, const int dir_x, const int atk);

#endif