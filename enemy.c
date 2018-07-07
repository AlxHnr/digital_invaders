#include "enemy.h"

static enemy *first_enemy = NULL;
static enemy *last_enemy = NULL;

static SDL_Surface *enemy_sprite[ENEMY_LEVEL_MAX];

static Uint32 enemy_timestop = 0;

static void enemy_exit(void)
{
  int counter;
  
  for(counter = 0; counter < ENEMY_LEVEL_MAX; counter++)
    SDL_FreeSurface(enemy_sprite[counter]);
  
  free_all_enemys();
}
int init_enemys(void)
{
  int counter;
  char tmp_string[512] = {0};
  
  for(counter = 0; counter < ENEMY_LEVEL_MAX; counter++)
  {
    sprintf(tmp_string, "gfx/enemy/%i.png", counter);
    if(!(enemy_sprite[counter] = load_image(tmp_string)))
      return 0;
  }
  
  enemy_timestop = SDL_GetTicks();
  
  atexit(enemy_exit);
  return 1;
}

enemy *new_enemy(const int level, const unsigned int points, void (*ai_function)(enemy *),
                 const weapon_type wpn_type, const double x, const double y,
                 const double speed, const int hp_max, const int atk, const int def, const double fire_rate)
{
  /* allocate memory and set new enemy to last enemy */
  if(!first_enemy)
  {
    first_enemy = (enemy *)malloc(sizeof(enemy));
    if(!first_enemy)
      return NULL;
    
    first_enemy->next = NULL;
    first_enemy->prev = NULL;
    last_enemy = first_enemy;
  }
  else
  {
    last_enemy->next = (enemy *)malloc(sizeof(enemy));
    if(!last_enemy->next)
      return NULL;
    
    last_enemy->next->prev = last_enemy;
    last_enemy->next->next = NULL;
    last_enemy = last_enemy->next;
  }
  
  /* set enemy properties */
  last_enemy->level = level;
  last_enemy->ai_function = ai_function;
  last_enemy->ai_timestop = SDL_GetTicks();
  last_enemy->wpn_type = wpn_type;
  
  last_enemy->x = x;
  last_enemy->y = y;
  last_enemy->dir_x = -1;
  last_enemy->dir_y = 0;
  last_enemy->speed = speed;
  last_enemy->next_dodge = 1;
  
  last_enemy->points = points;
  
  last_enemy->hp = hp_max;
  last_enemy->hp_max = hp_max;
  
  last_enemy->atk = atk;
  last_enemy->def = def;
  last_enemy->fire_rate = fire_rate;
  
  last_enemy->last_move = SDL_GetTicks();
  last_enemy->last_shot = 0;
  
  return last_enemy;
}
void free_enemy(enemy *object)
{
  if(!object)
    return;
  
  if(object == first_enemy)
  {
    first_enemy = object->next;
    
    if(object->next)
      object->next->prev = NULL;
    else
      last_enemy = NULL;
  }
  else if(object == last_enemy)
  {
    object->prev->next = NULL;
    last_enemy = object->prev;
  }
  else
  {
    object->next->prev = object->prev;
    object->prev->next = object->next;
  }
  
  free(object);
}
enemy *get_first_enemy(void)
{
  return first_enemy;
}
int get_enemy_w(const int level)
{
  return enemy_sprite[level]->w;
}
int get_enemy_h(const int level)
{
  return enemy_sprite[level]->h;
}

void handle_all_enemys(void)
{
  enemy *tmp_enemy = first_enemy;
  enemy *cache_enemy = NULL;
  
  while(tmp_enemy)
  {
    tmp_enemy->ai_function(tmp_enemy);
    
    /* free enemy if its moved out of window */
    if((tmp_enemy->x > SDL_GetVideoSurface()->w) ||
      (tmp_enemy->x < 0 - get_enemy_w(tmp_enemy->level)) ||
      (tmp_enemy->y > SDL_GetVideoSurface()->h) ||
      (tmp_enemy->y < 0 - get_enemy_h(tmp_enemy->level)))
    {
      cache_enemy = tmp_enemy;
      tmp_enemy = tmp_enemy->next;
      free_enemy(cache_enemy);
      continue;
    }
    
    tmp_enemy = tmp_enemy->next;
  }
}
void blit_all_enemys(SDL_Surface *dst)
{
  enemy *tmp_enemy = NULL;
  SDL_Rect tmp_rect;
  
  for(tmp_enemy = first_enemy; tmp_enemy; tmp_enemy = tmp_enemy->next)
  {
    tmp_rect.x = tmp_enemy->x;
    tmp_rect.y = tmp_enemy->y;
    
    SDL_BlitSurface(enemy_sprite[tmp_enemy->level], NULL, dst, &tmp_rect);
  }
}
void extend_all_enemys_timestamp(const Uint32 time)
{
  enemy *tmp_enemy = NULL;
  
  for(tmp_enemy = first_enemy; tmp_enemy; tmp_enemy = tmp_enemy->next)
  {
    tmp_enemy->last_move += time;
    tmp_enemy->last_shot += time;
    tmp_enemy->ai_timestop += time;
  }
}
void free_all_enemys(void)
{
  while(first_enemy)
    free_enemy(first_enemy);
}

void enemy_set_dir_to_ship(enemy *this)
{
  spaceship *ship = get_spaceship();
  double tmp_x, tmp_y;
  
  /* get absolute difference */
  if(this->x < ship->x)
    tmp_x = ship->x - this->x;
  else
    tmp_x = this->x - ship->x;
  
  if(this->y + get_enemy_h(this->level)/2 < ship->y + get_spaceship_h()/2)
    tmp_y = (ship->y + get_spaceship_h()/2) - (this->y + get_enemy_h(this->level)/2);
  else
    tmp_y = (this->y + get_enemy_h(this->level)/2) - (ship->y + get_spaceship_h()/2);
  
  /* get gradient */
  this->dir_y = tmp_y/tmp_x;
  
  /* correct gradient if its too high */
  if(this->dir_y > 1.75)
    this->dir_y = 1;
  
  if(this->y > ship->y)
    this->dir_y *= -1;
}
void enemy_dodge(enemy *this)
{
  bullet *tmp_bullet = NULL;
  
  for(tmp_bullet = get_first_bullet(); tmp_bullet; tmp_bullet = tmp_bullet->next)
  {
    /* return if bullet is not shot from player or bullet is behind 'this' */
    if((tmp_bullet->team != player_team) ||
       (tmp_bullet->x > this->x + get_enemy_w(this->level)))
      continue;
    
    /* check if bullet is flying towards 'this' */
    if((tmp_bullet->y + (this->x - tmp_bullet->x) * tmp_bullet->dir_y) + get_bullet_h(tmp_bullet->type) > this->y &&
       (tmp_bullet->y + (this->x - tmp_bullet->x) * tmp_bullet->dir_y) < this->y + get_enemy_h(this->level))
    {
      if(tmp_bullet->y + get_bullet_h(tmp_bullet->type)/2 < SDL_GetVideoSurface()->h/3)
        this->next_dodge = 1;
      else if(tmp_bullet->y + get_bullet_h(tmp_bullet->type)/2 > SDL_GetVideoSurface()->h * 0.75)
        this->next_dodge = -1;
      
      this->dir_y = this->next_dodge;
    }
  }
}
void enemy_move(enemy *this)
{
  /* move */
  this->x += this->dir_x * (SDL_GetTicks() - this->last_move) * this->speed;
  this->y += this->dir_y * (SDL_GetTicks() - this->last_move) * this->speed;
  
  this->last_move = SDL_GetTicks();
}
void enemy_shot(enemy *this)
{
  weapon_shot(enemy_team, this->wpn_type, this, &this->last_shot, this->fire_rate,
              this->x, this->y + get_enemy_h(this->level)/2, -1, this->atk);
}

enemy *spawn_enemy(const int lvl)
{
  if((lvl < 0) || (lvl >= ENEMY_LEVEL_MAX))
    return NULL;
  
  switch(lvl)
  {
    /* level | points | ai | weapon | x | y | speed | hp_max | atk | def | firerate */
    case 0:
      return new_enemy(lvl, 25, AI_move_to_ship_and_shot, needle_gun, SDL_GetVideoSurface()->w,
                rand() % (SDL_GetVideoSurface()->h - get_enemy_h(lvl)), 0.045, 55, 15, 4, 0.1);
    case 1:
      return new_enemy(lvl, 45, AI_move_to_ship_and_shot, needle_gun, SDL_GetVideoSurface()->w,
                rand() % (SDL_GetVideoSurface()->h - get_enemy_h(lvl)), 0.065, 75, 25, 10, 0.25);
    case 2:
      return new_enemy(lvl, 135, AI_move_into_screen_and_dodge, needle_thrower, SDL_GetVideoSurface()->w,
                rand() % (SDL_GetVideoSurface()->h - get_enemy_h(lvl)), 0.08, 385, 20, 35, 0.25);
    case 3:
      return new_enemy(lvl, 100, AI_move_to_ship_and_shot, red_laser, SDL_GetVideoSurface()->w,
                rand() % (SDL_GetVideoSurface()->h - get_enemy_h(lvl)), 0.065, 255, 75, 30, 0.75);
    case 4:
      return new_enemy(lvl, 3250, AI_move_into_screen_and_dodge, BOSS_red_laser_thrower, SDL_GetVideoSurface()->w,
                SDL_GetVideoSurface()->h/2 - get_enemy_h(lvl)/2, 0.045, 1985, 85, 45, 0.25);
    case 5:
      return new_enemy(lvl, 150, AI_move_to_ship_and_dodge, red_double_laser, SDL_GetVideoSurface()->w,
                rand() % (SDL_GetVideoSurface()->h - get_enemy_h(lvl)), 0.05, 950, 45, 40, 0.185);
    case 6:
      return new_enemy(lvl, 185, AI_move_to_ship_and_dodge, red_plus_shooter, SDL_GetVideoSurface()->w,
                rand() % (SDL_GetVideoSurface()->h - get_enemy_h(lvl)), 0.05, 1350, 50, 30, 0.2);
    case 7:
      return new_enemy(lvl, 265, AI_move_into_screen_and_dodge_quick, red_laser, SDL_GetVideoSurface()->w,
                rand() % (SDL_GetVideoSurface()->h - get_enemy_h(lvl)), 0.05, 2250, 95, 35, 0.3);
    case 8:
      return new_enemy(lvl, 5965, AI_move_into_screen_and_dodge_quick, BOSS_red_plus_thrower, SDL_GetVideoSurface()->w,
                SDL_GetVideoSurface()->h/2 - get_enemy_h(lvl)/2, 0.075, 6250, 125, 60, 0.5);
    case 9:
      return new_enemy(lvl, 565, AI_move_to_ship_and_shot, red_energy_gun, SDL_GetVideoSurface()->w,
                rand() % (SDL_GetVideoSurface()->h - get_enemy_h(lvl)), 0.08, 3200, 50, 80, 0.3);
    case 10:
      return new_enemy(lvl, 795, AI_move_to_ship_and_shot, red_energy_gun, SDL_GetVideoSurface()->w,
                rand() % (SDL_GetVideoSurface()->h - get_enemy_h(lvl)), 0.05, 4500, 120, 70, 0.2);
    case 11:
      return new_enemy(lvl, 1495, AI_move_into_screen_and_dodge_quick, red_plus_blaster, SDL_GetVideoSurface()->w,
                rand() % (SDL_GetVideoSurface()->h - get_enemy_h(lvl)), 0.08, 5400, 50, 95, 0.2);
    case 12:
      return new_enemy(lvl, 1795, AI_move_fast_to_ship_and_shot, red_binary_gun, SDL_GetVideoSurface()->w,
                rand() % (SDL_GetVideoSurface()->h - get_enemy_h(lvl)), 0.05, 3000, 80, 30, 0.8);
    case 13:
      return new_enemy(lvl, 58000, AI_move_into_screen_and_dodge, BOSS_dark_cannon, SDL_GetVideoSurface()->w,
                SDL_GetVideoSurface()->h/2 - get_enemy_h(lvl)/2, 0.05, 75000, 120, 100, 0.5);
    case 14:
      return new_enemy(lvl, 5200, AI_move_into_screen_and_shot, red_energy_blaster, SDL_GetVideoSurface()->w,
                rand() % (SDL_GetVideoSurface()->h - get_enemy_h(lvl)), 0.08, 20000, 100, 80, 0.2);
    case 15:
      return new_enemy(lvl, 6700, AI_move_into_screen_and_dodge_quick, red_plus_blaster, SDL_GetVideoSurface()->w,
                rand() % (SDL_GetVideoSurface()->h - get_enemy_h(lvl)), 0.09, 38000, 60, 80, 0.2);
    case 16:
      return new_enemy(lvl, 9700, AI_move_to_ship_and_shot, blue_blaster, SDL_GetVideoSurface()->w,
                rand() % (SDL_GetVideoSurface()->h - get_enemy_h(lvl)), 0.05, 42500, 100, 130, 0.3);
    case 17:
      return new_enemy(lvl, 18000, AI_move_into_screen_and_shot, dark_blaster, SDL_GetVideoSurface()->w,
                rand() % (SDL_GetVideoSurface()->h - get_enemy_h(lvl)), 0.08, 140000, 130, 150, 0.3);
    case 18:
      return new_enemy(lvl, 897385, AI_move_into_screen_and_shot, BOSS_energy_blaster, SDL_GetVideoSurface()->w,
                SDL_GetVideoSurface()->h/2 - get_enemy_h(lvl)/2, 0.08, 1000000, 180, 290, 0.2);
  }
  
  return NULL;
}