#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <SDL.h>
#include <SDL_image.h>

#include "graphic.h"
#include "weapon.h"
#include "spaceship_enchantments.h"

#define SLOW_DOWN 0.0003

#define SPACESHIP_HP_MAX 7965
#define SPACESHIP_SPEED_MAX 0.3
#define SPACESHIP_ACCELLERATION_MAX 0.0015

typedef struct spaceship spaceship;
struct spaceship
{
  double x, y;
  double movement_x, movement_y;
  double speed;
  double acceleration;
  Uint32 last_move;
  Uint32 last_shot;
  
  unsigned int points;
  weapon_type wpn_type;
  int hp, hp_max;
  int atk, def;
  double fire_rate;
  
  Uint8 invert, weapon_block, move_block;
};

/* return a pointer to the spaceship on success, otherwise NULL */
extern int init_spaceship(void);
extern spaceship *get_spaceship(void);
extern int get_spaceship_w(void);
extern int get_spaceship_h(void);

extern void reset_spaceship(void);
extern void accelerate_spaceship(const int dir_x, const int dir_y);
extern void handle_spaceship(void);
extern void spaceship_shot(void);
extern void blit_spaceship(SDL_Surface *dst);
extern void extend_spaceship_timestamp(const Uint32 time);

#endif