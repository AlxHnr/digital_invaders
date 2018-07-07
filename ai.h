#ifndef AI_H
#define AI_H

#include <SDL.h>

#include "enemy.h"
#include "spaceship.h"

extern void AI_move_to_ship_and_shot(enemy *this);
extern void AI_move_fast_to_ship_and_shot(enemy *this);
extern void AI_move_to_ship_and_dodge(enemy *this);
extern void AI_move_into_screen_and_shot(enemy *this);
extern void AI_move_into_screen_and_dodge(enemy *this);
extern void AI_move_into_screen_and_dodge_quick(enemy *this);

#endif