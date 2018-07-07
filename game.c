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

#include "game.h"

game_mode GAME_MODE = game_menu;

static TTF_Font *game_font = NULL;
static TTF_Font *game_font_win = NULL;

/* stage specific variables */
static void (*current_stage)(const bool) = NULL;
static Uint32 next_enemy_spawn[ENEMY_LEVEL_MAX] = {0};
static Uint32 next_powerup_spawn[POWERUP_ICON_MAX] = {0};
static enemy *current_boss = NULL;

/* the points collectet in current stage */
#define __STAGE_POINTS__ (ship->points - stage_points)

/* the amount of points the ship have when entering the stage */
static unsigned int stage_points = 0;
static spaceship *ship = NULL;

static void reset_stage(void)
{
  int counter;
  
  for(counter = 0; counter < ENEMY_LEVEL_MAX; counter++)
    next_enemy_spawn[counter] = SDL_GetTicks();
  
  for(counter = 0; counter < POWERUP_ICON_MAX; counter++)
    next_powerup_spawn[counter] = SDL_GetTicks();
  
  stage_points = ship->points;
}

/* some macros needed by stage */
#define __ENEMY__(level, time) \
        if(SDL_GetTicks() > next_enemy_spawn[level]){ \
          spawn_enemy(level); next_enemy_spawn[level] = SDL_GetTicks() + rand() % time; }
#define __WEAPON__(icon, time) \
        if(SDL_GetTicks() > next_powerup_spawn[icon]){ \
          new_powerup_weapon(icon); next_powerup_spawn[icon] = SDL_GetTicks() + rand() % time; }
#define __STATUS__(icon, time) \
        if(SDL_GetTicks() > next_powerup_spawn[icon]){ \
          new_powerup_status_increase(icon); next_powerup_spawn[icon] = SDL_GetTicks() + rand() % time; }
#define __ENCHANT__(icon, time) \
        if(SDL_GetTicks() > next_powerup_spawn[icon]){ \
          new_powerup_enchantment(icon); next_powerup_spawn[icon] = SDL_GetTicks() + rand() % time; }

/* stages start */
static void stage_final_boss(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    free_all_enemys();
    
    reset_stage();
    current_stage = stage_final_boss;
    current_boss = spawn_enemy(18);
    set_scrolling_speed(0.0, 0.0);
    
    new_powerup_status_increase(icon_ansi);
    
    next_enemy_spawn[16] = SDL_GetTicks() + rand() % 60000;
    
    next_powerup_spawn[icon_weapon_grade_0] = SDL_GetTicks() + rand() % 30000;
    next_powerup_spawn[icon_weapon_grade_4] = SDL_GetTicks() + rand() % 40000;
    
    next_powerup_spawn[icon_carry] = SDL_GetTicks() + rand() % 30000;
    next_powerup_spawn[icon_borrow] = SDL_GetTicks() + rand() % 30000;
    next_powerup_spawn[icon_complement] = SDL_GetTicks() + rand() % 75000;
    next_powerup_spawn[icon_function] = SDL_GetTicks() + rand() % 75000;
    next_powerup_spawn[icon_or] = SDL_GetTicks() + rand() % 75000;
    next_powerup_spawn[icon_ansi] = SDL_GetTicks() + rand() % 75000;
    next_powerup_spawn[icon_inhibition] = SDL_GetTicks() + rand() % 100000;
    
    next_powerup_spawn[icon_add] = SDL_GetTicks() + rand() % 14000;
    next_powerup_spawn[icon_problem] = SDL_GetTicks() + rand() % 14000;
    next_powerup_spawn[icon_idempotency] = SDL_GetTicks() + rand() % 14000;
    next_powerup_spawn[icon_inverted] = SDL_GetTicks() + rand() % 14000;
    
    return;
  }
  else if(__STAGE_POINTS__ > 800000)
  {
    GAME_MODE = game_outro;
    
    return;
  }
  /* init stage header end */
  
  /* stage content */
  __ENEMY__(16, 60000)
  
  __WEAPON__(icon_weapon_grade_0, 30000)
  __WEAPON__(icon_weapon_grade_4, 40000)
  
  __STATUS__(icon_carry, 30000)
  __STATUS__(icon_borrow, 30000)
  __STATUS__(icon_complement, 75000)
  __STATUS__(icon_function, 75000)
  __STATUS__(icon_or, 75000)
  __STATUS__(icon_ansi, 75000)
  
  __ENCHANT__(icon_inhibition, 100000)
  
  __ENCHANT__(icon_add, 14000)
  __ENCHANT__(icon_problem, 14000)
  __ENCHANT__(icon_idempotency, 14000)
  __ENCHANT__(icon_inverted, 14000)
  
  if((ship->move_block) || (ship->weapon_block))
  {
    current_boss->speed = 0.3;
    current_boss->wpn_type = BOSS_z_cannon;
  }
  else if(ship->y + get_spaceship_h() > current_boss->y &&
          ship->y < current_boss->y + get_enemy_h(current_boss->level))
  {
    current_boss->speed = 0.08;
    current_boss->wpn_type = BOSS_energy_blaster;
  }
  else
  {
    current_boss->speed = 0.08;
    current_boss->wpn_type = BOSS_plus_thrower;
  }
}
static void stage_14(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    reset_stage();
    current_stage = stage_14;
    
    new_powerup_status_increase(icon_complement);
    
    next_enemy_spawn[14] = SDL_GetTicks() + rand() % 18000;
    next_enemy_spawn[16] = SDL_GetTicks() + rand() % 35000;
    spawn_enemy(17);
    
    next_powerup_spawn[icon_weapon_grade_4] = SDL_GetTicks() + rand() % 40000;
    
    next_powerup_spawn[icon_carry] = SDL_GetTicks() + rand() % 40000;
    next_powerup_spawn[icon_borrow] = SDL_GetTicks() + rand() % 60000;
    next_powerup_spawn[icon_ansi] = SDL_GetTicks() + rand() % 130000;
    
    next_powerup_spawn[icon_add] = SDL_GetTicks() + rand() % 14000;
    next_powerup_spawn[icon_problem] = SDL_GetTicks() + rand() % 19000;
    next_powerup_spawn[icon_inverted] = SDL_GetTicks() + rand() % 19000;
    
    return;
  }
  else if(__STAGE_POINTS__ > 60000)
  {
    stage_final_boss(true);
    
    return;
  }
  /* init stage header end */
  
  /* stage content */
  __ENEMY__(14, 18000)
  __ENEMY__(16, 35000)
  
  __WEAPON__(icon_weapon_grade_4, 40000)
  
  __STATUS__(icon_carry, 40000)
  __STATUS__(icon_borrow, 60000)
  __STATUS__(icon_ansi, 130000)
  
  __ENCHANT__(icon_add, 14000)
  __ENCHANT__(icon_problem, 19000)
  __ENCHANT__(icon_inverted, 19000)
}
static void stage_13(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    reset_stage();
    current_stage = stage_13;
    
    new_powerup_status_increase(icon_ansi);
    
    next_enemy_spawn[14] = SDL_GetTicks() + rand() % 22000;
    next_enemy_spawn[15] = SDL_GetTicks() + rand() % 29000;
    next_enemy_spawn[16] = SDL_GetTicks() + rand() % 32000;
    
    next_powerup_spawn[icon_weapon_grade_4] = SDL_GetTicks() + rand() % 30000;
    
    next_powerup_spawn[icon_carry] = SDL_GetTicks() + rand() % 30000;
    next_powerup_spawn[icon_borrow] = SDL_GetTicks() + rand() % 50000;
    next_powerup_spawn[icon_ansi] = SDL_GetTicks() + rand() % 110000;
    
    next_powerup_spawn[icon_add] = SDL_GetTicks() + rand() % 14000;
    next_powerup_spawn[icon_inverted] = SDL_GetTicks() + rand() % 20000;
    
    return;
  }
  else if(__STAGE_POINTS__ > 60000)
  {
    stage_14(true);
    return;
  }
  /* init stage header end */
  
  /* stage content */
  __ENEMY__(14, 22000)
  __ENEMY__(15, 29000)
  __ENEMY__(16, 32000)
  
  __WEAPON__(icon_weapon_grade_4, 30000)
  
  __STATUS__(icon_carry, 30000)
  __STATUS__(icon_borrow, 50000)
  __STATUS__(icon_ansi, 110000)
  
  __ENCHANT__(icon_add, 14000)
  __ENCHANT__(icon_inverted, 20000)
}
static void stage_12(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    reset_stage();
    current_stage = stage_12;
    
    new_powerup_status_increase(icon_ansi);
    
    next_enemy_spawn[14] = SDL_GetTicks() + rand() % 15000;
    next_enemy_spawn[15] = SDL_GetTicks() + rand() % 23000;
    
    next_powerup_spawn[icon_weapon_grade_4] = SDL_GetTicks() + rand() % 40000;
    
    next_powerup_spawn[icon_carry] = SDL_GetTicks() + rand() % 40000;
    next_powerup_spawn[icon_borrow] = SDL_GetTicks() + rand() % 60000;
    next_powerup_spawn[icon_ansi] = SDL_GetTicks() + rand() % 130000;
    
    next_powerup_spawn[icon_add] = SDL_GetTicks() + rand() % 15000;
    next_powerup_spawn[icon_inverted] = SDL_GetTicks() + rand() % 22000;
    
    return;
  }
  else if(__STAGE_POINTS__ > 48000)
  {
    stage_13(true);
    return;
  }
  /* init stage header end */
  
  /* stage content */
  __ENEMY__(14, 15000)
  __ENEMY__(15, 23000)
  
  __WEAPON__(icon_weapon_grade_4, 40000)
  
  __STATUS__(icon_carry, 40000)
  __STATUS__(icon_borrow, 60000)
  __STATUS__(icon_ansi, 130000)
  
  __ENCHANT__(icon_add, 15000)
  __ENCHANT__(icon_inverted, 22000)
}
static void stage_11(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    reset_stage();
    current_stage = stage_11;
    
    new_powerup_weapon(icon_weapon_grade_4);
    
    next_enemy_spawn[14] = SDL_GetTicks() + rand() % 9000;
    
    next_powerup_spawn[icon_weapon_grade_4] = SDL_GetTicks() + rand() % 40000;
    
    next_powerup_spawn[icon_carry] = SDL_GetTicks() + rand() % 40000;
    next_powerup_spawn[icon_borrow] = SDL_GetTicks() + rand() % 60000;
    next_powerup_spawn[icon_ansi] = SDL_GetTicks() + rand() % 130000;
    
    next_powerup_spawn[icon_add] = SDL_GetTicks() + rand() % 30000;
    next_powerup_spawn[icon_problem] = SDL_GetTicks() + rand() % 30000;
    next_powerup_spawn[icon_inverted] = SDL_GetTicks() + rand() % 30000;
    next_powerup_spawn[icon_idempotency] = SDL_GetTicks() + rand() % 50000;
    
    return;
  }
  else if(__STAGE_POINTS__ > 28000)
  {
    stage_12(true);
    return;
  }
  /* init stage header end */
  
  /* stage content */
  __ENEMY__(14, 9000)
  
  __WEAPON__(icon_weapon_grade_4, 40000)
  
  __STATUS__(icon_carry, 40000)
  __STATUS__(icon_borrow, 60000)
  __STATUS__(icon_ansi, 130000)
  
  __ENCHANT__(icon_add, 30000)
  __ENCHANT__(icon_problem, 30000)
  __ENCHANT__(icon_inverted, 30000)
  __ENCHANT__(icon_idempotency, 50000)
}
static void stage_10_BOSS(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    free_all_enemys();
    
    reset_stage();
    current_stage = stage_10_BOSS;
    current_boss = spawn_enemy(13);
    set_scrolling_speed(0.0, 0.0);
    
    next_powerup_spawn[icon_carry] = SDL_GetTicks() + rand() % 20000;
    
    next_powerup_spawn[icon_inverted] = SDL_GetTicks() + rand() % 20000;
    next_powerup_spawn[icon_problem] = SDL_GetTicks() + rand() % 20000;
    next_powerup_spawn[icon_add] = SDL_GetTicks() + rand() % 2000;
  }
  if(__STAGE_POINTS__ > 50000)
  {
    current_boss = NULL;
    set_scrolling_speed(-0.035, 0);
    stage_11(true);
    return;
  }
  /* init stage header end */
  
  /* stage content */
  __STATUS__(icon_carry, 20000)
  
  __ENCHANT__(icon_inverted, 20000)
  __ENCHANT__(icon_problem, 20000)
  __ENCHANT__(icon_add, 20000)
  
  if((ship->move_block) || (ship->weapon_block))
  {
    current_boss->speed = 0.2;
    current_boss->ai_function = AI_move_into_screen_and_shot;
    current_boss->wpn_type = BOSS_plus_blaster;
  }
  else if(ship->y + get_spaceship_h() > current_boss->y &&
          ship->y < current_boss->y + get_enemy_h(current_boss->level))
  {
    current_boss->speed = 0.05;
    current_boss->ai_function = AI_move_into_screen_and_dodge_quick;
    current_boss->wpn_type = BOSS_dark_cannon;
  }
  else
  {
    current_boss->speed = 0.05;
    current_boss->ai_function = AI_move_into_screen_and_dodge;
    current_boss->wpn_type = BOSS_minus_thrower;
  }
}
static void stage_10_bonus(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    reset_stage();
    current_stage = stage_10_bonus;
    
    next_powerup_spawn[0] = SDL_GetTicks() + 3000;
    next_powerup_spawn[1] = SDL_GetTicks() + 6000;
    next_powerup_spawn[2] = SDL_GetTicks() + 9000;
    next_powerup_spawn[3] = SDL_GetTicks() + 12000;
    next_powerup_spawn[4] = SDL_GetTicks() + 15000;
    next_powerup_spawn[5] = SDL_GetTicks() + 18000;
  }
  /* init stage header end */
  
  /* stage content */
  if(SDL_GetTicks() > next_powerup_spawn[0])
  {
    new_powerup_status_increase(icon_ansi);
    next_powerup_spawn[0] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[1])
  {
    new_powerup_status_increase(icon_complement);
    next_powerup_spawn[1] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[2])
  {
    new_powerup_status_increase(icon_complement);
    next_powerup_spawn[2] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[3])
  {
    new_powerup_status_increase(icon_borrow);
    next_powerup_spawn[3] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[4])
  {
    new_powerup_status_increase(icon_borrow);
    next_powerup_spawn[4] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[5])
  {
    stage_10_BOSS(true);
    return;
  }
}
static void stage_10(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    reset_stage();
    current_stage = stage_10;
    
    new_powerup_status_increase(icon_complement);
    
    next_enemy_spawn[9] = SDL_GetTicks() + rand() % 12000;
    next_enemy_spawn[11] = SDL_GetTicks() + rand() % 40000;
    next_enemy_spawn[12] = SDL_GetTicks() + rand() % 30000;
    
    next_powerup_spawn[icon_weapon_grade_3] = SDL_GetTicks() + rand() % 40000;
    
    next_powerup_spawn[icon_carry] = SDL_GetTicks() + rand() % 40000;
    next_powerup_spawn[icon_borrow] = SDL_GetTicks() + rand() % 60000;
    next_powerup_spawn[icon_ansi] = SDL_GetTicks() + rand() % 130000;
    
    next_powerup_spawn[icon_add] = SDL_GetTicks() + rand() % 25000;
    next_powerup_spawn[icon_problem] = SDL_GetTicks() + rand() % 25000;
    next_powerup_spawn[icon_idempotency] = SDL_GetTicks() + rand() % 50000;
    
    return;
  }
  else if(__STAGE_POINTS__ > 12000)
  {
    stage_10_bonus(true);
    return;
  }
  /* init stage header end */
  
  /* stage content */
  __ENEMY__(9, 12000)
  __ENEMY__(11, 40000)
  __ENEMY__(12, 30000)
  
  __WEAPON__(icon_weapon_grade_3, 40000)
  
  __STATUS__(icon_carry, 40000)
  __STATUS__(icon_borrow, 60000)
  __STATUS__(icon_ansi, 130000)
  
  __ENCHANT__(icon_add, 25000)
  __ENCHANT__(icon_problem, 25000)
  __ENCHANT__(icon_idempotency, 50000)
}
static void stage_9(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    reset_stage();
    current_stage = stage_9;
    
    new_powerup_status_increase(icon_complement);
    
    next_enemy_spawn[9] = SDL_GetTicks() + rand() % 20000;
    next_enemy_spawn[10] = SDL_GetTicks() + rand() % 24000;
    next_enemy_spawn[11] = SDL_GetTicks() + rand() % 40000;
    
    next_powerup_spawn[icon_weapon_grade_3] = SDL_GetTicks() + rand() % 40000;
    
    next_powerup_spawn[icon_carry] = SDL_GetTicks() + rand() % 40000;
    next_powerup_spawn[icon_borrow] = SDL_GetTicks() + rand() % 60000;
    next_powerup_spawn[icon_ansi] = SDL_GetTicks() + rand() % 130000;
    
    next_powerup_spawn[icon_inhibition] = SDL_GetTicks() + rand() % 140000;
    
    next_powerup_spawn[icon_add] = SDL_GetTicks() + rand() % 25000;
    next_powerup_spawn[icon_idempotency] = SDL_GetTicks() + rand() % 50000;
    
    return;
  }
  else if(__STAGE_POINTS__ > 7000)
  {
    stage_10(true);
    return;
  }
  /* init stage header end */
  
  /* stage content */
  __ENEMY__(9, 20000)
  __ENEMY__(10, 24000)
  __ENEMY__(11, 40000)
  
  __WEAPON__(icon_weapon_grade_3, 40000)
  
  __STATUS__(icon_carry, 40000)
  __STATUS__(icon_borrow, 60000)
  __STATUS__(icon_ansi, 130000)
  
  __ENCHANT__(icon_inhibition, 140000)
  
  __ENCHANT__(icon_add, 25000)
  __ENCHANT__(icon_idempotency, 50000)
}
static void stage_8(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    reset_stage();
    current_stage = stage_8;
    
    next_enemy_spawn[9] = SDL_GetTicks() + rand() % 9000;
    next_enemy_spawn[10] = SDL_GetTicks() + rand() % 40000;
    
    next_powerup_spawn[icon_weapon_grade_3] = SDL_GetTicks() + rand() % 60000;
    
    next_powerup_spawn[icon_carry] = SDL_GetTicks() + rand() % 50000;
    next_powerup_spawn[icon_borrow] = SDL_GetTicks() + rand() % 70000;
    next_powerup_spawn[icon_ansi] = SDL_GetTicks() + rand() % 140000;
    
    next_powerup_spawn[icon_inhibition] = SDL_GetTicks() + rand() % 140000;
    
    next_powerup_spawn[icon_add] = SDL_GetTicks() + rand() % 25000;
    
    return;
  }
  else if(__STAGE_POINTS__ > 5000)
  {
    stage_9(true);
    return;
  }
  /* init stage header end */
  
  /* stage content */
  __ENEMY__(9, 9000)
  __ENEMY__(10, 40000)
  
  __WEAPON__(icon_weapon_grade_3, 60000)
  
  __STATUS__(icon_carry, 50000)
  __STATUS__(icon_borrow, 70000)
  __STATUS__(icon_ansi, 140000)
  
  __ENCHANT__(icon_inhibition, 140000)
  
  __ENCHANT__(icon_add, 25000)
}
static void stage_7_bonus(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    reset_stage();
    current_stage = stage_7_bonus;
    
    next_powerup_spawn[0] = SDL_GetTicks() + 3000;
    next_powerup_spawn[1] = SDL_GetTicks() + 6000;
    next_powerup_spawn[2] = SDL_GetTicks() + 9000;
    next_powerup_spawn[3] = SDL_GetTicks() + 12000;
    next_powerup_spawn[4] = SDL_GetTicks() + 15000;
    next_powerup_spawn[5] = SDL_GetTicks() + 18000;
    next_powerup_spawn[6] = SDL_GetTicks() + 21000;
    next_powerup_spawn[7] = SDL_GetTicks() + 24000;
    next_powerup_spawn[8] = SDL_GetTicks() + 27000;
    next_powerup_spawn[9] = SDL_GetTicks() + 30000;
    next_powerup_spawn[10] = SDL_GetTicks() + 33000;
    next_powerup_spawn[11] = SDL_GetTicks() + 36000;
  }
  /* init stage header end */
  
  /* stage content */
  if(SDL_GetTicks() > next_powerup_spawn[0])
  {
    new_powerup_status_increase(icon_ansi);
    next_powerup_spawn[0] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[1])
  {
    new_powerup_status_increase(icon_complement);
    next_powerup_spawn[1] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[2])
  {
    new_powerup_status_increase(icon_complement);
    next_powerup_spawn[2] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[3])
  {
    new_powerup_status_increase(icon_complement);
    next_powerup_spawn[3] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[4])
  {
    new_powerup_status_increase(icon_borrow);
    next_powerup_spawn[4] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[5])
  {
    new_powerup_status_increase(icon_borrow);
    next_powerup_spawn[5] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[6])
  {
    new_powerup_status_increase(icon_borrow);
    next_powerup_spawn[6] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[7])
  {
    new_powerup_status_increase(icon_function);
    next_powerup_spawn[7] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[8])
  {
    new_powerup_status_increase(icon_function);
    next_powerup_spawn[8] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[9])
  {
    new_powerup_status_increase(icon_or);
    next_powerup_spawn[9] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[10])
  {
    new_powerup_status_increase(icon_or);
    next_powerup_spawn[10] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[11])
  {
    new_powerup_weapon(icon_weapon_grade_3);
    
    stage_8(true);
    return;
  }
}
static void stage_7_BOSS(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    free_all_enemys();
    
    reset_stage();
    current_stage = stage_7_BOSS;
    current_boss = spawn_enemy(8);
    set_scrolling_speed(0.0, 0.0);
    
    new_powerup_status_increase(icon_ansi);
    
    next_powerup_spawn[icon_carry] = SDL_GetTicks() + rand() % 30000;
    next_powerup_spawn[icon_ansi] = SDL_GetTicks() + rand() % 80000;
    
    next_powerup_spawn[icon_inverted] = SDL_GetTicks() + rand() % 7500;
    next_powerup_spawn[icon_problem] = SDL_GetTicks() + rand() % 7500;
    next_powerup_spawn[icon_add] = SDL_GetTicks() + rand() % 7500;
  }
  if(__STAGE_POINTS__ > 5000)
  {
    current_boss = NULL;
    set_scrolling_speed(-0.035, 0);
    stage_7_bonus(true);
    return;
  }
  /* init stage header end */
  
  /* stage content */
  __STATUS__(icon_carry, 30000)
  __STATUS__(icon_ansi, 80000)
  
  __ENCHANT__(icon_inverted, 7500)
  __ENCHANT__(icon_problem, 7500)
  __ENCHANT__(icon_add, 7500)
  
  if(ship->y + get_spaceship_h() > current_boss->y &&
     ship->y < current_boss->y + get_enemy_h(current_boss->level))
    current_boss->wpn_type = BOSS_dark_blaster;
  else
    current_boss->wpn_type = BOSS_red_plus_thrower;
}
static void stage_7(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    reset_stage();
    current_stage = stage_7;
    
    next_enemy_spawn[5] = SDL_GetTicks() + rand() % 10000;
    next_enemy_spawn[6] = SDL_GetTicks() + rand() % 12000;
    next_enemy_spawn[7] = SDL_GetTicks() + rand() % 28000;
    
    next_powerup_spawn[icon_weapon_grade_2] = SDL_GetTicks() + rand() % 70000;
    
    next_powerup_spawn[icon_carry] = SDL_GetTicks() + rand() % 50000;
    next_powerup_spawn[icon_borrow] = SDL_GetTicks() + rand() % 50000;
    next_powerup_spawn[icon_ansi] = SDL_GetTicks() + rand() % 140000;
    
    next_powerup_spawn[icon_inverted] = SDL_GetTicks() + rand() % 35000;
    next_powerup_spawn[icon_add] = SDL_GetTicks() + rand() % 35000;
  }
  if(__STAGE_POINTS__ > 4350)
  {
    stage_7_BOSS(true);
    return;
  }
  /* init stage header end */
  
  /* stage content */
  __ENEMY__(5, 10000)
  __ENEMY__(6, 12000)
  __ENEMY__(7, 28000)
  
  __WEAPON__(icon_weapon_grade_2, 70000)
  
  __STATUS__(icon_carry, 50000)
  __STATUS__(icon_borrow, 50000)
  __STATUS__(icon_ansi, 140000)
  
  __ENCHANT__(icon_inverted, 35000)
  __ENCHANT__(icon_add, 35000)
}
static void stage_6_bonus(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    reset_stage();
    current_stage = stage_6_bonus;
    
    next_powerup_spawn[icon_ansi] = SDL_GetTicks() + 3000;
    next_powerup_spawn[icon_borrow] = SDL_GetTicks() + 6000;
    next_powerup_spawn[icon_complement] = SDL_GetTicks() + 9000;
    next_powerup_spawn[icon_or] = SDL_GetTicks() + 12000;
    next_powerup_spawn[icon_function] = SDL_GetTicks() +15000;
  }
  /* init stage header end */
  
  /* stage content */
  if(SDL_GetTicks() > next_powerup_spawn[icon_ansi])
  {
    new_powerup_status_increase(icon_ansi);
    next_powerup_spawn[icon_ansi] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[icon_borrow])
  {
    new_powerup_status_increase(icon_borrow);
    next_powerup_spawn[icon_borrow] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[icon_complement])
  {
    new_powerup_status_increase(icon_complement);
    next_powerup_spawn[icon_complement] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[icon_or])
  {
    new_powerup_status_increase(icon_or);
    next_powerup_spawn[icon_or] = SDL_GetTicks() + 900000;
  }
  else if(SDL_GetTicks() > next_powerup_spawn[icon_function])
  {
    new_powerup_status_increase(icon_function);
    
    stage_7(true);
    return;
  }
}
static void stage_6(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    reset_stage();
    current_stage = stage_6;
    
    new_powerup_status_increase(icon_complement);
    
    next_enemy_spawn[5] = SDL_GetTicks() + rand() % 6000;
    next_enemy_spawn[6] = SDL_GetTicks() + rand() % 12000;
    
    next_powerup_spawn[icon_weapon_grade_2] = SDL_GetTicks() + rand() % 45000;
    
    next_powerup_spawn[icon_carry] = SDL_GetTicks() + rand() % 50000;
    next_powerup_spawn[icon_borrow] = SDL_GetTicks() + rand() % 50000;
    next_powerup_spawn[icon_ansi] = SDL_GetTicks() + rand() % 140000;
    
    next_powerup_spawn[icon_inverted] = SDL_GetTicks() + rand() % 50000;
    next_powerup_spawn[icon_idempotency] = SDL_GetTicks() + rand() % 50000;
  }
  if(__STAGE_POINTS__ > 3000)
  {
    stage_6_bonus(true);
    return;
  }
  /* init stage header end */
  
  /* stage content */
  __ENEMY__(5, 12000)
  __ENEMY__(6, 30000)
  
  __WEAPON__(icon_weapon_grade_2, 45000)
  
  __STATUS__(icon_carry, 50000)
  __STATUS__(icon_borrow, 50000)
  __STATUS__(icon_ansi, 140000)
  
  __ENCHANT__(icon_inverted, 50000)
  __ENCHANT__(icon_idempotency, 50000)
}
static void stage_5(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    reset_stage();
    current_stage = stage_5;
    
    new_powerup_status_increase(icon_complement);
    
    next_enemy_spawn[5] = SDL_GetTicks() + rand() % 12000;
    
    next_powerup_spawn[icon_weapon_grade_1] = SDL_GetTicks() + rand() % 30000;
    next_powerup_spawn[icon_weapon_grade_2] = SDL_GetTicks() + rand() % 45000;
    
    next_powerup_spawn[icon_carry] = SDL_GetTicks() + rand() % 45000;
    next_powerup_spawn[icon_borrow] = SDL_GetTicks() + rand() % 60000;
    next_powerup_spawn[icon_ansi] = SDL_GetTicks() + rand() % 90000;
    
    next_powerup_spawn[icon_problem] = SDL_GetTicks() + rand() % 50000;
    next_powerup_spawn[icon_inverted] = SDL_GetTicks() + rand() % 50000;
    next_powerup_spawn[icon_idempotency] = SDL_GetTicks() + rand() % 70000;
  }
  if(__STAGE_POINTS__ > 1500)
  {
    stage_6(true);
    return;
  }
  /* init stage header end */
  
  /* stage content */
  __ENEMY__(5, 12000)
  
  __WEAPON__(icon_weapon_grade_1, 30000)
  __WEAPON__(icon_weapon_grade_2, 45000)
  
  __STATUS__(icon_carry, 45000)
  __STATUS__(icon_borrow, 60000)
  __STATUS__(icon_ansi, 90000)
  
  __ENCHANT__(icon_problem, 50000)
  __ENCHANT__(icon_inverted, 50000)
  __ENCHANT__(icon_idempotency, 70000)
}
static void stage_4_BOSS(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    free_all_enemys();
    
    reset_stage();
    current_stage = stage_4_BOSS;
    current_boss = spawn_enemy(4);
    set_scrolling_speed(0.0, 0.0);
    
    new_powerup_status_increase(icon_ansi);
    
    next_powerup_spawn[icon_carry] = SDL_GetTicks() + rand() % 30000;
    next_powerup_spawn[icon_ansi] = SDL_GetTicks() + rand() % 80000;
    
    next_powerup_spawn[icon_inverted] = SDL_GetTicks() + rand() % 5000;
    next_powerup_spawn[icon_problem] = SDL_GetTicks() + rand() % 5000;
    
    /* use icon_weapon_grade_0 as weapon change timer */
    next_powerup_spawn[icon_weapon_grade_0] = SDL_GetTicks() + rand() % 20000;
  }
  if(__STAGE_POINTS__ > 3000)
  {
    current_boss = NULL;
    set_scrolling_speed(-0.035, 0);
    stage_5(true);
    return;
  }
  /* init stage header end */
  
  /* stage content */
  __STATUS__(icon_carry, 30000)
  __STATUS__(icon_ansi, 80000)
  
  if(current_boss->wpn_type == BOSS_red_laser_thrower)
  {
    __ENCHANT__(icon_inverted, 5000)
    __ENCHANT__(icon_problem, 5000)
  }
  
  /* use icon_weapon_grade_0 as weapon change timer */
  if(SDL_GetTicks() > next_powerup_spawn[icon_weapon_grade_0])
  {
    next_powerup_spawn[icon_weapon_grade_0] = SDL_GetTicks() + rand() % 20000;
    current_boss->wpn_type = BOSS_needle_blaster + rand() % 2;
  }
}
static void stage_4(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    reset_stage();
    current_stage = stage_4;
    
    new_powerup_status_increase(icon_complement);
    
    next_enemy_spawn[1] = SDL_GetTicks() + rand() % 6000;
    next_enemy_spawn[2] = SDL_GetTicks() + rand() % 23500;
    next_enemy_spawn[3] = SDL_GetTicks() + rand() % 30000;
    
    next_powerup_spawn[icon_weapon_grade_1] = SDL_GetTicks() + rand() % 30000;
    
    next_powerup_spawn[icon_carry] = SDL_GetTicks() + rand() % 30000;
    next_powerup_spawn[icon_borrow] = SDL_GetTicks() + rand() % 50000;
    next_powerup_spawn[icon_ansi] = SDL_GetTicks() + rand() % 80000;
    
    next_powerup_spawn[icon_inverted] = SDL_GetTicks() + rand() % 30000;
    next_powerup_spawn[icon_problem] = SDL_GetTicks() + rand() % 30000;
  }
  if(__STAGE_POINTS__ > 1250)
  {
    stage_4_BOSS(true);
    return;
  }
  /* init stage header end */
  
  /* stage content */
  __ENEMY__(1, 6000)
  __ENEMY__(2, 23500)
  __ENEMY__(3, 30000)
  
  __WEAPON__(icon_weapon_grade_1, 30000)
  
  __STATUS__(icon_carry, 30000)
  __STATUS__(icon_borrow, 50000)
  __STATUS__(icon_ansi, 80000)
  
  __ENCHANT__(icon_inverted, 30000)
  __ENCHANT__(icon_problem, 30000)
}
static void stage_3(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    reset_stage();
    current_stage = stage_3;
    
    new_powerup_status_increase(icon_complement);
    
    next_enemy_spawn[0] = SDL_GetTicks() + rand() % 8000;
    next_enemy_spawn[1] = SDL_GetTicks() + rand() % 8000;
    next_enemy_spawn[2] = SDL_GetTicks() + rand() % 23500;
    
    next_powerup_spawn[icon_weapon_grade_0] = SDL_GetTicks() + rand() % 20000;
    next_powerup_spawn[icon_weapon_grade_1] = SDL_GetTicks() + rand() % 30000;
    
    next_powerup_spawn[icon_carry] = SDL_GetTicks() + rand() % 30000;
    next_powerup_spawn[icon_borrow] = SDL_GetTicks() + rand() % 50000;
    
    next_powerup_spawn[icon_7] = SDL_GetTicks() + rand() % 30000;
    next_powerup_spawn[icon_problem] = SDL_GetTicks() + rand() % 30000;
  }
  if(__STAGE_POINTS__ > 2000)
  {
    stage_4(true);
    return;
  }
  /* init stage header end */
  
  /* stage content */
  __ENEMY__(0, 9000)
  __ENEMY__(1, 9000)
  __ENEMY__(2, 23500)
  
  __WEAPON__(icon_weapon_grade_0, 20000)
  __WEAPON__(icon_weapon_grade_1, 30000)
  
  __STATUS__(icon_carry, 30000)
  __STATUS__(icon_borrow, 50000)
  __STATUS__(icon_7, 30000)
  
  __ENCHANT__(icon_problem, 30000)
}
static void stage_2(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    new_powerup_status_increase(icon_complement);
    
    reset_stage();
    current_stage = stage_2;
    
    next_enemy_spawn[0] = SDL_GetTicks() + rand() % 8000;
    next_enemy_spawn[1] = SDL_GetTicks() + rand() % 8000;
    
    next_powerup_spawn[icon_weapon_grade_0] = SDL_GetTicks() + rand() % 20000;
    next_powerup_spawn[icon_problem] = SDL_GetTicks() + rand() % 10000;
  }
  else if(__STAGE_POINTS__ > 300)
  {
    stage_3(true);
    return;
  }
  /* init stage header end */
  
  /* stage content */
  __ENEMY__(0, 9000)
  __ENEMY__(1, 9000)
  
  __WEAPON__(icon_weapon_grade_0, 20000)
  
  __ENCHANT__(icon_problem, 45000)
  
}
static void stage_1(const bool init_stage)
{
  /* init stage header start */
  if(init_stage)
  {
    reset_stage();
    current_stage = stage_1;
    
    next_enemy_spawn[0] = SDL_GetTicks() + rand() % 6000;
    
    return;
  }
  else if(__STAGE_POINTS__ > 150)
  {
    stage_2(true);
    return;
  }
  /* init stage header end */
  
  /* stage content */
  __ENEMY__(0, 6000)
}
/* stages end */

static void reset_game(void)
{
  reset_scrolling_map();
  
  free_all_bullets();
  free_all_enemys();
  free_all_powerups();
  free_all_scrolling_texts();
  
  reset_spaceship();
}

static void game_exit(void)
{
  TTF_CloseFont(game_font);
  TTF_CloseFont(game_font_win);
}
int init_game(void)
{
  if(!init_scrolling_map())
    return 0;
  
  if(!init_bullet())
    return 0;
  
  if(!init_spaceship())
    return 0;
  ship = get_spaceship();
  
  if(!init_enemys())
    return 0;
  
  if(!init_scrolling_text())
    return 0;
  
  if(!init_weapons())
    return 0;
  
  if(!init_powerups())
    return 0;
  
  if(!init_interface())
    return 0;
  
  if(!(game_font = TTF_OpenFont("gfx/LiberationSans-Regular.ttf", 26)))
    return 0;
  if(!(game_font_win = TTF_OpenFont("gfx/LiberationSans-Regular.ttf", 32)))
    return 0;
  
  atexit(game_exit);
  
  return 1;
}
void game_pause(void)
{
  SDL_Surface *screen = SDL_GetVideoSurface();
  Uint32 pause_start = SDL_GetTicks();
  
  /* grey out screen */
  boxRGBA(screen, 0, 0, screen->w - 1, screen->h - 1, 0, 0, 0, 127);
  
  /* write 'game paused' */
  SDL_Color text_color = {0, 0, 0, 0};
  SDL_Surface *text_pause_surface = TTF_RenderText_Blended(game_font, "- GAME PAUSED - PRESS R TO RESTART -", text_color);;
  SDL_Rect text_pause_dst = {screen->w/2 - text_pause_surface->w/2, screen->h - text_pause_surface->h, 0, 0};
  SDL_BlitSurface(text_pause_surface, NULL, screen, &text_pause_dst);
  SDL_FreeSurface(text_pause_surface);
  
  SDL_Event event;
  while(GAME_MODE == game_paused)
  {
    SDL_WaitEvent(&event);
    
    /* handle events */
    if(event.type == SDL_KEYDOWN)
    {
      if(event.key.keysym.sym == SDLK_ESCAPE)
        GAME_MODE = game_running;
      else if(event.key.keysym.sym == SDLK_r)
        GAME_MODE = game_restart;
    }
    else if(event.type == SDL_QUIT)
      GAME_MODE = game_quit;
    
    /* update screen */
    SDL_Flip(screen);
  }
  
  /* extend all timestamps */
  extend_all_bullets_timestamp(SDL_GetTicks() - pause_start);
  extend_all_enemys_timestamp(SDL_GetTicks() - pause_start);
  extend_all_powerups_timestamp(SDL_GetTicks() - pause_start);
  extend_all_scrolling_texts_timestamp(SDL_GetTicks() - pause_start);
  extend_scrolling_map_timestamp(SDL_GetTicks() - pause_start);
  extend_spaceship_enchantments_timestamp(SDL_GetTicks() - pause_start);
  extend_spaceship_timestamp(SDL_GetTicks() - pause_start);
  
  /* extend stage timestamps */
  int counter;
  
  for(counter = 0; counter < ENEMY_LEVEL_MAX; counter++)
    next_enemy_spawn[counter] += SDL_GetTicks() - pause_start;
  
  for(counter = 0; counter < POWERUP_ICON_MAX; counter++)
    next_powerup_spawn[counter] += SDL_GetTicks() - pause_start;
}
void game_run(void)
{
  SDL_Surface *screen = SDL_GetVideoSurface();
  SDL_Event event;
  Uint8 key_button[323] = {0};
  Uint8 autofire = 0;
  Uint8 background = 1;
  
  /* set game properties */
  reset_game();
  set_scrolling_speed(-0.035, 0);
  double dir_x = 1;
  double dir_y = 1;
  int counter;
  
  /* setup spaceship */
  ship->x = 0;
  ship->y = screen->h/2 - get_spaceship_h()/2 - 50;
  
  /* fade in loop */
  double fade_alpha = 1.0;
  Uint32 last_fade = SDL_GetTicks();
  while(fade_alpha > 0)
  {
    /* poll events */
    while(SDL_PollEvent(&event))
    {
      if(event.type == SDL_QUIT)
      {
        fade_alpha = 0;
        GAME_MODE = game_quit;
      }
    }
    
    /* handle everything */
    scroll_map();
    accelerate_spaceship(dir_x, dir_y);
    handle_spaceship();
    
    /* blit everything */
    blit_scrolling_map(screen);
    blit_spaceship(screen);
    blit_interface(screen);
    
    /* fade in */
    fade_alpha -= (SDL_GetTicks() - last_fade) * 0.005;
    
    if(fade_alpha < 0.0)
      fade_alpha = 0.0;
    last_fade = SDL_GetTicks();
    
    /* grey out screen */
    boxRGBA(screen, 0, 0, screen->w, screen->h, 0, 0, 0, 255 * fade_alpha);
    
    /* update screen and keep framerate */
    SDL_Flip(screen);
    controll_framerate();
  }
  
  /* main loop */
  stage_1(true);
  Uint32 game_startet = SDL_GetTicks();
  Uint32 game_pause_stop;
  
  while(GAME_MODE == game_running)
  {
    /* poll events */
    while(SDL_PollEvent(&event))
    {
      if(event.type == SDL_KEYDOWN)
      {
        if(event.key.keysym.sym == SDLK_TAB)
        {
          if(autofire)
          {
            autofire = 0;
            new_scrolling_text(ship->x + get_spaceship_w()/2, ship->y + get_spaceship_h()/2, "Autofire off", 255, 0, 0);
          }
          else
          {
            autofire = 1;
            new_scrolling_text(ship->x + get_spaceship_w()/2, ship->y + get_spaceship_h()/2, "Autofire on", 0, 0, 255);
          }
        }
        else if(event.key.keysym.sym == SDLK_PRINT)
        {
          if(background)
          {
            background = 0;
            new_scrolling_text(ship->x + get_spaceship_w()/2, ship->y + get_spaceship_h()/2, "Background off", 255, 0, 0);
          }
          else
          {
            background = 1;
            new_scrolling_text(ship->x + get_spaceship_w()/2, ship->y + get_spaceship_h()/2, "Background on", 0, 0, 255);
          }
        }
        else
          key_button[event.key.keysym.sym] = 1;
      }
      if(event.type == SDL_KEYUP)
        key_button[event.key.keysym.sym] = 0;
      else if(event.type == SDL_QUIT)
        GAME_MODE = game_quit;
    }
    
    /* handle input start */
    if(key_button[SDLK_ESCAPE])
    {
      GAME_MODE = game_paused;
      
      /* unset all pressed keys */
      for(counter = 0; counter < 323; counter++)
        key_button[counter] = 0;
      
      game_pause_stop = SDL_GetTicks();
      game_pause();
      if(GAME_MODE == game_restart)
        return;
      
      game_startet += SDL_GetTicks() - game_pause_stop;
    }
    
    /* handle directions */
    if(key_button[SDLK_LEFT])
      dir_x = -1;
    else if(key_button[SDLK_RIGHT])
      dir_x = 1;
    else
      dir_x = 0;
    
    if(key_button[SDLK_UP])
      dir_y = -1;
    else if(key_button[SDLK_DOWN])
      dir_y = 1;
    else
      dir_y = 0;
    
    /* handle shooting */
    if((key_button[SDLK_SPACE]) || (autofire))
      spaceship_shot();
    /* handle input end */
    
    /* handle everything */
    current_stage(false);
    
    handle_collision();
    handle_spaceship_enchantments();
    scroll_map();
    handle_all_enemys();
    move_all_bullets();
    move_all_powerups();
    accelerate_spaceship(dir_x, dir_y);
    handle_spaceship();
    
    if(ship->hp <= 0)
    {
      GAME_MODE = game_over;
      break;
    }
    
    /* blit everything */
    if(background)
      blit_scrolling_map(screen);
    else
      SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
    
    blit_all_enemys(screen);
    blit_all_bullets(screen);
    blit_all_powerups(screen);
    blit_spaceship(screen);
    blit_all_scrolling_texts(screen);
    blit_interface(screen);
    
    /* update screen and keep framerate */
    SDL_Flip(screen);
    controll_framerate();
  }
}
void game_run_menu(void)
{
  SDL_Surface *screen = SDL_GetVideoSurface();
  
  /* reset some values */
  reset_game();
  
  ship->speed = 0.05;
  ship->acceleration = 0.000075;
  double dir_x = 0;
  double dir_y = -1;
  
  set_scrolling_speed(-0.015, 0);
  
  /* text */
  SDL_Color text_color = {0, 0, 0, 0};
  
  SDL_Surface *text_space_surface = TTF_RenderText_Blended(game_font, "- PRESS SPACE -", text_color);
  SDL_Rect text_space_dst = {screen->w/2 - text_space_surface->w/2, screen->h - text_space_surface->h, 0, 0};
  
  SDL_Surface *text_de_surface = TTF_RenderText_Blended(game_font, "Digital Invaders", text_color);
  SDL_Rect text_de_dst = {screen->w/2 - text_de_surface->w/2, screen->h/2 - text_de_surface->h/2 - 33, 0, 0};
  
  Uint32 last_fade = SDL_GetTicks();
  double fade_alpha = 1.0;
  
  SDL_Event event;
  Uint8 runlevel = 0; /* 0 = fade in; 1 = fade out */
  while(GAME_MODE == game_menu)
  {
    /* poll and handle events */
    while(SDL_PollEvent(&event))
    {
      if(runlevel == 0 &&
         event.type == SDL_KEYDOWN &&
         event.key.keysym.sym == SDLK_SPACE)
      {
        runlevel = 1;
        
        dir_y = 0;
        dir_x = 1;
        
        last_fade = SDL_GetTicks();
      }
      else if(event.type == SDL_QUIT)
      {
        GAME_MODE = game_quit;
      }
    }
    
    /* draw all */
    scroll_map();
    blit_scrolling_map(screen);
    
    accelerate_spaceship(dir_x, dir_y);
    handle_spaceship();
    blit_spaceship(screen);
    
    /* draw grey line */
    boxRGBA(screen, 0, 52, screen->w - 97, 57, 128, 128, 128, 255);
    
    /* write texts */
    SDL_BlitSurface(text_space_surface, NULL, screen, &text_space_dst);
    SDL_BlitSurface(text_de_surface, NULL, screen, &text_de_dst);
    
    /* handle runlevel */
    if(runlevel == 0)
    {
      /* move ship */
      if(ship->y < 150 && dir_y < 0)
        dir_y = 1;
      if(ship->y > screen->h - 150 && dir_y > 0)
        dir_y = -1;
      
      /* fade in */
      fade_alpha -= (SDL_GetTicks() - last_fade) * 0.005;
      
      if(fade_alpha < 0.25)
        fade_alpha = 0.25;
    }
    else if(runlevel == 1)
    {
      /* fade out screen */
      fade_alpha += (SDL_GetTicks() - last_fade) * 0.0015;
      
      if(fade_alpha > 1.0)
        fade_alpha = 1.0;
      
      if(ship->x >= screen->w - get_spaceship_w())
        GAME_MODE = game_running;
      
      ship->speed = 2.5;
      ship->acceleration = 0.00175;
    }
    
    /* grey out screen */
    boxRGBA(screen, 0, 0, screen->w, screen->h, 0, 0, 0, 255 * fade_alpha);
    last_fade = SDL_GetTicks();
    
    /* update screen and keep framerate */
    SDL_Flip(screen);
    controll_framerate();
  }
  
  SDL_FreeSurface(text_space_surface);
  SDL_FreeSurface(text_de_surface);
}
void game_run_over(void)
{
  SDL_Surface *screen = SDL_GetVideoSurface();
  Uint32 over_start = SDL_GetTicks();
  
  /* render text */
  SDL_Color text_color = {255, 0, 0, 0};
  SDL_Surface *text_over_surface = TTF_RenderText_Blended(game_font, "- GAME OVER -", text_color);;
  SDL_Rect text_over_dst = {screen->w/2 - text_over_surface->w/2, screen->h - text_over_surface->h, 0, 0};
  
  /* take screenshot */
  SDL_Surface *screen_shot = SDL_ConvertSurface(screen, screen->format, screen->flags);
  
  SDL_Event event;
  while(GAME_MODE == game_over)
  {
    /* poll and handle events */
    while(SDL_PollEvent(&event))
    {
      if(event.type == SDL_QUIT)
      {
        GAME_MODE = game_quit;
      }
    }
    
    /* draw all */
    SDL_BlitSurface(screen_shot, NULL, screen, NULL);
    
    /* write text */
    SDL_BlitSurface(text_over_surface, NULL, screen, &text_over_dst);
    
    /* fade out screen */
    if(SDL_GetTicks() - over_start < 1500)
      boxRGBA(screen, 0, 0, screen->w, screen->h, 0, 0, 0, (SDL_GetTicks() - over_start) * 0.17);
    else
    {
      SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
      GAME_MODE = game_menu;
    }
    
    /* update screen and keep framerate */
    SDL_Flip(screen);
    controll_framerate();
  }
  
  SDL_FreeSurface(text_over_surface);
  SDL_FreeSurface(screen_shot);
}
void game_run_outro(void)
{
  SDL_Surface *screen = SDL_GetVideoSurface();
  SDL_Event event;
  set_scrolling_speed(0.0, 0.0);
  
  /* fade out screen */
  double fade_alpha = 0.0;
  Uint32 last_fade = SDL_GetTicks();
  while(fade_alpha < 1.0)
  {
    /* poll and handle events */
    while(SDL_PollEvent(&event))
    {
      if(event.type == SDL_QUIT)
      {
        GAME_MODE = game_quit;
        return;
      }
    }
    
    scroll_map();
    blit_scrolling_map(screen);
    
    accelerate_spaceship(1, 0);
    handle_spaceship();
    blit_spaceship(screen);
    
    fade_alpha += (SDL_GetTicks() - last_fade) * 0.0005;
    
    if(fade_alpha > 1.0)
      fade_alpha = 1.0;
    
    /* grey out screen */
    boxRGBA(screen, 0, 0, screen->w, screen->h, 0, 0, 0, 255 * fade_alpha);
    last_fade = SDL_GetTicks();
    
    /* update screen and keep framerate */
    SDL_Flip(screen);
    controll_framerate();
  }
  
  /* set some settings */
  reset_game();
  ship->x = 0 - get_spaceship_w();
  int dir_y = 1;
  
  /* render text */
  SDL_Color text_color;
  
  SDL_Surface *win_text[5];
  int current_win_text = 0;
  
  text_color.r = 0; text_color.g = 0; text_color.b = 128;
  win_text[0] = TTF_RenderText_Blended(game_font_win, "You beat the game!", text_color);
  
  text_color.r = 128; text_color.g = 0; text_color.b = 0;
  win_text[1] = TTF_RenderText_Blended(game_font_win, "You are a Hero now!", text_color);
  
  text_color.r = 0; text_color.g = 128; text_color.b = 0;
  win_text[2] = TTF_RenderText_Blended(game_font_win, "You, your friends and your family must celebrate this event!", text_color);
  
  text_color.r = 0; text_color.g = 0; text_color.b = 128;
  win_text[3] = TTF_RenderText_Blended(game_font_win, "Press 'Tab' during game to enable autofire", text_color);
  
  text_color.r = 0; text_color.g = 0; text_color.b = 128;
  win_text[4] = TTF_RenderText_Blended(game_font_win, "Press 'Print' to toggle the background", text_color);
  
  double text_dst_x = screen->w;
  SDL_Rect text_dst = {screen->w, 130, 0, 0};
  Uint32 last_text_move = SDL_GetTicks();
  
  /* fade in screen */
  while(current_win_text < 5)
  {
    /* poll and handle events */
    while(SDL_PollEvent(&event))
    {
      if(event.type == SDL_QUIT)
      {
        GAME_MODE = game_quit;
        return;
      }
    }
    
    /* handle map */
    if(get_scrolling_speed_x() > -0.05)
      increase_scrolling_speed(-0.005, 0.0);
    scroll_map();
    blit_scrolling_map(screen);
    
    /* handle and blit spaceship */
    if(ship->y < screen->h * 0.33)
      dir_y = 1;
    else if(ship->y > screen->h * 0.66)
      dir_y = -1;
    
    if(ship->x < 10)
      accelerate_spaceship(1, 0);
    else
    {
      ship->speed = 0.02;
      ship->acceleration = 0.000075;
      accelerate_spaceship(0, dir_y);
    }
    
    handle_spaceship();
    blit_spaceship(screen);
    
    /* fade in screen */
    if(fade_alpha > 0.0)
    {
      fade_alpha -= (SDL_GetTicks() - last_fade) * 0.0015;
      last_fade = SDL_GetTicks();
      
      /* prevent flickering */
      if(fade_alpha < 0.0)
        fade_alpha = 0.0;
      
      boxRGBA(screen, 0, 0, screen->w, screen->h, 0, 0, 0, 255 * fade_alpha);
    }
    
    /* handle, and blit text */
    text_dst_x -= (SDL_GetTicks() - last_text_move) * 0.15;
    last_text_move = SDL_GetTicks();
    
    /* move to next text */
    if(text_dst_x + win_text[current_win_text]->w < 0)
    {
      text_dst_x = screen->w;
      
      if(text_dst.y == 130)
        text_dst.y = screen->h - 200;
      else
        text_dst.y = 130;
      
      current_win_text++;
    }
    if(current_win_text >= 5)
      break;
    
    text_dst.x = (int)text_dst_x;
    SDL_BlitSurface(win_text[current_win_text], NULL, screen, &text_dst);
    
    /* draw cinema-style rects */
    boxRGBA(screen, 0, 0, screen->w, 80, 0, 0, 0, 255);
    boxRGBA(screen, 0, screen->h - 81, screen->w, screen->h, 0, 0, 0, 255);
    
    /* update screen and keep framerate */
    SDL_Flip(screen);
    controll_framerate();
  }
  
  /* free all texts */
  int counter;
  for(counter = 0; counter < 5; counter++)
    SDL_FreeSurface(win_text[counter]); 
  
  /* fade out screen */
  last_fade = SDL_GetTicks();
  while(fade_alpha < 1.0)
  {
    /* poll and handle events */
    while(SDL_PollEvent(&event))
    {
      if(event.type == SDL_QUIT)
      {
        GAME_MODE = game_quit;
        return;
      }
    }
    
    scroll_map();
    blit_scrolling_map(screen);
    
    ship->speed = 2.5;
    ship->acceleration = 0.00175;
    accelerate_spaceship(1, 0);
    handle_spaceship();
    blit_spaceship(screen);
    
    /* draw cinema-style rects */
    boxRGBA(screen, 0, 0, screen->w, 80, 0, 0, 0, 255);
    boxRGBA(screen, 0, screen->h - 81, screen->w, screen->h, 0, 0, 0, 255);
    
    /* grey out screen */
    fade_alpha += (SDL_GetTicks() - last_fade) * 0.0015;
    if(fade_alpha > 1.0)
      fade_alpha = 1.0;
    last_fade = SDL_GetTicks();
    
    boxRGBA(screen, 0, 0, screen->w, screen->h, 0, 0, 0, 255 * fade_alpha);
    
    /* update screen and keep framerate */
    SDL_Flip(screen);
    controll_framerate();
  }
  
  GAME_MODE = game_menu;
}