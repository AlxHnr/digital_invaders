#include "collision.h"

#define __SET_TEXT__(__WEAPON__, __TEXT__) \
        if(tmp_ship->wpn_type == __WEAPON__){ text = __TEXT__;}

void handle_collision(void)
{
  enemy *tmp_enemy = NULL;
  spaceship *tmp_ship = get_spaceship();
  
  /* check all bullet-related collisions */
  bullet *tmp_bullet = get_first_bullet();
  bullet *cache_bullet = NULL;
  while(tmp_bullet)
  {
    /* check collision between ship and bullet */
    if(tmp_bullet->team == enemy_team &&
       tmp_bullet->x + get_bullet_w(tmp_bullet->type) > tmp_ship->x &&
       tmp_bullet->x < tmp_ship->x + get_spaceship_w() &&
       tmp_bullet->y + get_bullet_h(tmp_bullet->type) > tmp_ship->y &&
       tmp_bullet->y < tmp_ship->y + get_spaceship_h())
    {
      if(tmp_bullet->power - tmp_ship->def > 0)
      {
        tmp_ship->hp -= tmp_bullet->power - tmp_ship->def;
        new_scrolling_int(tmp_ship->x + get_spaceship_w()/2, tmp_ship->y,
                          tmp_bullet->power - tmp_ship->def, COLOR_ENEMY_DAMAGE);
      }
      else
      {
        new_scrolling_int(tmp_ship->x + get_spaceship_w()/2, tmp_ship->y, 0, COLOR_ENEMY_DAMAGE);
      }
      
      cache_bullet = tmp_bullet;
      tmp_bullet = tmp_bullet->next;
      free_bullet(cache_bullet);
      continue;
    }
    
    /* check each enemy for collision with bullet */
    tmp_enemy = get_first_enemy();
    while(tmp_bullet->team == player_team && tmp_enemy)
    {
      if(tmp_bullet->shot_by != tmp_enemy &&
         tmp_ship->x + get_spaceship_w() < tmp_enemy->x &&
         tmp_bullet->x + get_bullet_w(tmp_bullet->type) > tmp_enemy->x &&
         tmp_bullet->x < tmp_enemy->x + get_enemy_w(tmp_enemy->level) &&
         tmp_bullet->y + get_bullet_h(tmp_bullet->type) > tmp_enemy->y &&
         tmp_bullet->y < tmp_enemy->y + get_enemy_h(tmp_enemy->level))
      {
        if(tmp_bullet->power - tmp_enemy->def > 0)
        {
          tmp_enemy->hp -= tmp_bullet->power - tmp_enemy->def;
          new_scrolling_int(tmp_enemy->x + get_enemy_w(tmp_enemy->level)/2, tmp_enemy->y,
                            tmp_bullet->power - tmp_enemy->def, COLOR_SHIP_DAMAGE);
        }
        else
        {
          new_scrolling_int(tmp_enemy->x + get_enemy_w(tmp_enemy->level)/2, tmp_enemy->y, 0, COLOR_SHIP_DAMAGE);
        }
        
        if(tmp_bullet->splitter)
          splitter_bullet(tmp_bullet, tmp_enemy, tmp_enemy->x, tmp_enemy->y, get_enemy_w(tmp_enemy->level),
                          get_enemy_h(tmp_enemy->level), tmp_bullet->splitter);
        
        if(tmp_enemy->hp <= 0)
        {
          tmp_ship->points += tmp_enemy->points;
          free_enemy(tmp_enemy);
          tmp_enemy = NULL;
        }
        
        cache_bullet = tmp_bullet;
        tmp_bullet = tmp_bullet->next;
        free_bullet(cache_bullet);
        break;
      }
      
      tmp_enemy = tmp_enemy->next;
    }
    
    if(tmp_bullet)
      tmp_bullet = tmp_bullet->next;
  }
  
  /* power up related collisions */
  powerup *tmp_powerup = get_first_powerup();
  powerup *cache_powerup = NULL;
  char *text = "Unknown Weapon";
  while(tmp_powerup)
  {
    /* check collision between powerup and ship */
    if(tmp_powerup->x + get_powerup_w(tmp_powerup->icon) > tmp_ship->x &&
       tmp_powerup->x < tmp_ship->x + get_spaceship_w() &&
       tmp_powerup->y + get_powerup_h(tmp_powerup->icon) > tmp_ship->y &&
       tmp_powerup->y < tmp_ship->y + get_spaceship_h())
    {
      if(tmp_powerup->type == enchantment)
      {
        if(tmp_powerup->icon == icon_inhibition)
        {
          spaceship_enchant(0, 0, 0, 0, 1000000, 0, 0, 0, 0, 10000);
          new_scrolling_text(tmp_ship->x + get_spaceship_w()/2, tmp_ship->y + get_spaceship_h()/2, "Shield", COLOR_UPGRADE);
        }
        else if(tmp_powerup->icon == icon_problem)
        {
          spaceship_enchant(0, 0, 0, 0, 0, 0, 0, 0, 1, 5000);
          new_scrolling_text(tmp_ship->x + get_spaceship_w()/2, tmp_ship->y + get_spaceship_h()/2, "Problem", COLOR_DOWNGRADE);
        }
        else if(tmp_powerup->icon == icon_inverted)
        {
          spaceship_enchant(0, 0, 0, 0, 0, 0, 1, 0, 0, 5000);
          new_scrolling_text(tmp_ship->x + get_spaceship_w()/2, tmp_ship->y + get_spaceship_h()/2, "Invert", COLOR_DOWNGRADE);
        }
        else if(tmp_powerup->icon == icon_idempotency)
        {
          spaceship_enchant(0, 0, 0, 0, 0, 0, 0, 1, 0, 5000);
          new_scrolling_text(tmp_ship->x + get_spaceship_w()/2, tmp_ship->y + get_spaceship_h()/2, "Shutdown", COLOR_DOWNGRADE);
        }
        else if(tmp_powerup->icon == icon_add)
        {
          spaceship_enchant(0, 0, 0, 0, 0, 0, 0, 1, 1, 5000);
          new_scrolling_text(tmp_ship->x + get_spaceship_w()/2, tmp_ship->y + get_spaceship_h()/2, "Stun", COLOR_DOWNGRADE);
        }
      }
      else if(tmp_powerup->type == status_incrase)
      {
        if(tmp_powerup->icon == icon_carry)
        {
          tmp_ship->hp += tmp_ship->hp_max * 0.20;
          new_scrolling_text(tmp_ship->x + get_spaceship_w()/2, tmp_ship->y + get_spaceship_h()/2, "HP Up", COLOR_UPGRADE);
        }
        else if(tmp_powerup->icon == icon_borrow)
        {
          tmp_ship->hp += 100;
          tmp_ship->hp_max += 100;
          new_scrolling_text(tmp_ship->x + get_spaceship_w()/2, tmp_ship->y + get_spaceship_h()/2, "Max HP Up", COLOR_UPGRADE);
        }
        else if(tmp_powerup->icon == icon_or)
        {
          tmp_ship->def += 7;
          new_scrolling_text(tmp_ship->x + get_spaceship_w()/2, tmp_ship->y + get_spaceship_h()/2, "Def Up", COLOR_UPGRADE);
        }
        else if(tmp_powerup->icon == icon_function)
        {
          tmp_ship->atk += 10;
          new_scrolling_text(tmp_ship->x + get_spaceship_w()/2, tmp_ship->y + get_spaceship_h()/2, "Atk Up", COLOR_UPGRADE);
        }
        else if(tmp_powerup->icon == icon_variable)
        {
          tmp_ship->points += 500;
          new_scrolling_text(tmp_ship->x + get_spaceship_w()/2, tmp_ship->y + get_spaceship_h()/2, "Bonus Points", COLOR_UPGRADE);
        }
        else if(tmp_powerup->icon == icon_complement)
        {
          tmp_ship->hp += tmp_ship->hp_max/2;
          tmp_ship->hp_max += 125;
          tmp_ship->def += 7;
          tmp_ship->atk += 10;
          new_scrolling_text(tmp_ship->x + get_spaceship_w()/2, tmp_ship->y + get_spaceship_h()/2, "Level Up", COLOR_UPGRADE);
        }
        else if(tmp_powerup->icon == icon_ansi)
        {
          tmp_ship->hp = tmp_ship->hp_max;
          new_scrolling_text(tmp_ship->x + get_spaceship_w()/2, tmp_ship->y + get_spaceship_h()/2, "Full HP", COLOR_UPGRADE);
        }
        else if(tmp_powerup->icon == icon_subtraction)
        {
          tmp_ship->hp -= 100;
          tmp_ship->hp_max -= 100;
          tmp_ship->def -= 3;
          tmp_ship->atk -= 6;
          new_scrolling_text(tmp_ship->x + get_spaceship_w()/2, tmp_ship->y + get_spaceship_h()/2, "Level down", COLOR_DOWNGRADE);
        }
        else if(tmp_powerup->icon == icon_7)
        {
          tmp_ship->def -= 3;
          tmp_ship->atk -= 6;
          new_scrolling_text(tmp_ship->x + get_spaceship_w()/2, tmp_ship->y + get_spaceship_h()/2, "Stats down", COLOR_DOWNGRADE);
        }
      }
      else if(tmp_powerup->type == weapon_change)
      {
        /* set random weapon */
        if(tmp_powerup->icon == icon_weapon_grade_0)
          tmp_ship->wpn_type = needle_gun + rand() % LOW_GRADE_WEAPONS;
        else if(tmp_powerup->icon == icon_weapon_grade_1)
          tmp_ship->wpn_type = needle_thrower + rand() % MID_GRADE_WEAPONS;
        else if(tmp_powerup->icon == icon_weapon_grade_2)
          tmp_ship->wpn_type = blue_tripple_laser + rand() % HIGH_GRADE_WEAPONS;
        else if(tmp_powerup->icon == icon_weapon_grade_3)
          tmp_ship->wpn_type = binary_gun + rand() % SUPERIOR_GRADE_WEAPONS;
        else if(tmp_powerup->icon == icon_weapon_grade_4)
          tmp_ship->wpn_type = ultimate_blue_laser + rand() % ULTRA_GRADE_WEAPONS;
        
        /* scroll weapon name */
        __SET_TEXT__(needle_gun, "Needle Gun")
        else __SET_TEXT__(dot_gun, "Dot Gun")
        else __SET_TEXT__(blue_dot_gun, "Blue Dot Gun")
        else __SET_TEXT__(blue_laser, "Blue Laser")
        
        else __SET_TEXT__(needle_thrower, "Needle Thrower")
        else __SET_TEXT__(dot_thrower, "Dot Thrower")
        else __SET_TEXT__(blue_dot_shooter, "Blue Dot Shooter")
        else __SET_TEXT__(blue_double_laser, "Blue double Laser")
        
        else __SET_TEXT__(blue_tripple_laser, "Blue tripple Laser")
        else __SET_TEXT__(blue_energy_gun, "Blue Energy Gun")
        else __SET_TEXT__(blue_needle_thrower, "Blue Needle Thrower")
        else __SET_TEXT__(e_gun, "E Gun")
        
        else __SET_TEXT__(binary_gun, "Binary Gun")
        else __SET_TEXT__(blue_quadro_laser, "Blue qadro Laser")
        else __SET_TEXT__(blue_energy_blaster, "Blue Energy Blaster")
        else __SET_TEXT__(dark_energy_blaster, "Dark Energy Blaster")
        else __SET_TEXT__(heavy_minus_blaster, "Heavy Minus Blaster")
        
        else __SET_TEXT__(ultimate_blue_laser, "Ultimate blue Laser")
        else __SET_TEXT__(ultraheavy_minus_blaster, "Ultraheavy Minus Blaster")
        else __SET_TEXT__(ultraheavy_plus_blaster, "Ultraheavy Plus Blaster")
        else __SET_TEXT__(greycode_deathblaster, "Greycode Deathblaster")
        else __SET_TEXT__(venn_shockwaver, "Venn Shockwaver")
        
        new_scrolling_text(tmp_ship->x + get_spaceship_w()/2, tmp_ship->y + get_spaceship_h()/2, text, COLOR_WEAPON);
      }
      
      /* free powerup */
      cache_powerup = tmp_powerup;
      tmp_powerup = tmp_powerup->next;
      free_powerup(cache_powerup);
      continue;
    }
    
    tmp_powerup = tmp_powerup->next;
  }
}