#include "spaceship_enchantments.h"

static powerup *first_enchantment = NULL;
static powerup *last_enchantment = NULL;

powerup *spaceship_enchant(const double speed, const double acceleration, const int hp_max,
                           const int atk, const int def, const double fire_rate, const Uint8 invert,
                           const Uint8 weapon_block, const Uint8 move_block, const Uint32 duration)
{
  /* allocate memory and set new enchantment to ship.last enchantment */
  if(!first_enchantment)
  {
    first_enchantment = (powerup *)malloc(sizeof(powerup));
    if(!first_enchantment)
      return NULL;
    
    first_enchantment->next = NULL;
    first_enchantment->prev = NULL;
    last_enchantment = first_enchantment;
  }
  else
  {
    last_enchantment->next = (powerup *)malloc(sizeof(powerup));
    if(!last_enchantment->next)
      return NULL;
    
    last_enchantment->next->prev = last_enchantment;
    last_enchantment->next->next = NULL;
    last_enchantment = last_enchantment->next;
  }
        
  /* set enchantment properties */
  last_enchantment->speed = speed;
  last_enchantment->acceleration = acceleration;
  last_enchantment->hp_max = hp_max;
  last_enchantment->atk = atk;
  last_enchantment->def = def;
  last_enchantment->fire_rate = fire_rate;
  
  last_enchantment->invert = invert;
  last_enchantment->weapon_block = weapon_block;
  last_enchantment->move_block = move_block;
  
  /* set ship properties */
  spaceship *ship = get_spaceship();
  ship->speed += speed;
  ship->acceleration += acceleration;
  ship->hp_max += hp_max;
  ship->atk += atk;
  ship->def += def;
  ship->fire_rate += fire_rate;
  
  ship->invert += invert;
  ship->weapon_block += weapon_block;
  ship->move_block += move_block;
  
  last_enchantment->start_time = SDL_GetTicks();
  last_enchantment->duration = duration;
  
  return last_enchantment;
}
void free_spaceship_enchantment(powerup *enchant)
{
  if(!enchant)
    return;
  
  spaceship *ship = get_spaceship();
  ship->speed -= enchant->speed;
  ship->acceleration -= enchant->acceleration;
  ship->hp_max -= enchant->hp_max;
  ship->atk -= enchant->atk;
  ship->def -= enchant->def;
  
  ship->invert -= enchant->invert;
  ship->weapon_block -= enchant->weapon_block;
  ship->move_block -= enchant->move_block;
  
  ship->fire_rate -= enchant->fire_rate;
  
  if(enchant == first_enchantment)
  {
    first_enchantment = enchant->next;
    
    if(enchant->next)
      enchant->next->prev = NULL;
    else
      last_enchantment = NULL;
  }
  else if(enchant == last_enchantment)
  {
    enchant->prev->next = NULL;
    last_enchantment = enchant->prev;
  }
  else
  {
    enchant->next->prev = enchant->prev;
    enchant->prev->next = enchant->next;
  }
  
  free(enchant);
}
void free_all_spaceship_enchants(void)
{
  while(first_enchantment)
    free_spaceship_enchantment(first_enchantment);
}
void handle_spaceship_enchantments(void)
{
  powerup *tmp_enchantment = first_enchantment;
  powerup *cache_enchantment = NULL;
  
  while(tmp_enchantment)
  {
    /* remove timed out enchantment */
    if(SDL_GetTicks() - tmp_enchantment->start_time > tmp_enchantment->duration)
    {
      cache_enchantment = tmp_enchantment;
      tmp_enchantment = tmp_enchantment->next;
      free_spaceship_enchantment(cache_enchantment);
      continue;
    }
    
    tmp_enchantment = tmp_enchantment->next;
  }
}
void extend_spaceship_enchantments_timestamp(const Uint32 time)
{
  powerup *tmp_enchantment = NULL;
  
  for(tmp_enchantment = first_enchantment; tmp_enchantment; tmp_enchantment = tmp_enchantment->next)
  {
    tmp_enchantment->start_time += time;
    tmp_enchantment->last_move += time;
  }
}