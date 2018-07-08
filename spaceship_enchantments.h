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