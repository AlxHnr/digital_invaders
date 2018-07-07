#ifndef POWERUP_H
#define POWERUP_H

#include <string.h>
#include <SDL.h>
#include <SDL_image.h>

#include "graphic.h"
#include "weapon.h"

#define P_BUFFER_LENGTH 128

typedef enum
{
  icon_weapon_grade_0,
  icon_weapon_grade_1,
  icon_weapon_grade_2,
  icon_weapon_grade_3,
  icon_weapon_grade_4,
  
  icon_carry,      /* hp up */
  icon_borrow,     /* max hp up */
  icon_or,         /* def up */
  icon_function,   /* atk up */
  icon_inhibition, /* 10 seconds shield */
  icon_variable,   /* Bonus Points */
  icon_complement, /* level up */
  icon_ansi,       /* full hp */
  
  icon_problem,    /* 5 seconds stop ship */
  icon_subtraction,/* level down */
  icon_7,          /* def/atk/max hp down */
  icon_add,        /* 5 seconds stun */
  icon_inverted,   /* 5 seconds invert control */
  icon_idempotency,/* 5 seconds weapon block */
  
  POWERUP_ICON_MAX
}powerup_icon;

typedef enum
{
  enchantment,
  status_incrase,
  weapon_change
}powerup_type;

typedef struct powerup powerup;
struct powerup
{
  powerup_type type;
  powerup_icon icon;
  
  char text[P_BUFFER_LENGTH];
  Uint8 r, g, b;
  
  double x, y;
  double dir_x, dir_y;
  
  Uint32 last_move;
  double moving_speed;
  
  double speed;
  double acceleration;
  
  weapon_type wpn_type;
  int hp, hp_max;
  int atk, def;
  double fire_rate;
  
  Uint8 invert, weapon_block, move_block;
  
  Uint32 start_time;
  Uint32 duration;
  
  powerup *next;
  powerup *prev;
};

extern int init_powerups(void);

extern powerup *new_powerup(const powerup_type type, const powerup_icon icon,
                            const char *text, const double x, const double y,
                            const double dir_x, const double dir_y,
                            const double moving_speed);

#define new_powerup_enchantment(type) \
                                new_powerup(enchantment, type, "", SDL_GetVideoSurface()->w, \
                                rand() % (SDL_GetVideoSurface()->h - get_powerup_h(type)), -1, 0, 0.3)

#define new_powerup_status_increase(type) \
                                    new_powerup(status_incrase, type, "", SDL_GetVideoSurface()->w, \
                                    rand() % (SDL_GetVideoSurface()->h - get_powerup_h(type)), -1, 0, 0.3)

#define new_powerup_weapon(grade) \
                           new_powerup(weapon_change, grade, "", SDL_GetVideoSurface()->w, \
                           rand() % (SDL_GetVideoSurface()->h - get_powerup_h(grade)), -1, 0, 0.2)

extern void free_powerup(powerup *object);
extern powerup *get_first_powerup(void);

extern int get_powerup_w(const powerup_icon icon);
extern int get_powerup_h(const powerup_icon icon);

extern void move_all_powerups(void);
extern void blit_all_powerups(SDL_Surface *dst);
extern void extend_all_powerups_timestamp(const Uint32 time);
extern void free_all_powerups(void);

#endif