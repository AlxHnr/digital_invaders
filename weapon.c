#include "weapon.h"

static weapon wpns[WEAPON_TYPE_MAX];

#define __WEAPON__(__name__, __blt_type__, __blt_count__, __splitter__, __atk__, __fire_rate__, __scatter_angle__, __bullet_speed__); \
        wpns[__name__].blt_type = __blt_type__; \
        wpns[__name__].bullets = __blt_count__; \
        wpns[__name__].splitter = __splitter__; \
        wpns[__name__].atk = __atk__; \
        wpns[__name__].fire_rate = __fire_rate__; \
        wpns[__name__].scatter_angle = __scatter_angle__; \
        wpns[__name__].bullet_speed = __bullet_speed__;

int init_weapons(void)
{
  /* low grade weapons */
  __WEAPON__(needle_gun, minus_black, 1, 0, 16, 2.5, 0.0, 0.45);
  __WEAPON__(dot_gun, dot_black, 1, 0, 20, 2.0, 0.0, 0.45);
  __WEAPON__(blue_dot_gun, dot_blue, 1, 0, 15, 2.65, 0.0, 0.5);
  __WEAPON__(blue_laser, minus_blue, 1, 0, 13, 3.1, 0.0, 0.5);
  
  __WEAPON__(red_laser, minus_red, 1, 0, 20, 2.5, 0.0, 0.5);
  
  __WEAPON__(BOSS_needle_blaster, minus_black, 25, 0, 20, 3.0, 180.0, 0.5);
  __WEAPON__(BOSS_red_laser_thrower, ball_red, 15, 0, 65, 1.0, 180.0, 0.45);
  
  /* mid grade weapons */
  __WEAPON__(needle_thrower, minus_black, 3, 0, 25, 3.0, 5.0, 0.5);
  __WEAPON__(dot_thrower, dot_black, 3, 0, 40, 2.0, 5.0, 0.5);
  __WEAPON__(blue_dot_shooter, dot_blue, 3, 0, 20, 2.0, 0.0, 0.5);
  __WEAPON__(blue_double_laser, minus_blue, 2, 0, 30, 3.5, 0.0, 0.5);
  
  __WEAPON__(red_double_laser, minus_red, 2, 0, 45, 2.5, 0.0, 0.5);
  
  __WEAPON__(BOSS_dark_blaster, ball_black, 9, 0, 145, 1.75, 0.0, 0.5);
  __WEAPON__(BOSS_red_plus_thrower, plus_red, 25, 0, 95, 1.25, 250.0, 0.5);
  
  /* high grade weapons */
  __WEAPON__(blue_tripple_laser, minus_blue, 3, 0, 40, 3.95, 0.0, 0.55);
  __WEAPON__(blue_energy_gun, ball_blue, 1, 0, 250, 2.25, 0.0, 0.5);
  __WEAPON__(blue_needle_thrower, minus_blue, 6, 0, 43, 2.25, 15.0, 0.5);
  __WEAPON__(e_gun, e_blue, 1, 0, 165, 3.25, 0.0, 0.5);
  
  __WEAPON__(red_plus_shooter, plus_red, 1, 0, 85, 3.0, 0.0, 0.5);
  __WEAPON__(red_energy_gun, ball_red, 1, 0, 150, 2.25, 0.0, 0.5);
  
  __WEAPON__(BOSS_plus_blaster, plus_big_brown, 4, 0, 150, 1.5, 0, 0.5);
  __WEAPON__(BOSS_dark_cannon, ball_black, 12, 0, 100, 1.5, 0, 0.5);
  __WEAPON__(BOSS_minus_thrower, minus_big_brown, 15, 0, 80, 1.85, 180, 0.45);
  
  /* superior grade weapon */
  __WEAPON__(binary_gun, blue_one, 1, 0, 75, 12.0, 0.0, 0.4);
  __WEAPON__(blue_quadro_laser, minus_blue, 4, 0, 65, 5.0, 0.0, 0.585);
  __WEAPON__(blue_energy_blaster, ball_blue, 3, 0, 325, 3.0, 12.0, 0.5);
  __WEAPON__(dark_energy_blaster, ball_black, 1, 0, 1230, 2.0, 0.0, 0.45);
  __WEAPON__(heavy_minus_blaster, minus_big_brown, 1, 0, 890, 2.75, 0.0, 0.45);
  
  __WEAPON__(red_binary_gun, red_one, 1, 0, 72, 12.0, 0.0, 0.4);
  __WEAPON__(red_plus_blaster, plus_red, 3, 0, 115, 3.0, 25.0, 0.5);
  __WEAPON__(red_energy_blaster, ball_red, 3, 0, 100, 3.0, 12.0, 0.5);
  __WEAPON__(blue_blaster, ball_blue, 4, 0, 90, 2.65, 35.0, 0.5);
  __WEAPON__(dark_blaster, ball_black, 4, 0, 120, 2.0, 17.0, 0.5);
  
  __WEAPON__(BOSS_z_cannon, bullet_z, 5, 0, 150, 20.0, 7.0, 0.65);
  __WEAPON__(BOSS_energy_blaster, ball_black, 5, 0, 120, 5.0, 15.0, 0.5);
  __WEAPON__(BOSS_plus_thrower, plus_big_brown, 15, 0, 80, 4.0, 360.0, 0.475);
  
  /* ultra weapons */
  __WEAPON__(ultimate_blue_laser, minus_blue, 5, 0, 225, 9.85, 0.0, 0.75);
  __WEAPON__(ultraheavy_plus_blaster, plus_big_brown, 1, 0, 3840, 2.75, 0.0, 0.5);
  __WEAPON__(ultraheavy_minus_blaster, minus_big_brown, 2, 0, 940, 5.0, 0.0, 0.5);
  __WEAPON__(greycode_deathblaster, greycode, 3, 0, 1185, 2.85, 15.0, 0.5);
  __WEAPON__(venn_shockwaver, venn_elipse, 10, 0, 215, 4.0, 35.0, 0.45);
  
  return 1;
}

void weapon_shot(const bullet_team team, const weapon_type type, enemy *shot_by, Uint32 *last_shot,
                 const double fire_rate, const double x, const double y, const int dir_x, const int atk)
{
  if((fire_rate * wpns[type].fire_rate <= 0.0) || 
     (SDL_GetTicks() - *last_shot < (Uint32)(1000.0/(fire_rate * wpns[type].fire_rate))))
    return;
  
  int counter;
  
  for(counter = 0; counter < wpns[type].bullets; counter++)
  {
    if(type == binary_gun)
    {
      new_bullet(team, blue_one + rand() % 2, shot_by, x, y - get_bullet_h(wpns[type].blt_type)/2, dir_x,
                   0, wpns[type].splitter, wpns[type].bullet_speed, wpns[type].atk + atk);
    }
    else if(type == red_binary_gun)
    {
      new_bullet(team, red_one + rand() % 2, shot_by, x, y - get_bullet_h(wpns[type].blt_type)/2, dir_x,
                   0, wpns[type].splitter, wpns[type].bullet_speed, wpns[type].atk + atk);
    }
    else if(wpns[type].bullets > 1)
    {
      if(wpns[type].scatter_angle <= 0.0)
        new_bullet(team, wpns[type].blt_type, shot_by, x,
                   y  - (wpns[type].bullets/2 * get_bullet_h(wpns[type].blt_type)) + counter * get_bullet_h(wpns[type].blt_type),
                   dir_x, 0, wpns[type].splitter, wpns[type].bullet_speed, wpns[type].atk + atk);
      else
        new_bullet(team, wpns[type].blt_type, shot_by, x, y - get_bullet_h(wpns[type].blt_type)/2,
                   dir_x * sin(((90.0 + wpns[type].scatter_angle/2) - (wpns[type].scatter_angle/wpns[type].bullets * (counter + 0.5))) * M_PI/180.0),
                   cos(((90.0 + wpns[type].scatter_angle/2) - (wpns[type].scatter_angle/wpns[type].bullets * (counter + 0.5))) * M_PI/180.0),
                   wpns[type].splitter, wpns[type].bullet_speed, wpns[type].atk + atk);
    }
    else
      new_bullet(team, wpns[type].blt_type, shot_by, x, y - get_bullet_h(wpns[type].blt_type)/2, dir_x,
                 0, wpns[type].splitter, wpns[type].bullet_speed, wpns[type].atk + atk);
  }
  
  *last_shot = SDL_GetTicks();
}