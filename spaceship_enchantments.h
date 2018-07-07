#ifndef SPACESHIP_ENCHANTMENTS_H
#define SPACESHIP_ENCHANTMENTS_H

#include "spaceship.h"
#include "powerup.h"

extern powerup *spaceship_enchant(const double speed, const double acceleration, const int hp_max,
                                  const int atk, const int def, const double fire_rate, const Uint8 invert,
                                  const Uint8 weapon_block, const Uint8 move_block, const Uint32 duration);
extern void free_spaceship_enchantment(powerup *enchant);
extern void free_all_spaceship_enchants(void);
extern void handle_spaceship_enchantments(void);
extern void extend_spaceship_enchantments_timestamp(const Uint32 time);

#endif