#include "developers_panel.h"

static void hide_window(const g_event *event, g_window *window, void *data)
{
  if(event->type == G_WINDOW_CLOSE)
  {
    window->flags.visible = 0;
  }
}
static void show_window(const g_event *event, g_widget *widget, void *data)
{
  if(event->type == G_CLICK_LEFT)
  {
    g_window *window = (g_window *)data;
    g_toggle(window->flags.visible);
    g_raise_window(window);
  }
}

static void button_spawn_weapon(const g_event *event, g_widget *widget, void *data)
{
  if(event->type == G_CLICK_LEFT)
    new_powerup_weapon((powerup_icon)data);
}
static void button_spawn_enchantment(const g_event *event, g_widget *widget, void *data)
{
  if(event->type == G_CLICK_LEFT)
    new_powerup_enchantment((powerup_icon)data);
}
static void button_spawn_status_increase(const g_event *event, g_widget *widget, void *data)
{
  if(event->type == G_CLICK_LEFT)
    new_powerup_status_increase((powerup_icon)data);
}

static void slider_ship_speed(const g_event *event, g_widget *widget, void *data)
{
  g_widget *text = (g_widget *)data;
  spaceship *ship = get_spaceship();
  
  ship->speed = widget->slider.value;
  sprintf(text->text.text, "%.3lf", widget->slider.value);
}
static void slider_ship_acceleration(const g_event *event, g_widget *widget, void *data)
{
  g_widget *text = (g_widget *)data;
  spaceship *ship = get_spaceship();
  
  ship->acceleration = widget->slider.value;
  sprintf(text->text.text, "%.5lf", widget->slider.value);
}
static void slider_ship_fire_rate(const g_event *event, g_widget *widget, void *data)
{
  g_widget *text = (g_widget *)data;
  spaceship *ship = get_spaceship();
  
  ship->fire_rate = widget->slider.value;
  sprintf(text->text.text, "%.2lf", widget->slider.value);
}

static void toggle_ship_invert(const g_event *event, g_widget *widget, void *data)
{
  if(event->type == G_CLICK_LEFT)
  {
    spaceship *ship = get_spaceship();
    
    if(widget->check.state)
      ship->invert++;
    else
      ship->invert--;
  }
}
static void toggle_ship_weapon_block(const g_event *event, g_widget *widget, void *data)
{
  if(event->type == G_CLICK_LEFT)
  {
    spaceship *ship = get_spaceship();
    
    if(widget->check.state)
      ship->weapon_block++;
    else
      ship->weapon_block--;
  }
}
static void toggle_ship_move_block(const g_event *event, g_widget *widget, void *data)
{
  if(event->type == G_CLICK_LEFT)
  {
    spaceship *ship = get_spaceship();
    
    if(widget->check.state)
      ship->move_block++;
    else
      ship->move_block--;
  }
}

bool ai_controlled_bullets = false;
static void toggle_ai_bullets(const g_event *event, g_widget *widget, void *data)
{
  if(event->type == G_CLICK_LEFT)
  {
    if(widget->check.state)
      ai_controlled_bullets = true;
    else
      ai_controlled_bullets = false;
  }
}

bool ai_controlled_enemy_bullets = false;
static void toggle_ai_enemy_bullets(const g_event *event, g_widget *widget, void *data)
{
  if(event->type == G_CLICK_LEFT)
  {
    if(widget->check.state)
      ai_controlled_enemy_bullets = true;
    else
      ai_controlled_enemy_bullets = false;
  }
}

bool ai_controlled_powerups = false;
static void toggle_ai_powerups(const g_event *event, g_widget *widget, void *data)
{
  if(event->type == G_CLICK_LEFT)
  {
    if(widget->check.state)
      ai_controlled_powerups = true;
    else
      ai_controlled_powerups = false;
  }
}

bool ai_controlled_weapons = false;
static void toggle_ai_weapons(const g_event *event, g_widget *widget, void *data)
{
  if(event->type == G_CLICK_LEFT)
  {
    if(widget->check.state)
      ai_controlled_weapons = true;
    else
      ai_controlled_weapons = false;
  }
}

bool ai_controlled_debuffs = false;
static void toggle_ai_debuffs(const g_event *event, g_widget *widget, void *data)
{
  if(event->type == G_CLICK_LEFT)
  {
    if(widget->check.state)
      ai_controlled_debuffs = true;
    else
      ai_controlled_debuffs = false;
  }
}

static void enemy_parameter(const g_event *event, g_widget *widget, void *data)
{
  if(event->type == G_KEYDOWN && atoi(widget->input.text) >= ENEMY_LEVEL_MAX)
    sprintf(widget->input.text, "%i", ENEMY_LEVEL_MAX - 1);
}

static void button_call_function(const g_event *event, g_widget *widget, void *data)
{
  if(event->type == G_CLICK_LEFT)
  {
    void (*function)(void) = data;
    function();
  }
}

static void button_spawn_enemy(const g_event *event, g_widget *widget, void *data)
{
  if(event->type == G_CLICK_LEFT)
  {
    g_widget *tmp_widget = (g_widget *)data;
    spawn_enemy(atoi(tmp_widget->input.text));
  }
}

static g_window *create_weapon_window(void)
{
  g_window *window = g_create_window(rand() % 400 + 50, rand() % 250 + 50, 250, 60, "Weapons");
  window->flags.visible = 0;
  window->event_function = hide_window;
  
  g_widget *button = g_attach_button(window, 0, 0, "Grade 1");
  button->event_function = button_spawn_weapon;
  button->event_data = (void *)icon_weapon_grade_0;
  
  button = g_attach_button(window, 90, 0, "Grade 2");
  button->event_function = button_spawn_weapon;
  button->event_data = (void *)icon_weapon_grade_1;
  
  button = g_attach_button(window, 180, 0, "Grade 3");
  button->event_function = button_spawn_weapon;
  button->event_data = (void *)icon_weapon_grade_2;
  
  button = g_attach_button(window, 0, 32, "Grade 4");
  button->event_function = button_spawn_weapon;
  button->event_data = (void *)icon_weapon_grade_3;
  
  button = g_attach_button(window, 90, 32, "Grade 5");
  button->event_function = button_spawn_weapon;
  button->event_data = (void *)icon_weapon_grade_4;
  
  return window;
}
static g_window *create_powerup_window(void)
{
  g_window *window = g_create_window(rand() % 400 + 50, rand() % 250 + 50, 115, 175 , "Powerups");
  window->flags.visible = 0;
  window->event_function = hide_window;
  
  g_widget *button = g_attach_button(window, 0, 0, "   Carry  ");
  button->event_function = button_spawn_status_increase;
  button->event_data = (void *)icon_carry;
  
  button = g_attach_button(window, 0, 0, "  Borrow  ");
  button->event_function = button_spawn_status_increase;
  button->event_data = (void *)icon_borrow;
  
  button = g_attach_button(window, 0, 32, "    Or    ");
  button->event_function = button_spawn_status_increase;
  button->event_data = (void *)icon_or;
  
  button = g_attach_button(window, 0, 55, "   Ansi   ");
  button->event_function = button_spawn_status_increase;
  button->event_data = (void *)icon_ansi;
  
  button = g_attach_button(window, 0, 78, " Function ");
  button->event_function = button_spawn_status_increase;
  button->event_data = (void *)icon_function;
  
  button = g_attach_button(window, 0, 101, "Inhibition");
  button->event_function = button_spawn_enchantment;
  button->event_data = (void *)icon_inhibition;
  
  button = g_attach_button(window, 0, 124, " Variable ");
  button->event_function = button_spawn_status_increase;
  button->event_data = (void *)icon_variable;
  
  button = g_attach_button(window, 0, 147 , "Complement");
  button->event_function = button_spawn_status_increase;
  button->event_data = (void *)icon_complement;
  
  return window;
}
static g_window *create_spaceship_window(void)
{
  spaceship *ship = get_spaceship();
  
  g_window *window = g_create_window(rand() % 400 + 50, rand() % 250 + 50, 250, 220 , "Spaceship Properties");
  window->flags.visible = 0;
  window->event_function = hide_window;
  
  g_widget *widget, *widget_2 = NULL;
  
  g_attach_text(window, 0, 0, 0, 0, "Speed:");
  widget = g_attach_text(window, 150, 0, 0, 0, "");
  sprintf(widget->text.text, "%.3lf", ship->speed);
  widget_2 = g_attach_slider_h(window, 0, 25, 0, ship->speed, SPACESHIP_SPEED_MAX);
  widget_2->event_function = slider_ship_speed;
  widget_2->event_data = (void *)widget;
  
  g_attach_text(window, 0, 45, 0, 0, "Acceleration:");
  widget = g_attach_text(window, 150, 45, 0, 0, "");
  sprintf(widget->text.text, "%.5lf", ship->acceleration);
  widget_2 = g_attach_slider_h(window, 0, 60, 0, ship->acceleration, SPACESHIP_ACCELLERATION_MAX);
  widget_2->event_function = slider_ship_acceleration;
  widget_2->event_data = (void *)widget;
  
  g_attach_text(window, 0, 85, 0, 0, "Firerate:");
  widget = g_attach_text(window, 150, 85, 0, 0, "");
  sprintf(widget->text.text, "%.2lf", ship->fire_rate);
  widget_2 = g_attach_slider_h(window, 0, 100, 0, ship->fire_rate, 20.0);
  widget_2->event_function = slider_ship_fire_rate;
  widget_2->event_data = widget;
  
  g_attach_text(window, 0, 135, 0, 0, "Invert:");
  widget = g_attach_check_box(window, 120, 130);
  widget->event_function = toggle_ship_invert;
  
  g_attach_text(window, 0, 165, 0, 0, "Weapon Block:");
  widget = g_attach_check_box(window, 120, 160);
  widget->event_function = toggle_ship_weapon_block;
  
  g_attach_text(window, 0, 195, 0, 0, "Move Block:");
  widget = g_attach_check_box(window, 120, 190);
  widget->event_function = toggle_ship_move_block;
  
  g_attach_surface(window, 170, 153, 0, 0, get_spaceship_sprite());
  
  return window;
}
static g_window *create_ai_window(void)
{
  g_window *window = g_create_window(rand() % 400 + 50, rand() % 250 + 50, 265, 125, "AI Features");
  window->flags.visible = 0;
  window->event_function = hide_window;
  
  g_widget *widget = NULL;
  
  g_attach_text(window, 0, 13, 0, 0, "AI Controlled Bullets:");
  widget = g_attach_check_box(window, 240, 0);
  widget->event_function = toggle_ai_bullets;
  
  g_attach_text(window, 0, 35, 0, 0, "AI Controlled Enemy Bullets:");
  widget = g_attach_check_box(window, 240, 30);
  widget->event_function = toggle_ai_enemy_bullets;
  
  g_attach_text(window, 0, 58, 0, 0, "AI Controlled Powerups:");
  widget = g_attach_check_box(window, 240, 53);
  widget->event_function = toggle_ai_powerups;
  
  g_attach_text(window, 0, 81, 0, 0, "AI Controlled Weapons:");
  widget = g_attach_check_box(window, 240, 76);
  widget->event_function = toggle_ai_weapons;
  
  g_attach_text(window, 0, 104, 0, 0, "AI Controlled Debuffs:");
  widget = g_attach_check_box(window, 240, 99);
  widget->event_function = toggle_ai_debuffs;
  
  return window;
}
static g_window *create_function_window(void)
{
  g_window *window = g_create_window(rand() % 400 + 50, rand() % 250 + 50, 375, 135, "Engine Functions");
  window->flags.visible = 0;
  window->event_function = hide_window;
  
  g_widget *widget, *widget_2 = NULL;
  
  widget = g_attach_button(window, 0, 0, "call");
  widget->event_function = button_call_function;
  widget->event_data = (void *)free_all_enemys;
  g_attach_text(window, 65, 14, 0, 0, "free_all_enemys(void);");
  
  widget = g_attach_button(window, 0, 32, "call");
  widget->event_function = button_call_function;
  widget->event_data = (void *)free_all_bullets;
  g_attach_text(window, 65, 39, 0, 0, "free_all_bullets(void);");
  
  widget = g_attach_button(window, 0, 56, "call");
  widget->event_function = button_call_function;
  widget->event_data = (void *)free_all_powerups;
  g_attach_text(window, 65, 63, 0, 0, "free_all_powerups(void);");
  
  widget = g_attach_button(window, 0, 80, "call");
  widget->event_function = button_call_function;
  widget->event_data = (void *)free_all_spaceship_enchants;
  g_attach_text(window, 65, 87, 0, 0, "free_all_spaceship_enchantments(void);");
  
  widget = g_attach_button(window, 0, 104, "call");
  widget->event_function = button_spawn_enemy;
  g_attach_text(window, 65, 111, 0, 0, "spawn_enemy(    );");
  widget_2 = g_attach_input_box(window, 161, 102, 30, G_INPUT_BOX_H);
  widget->event_data = widget_2;
  strcpy(widget_2->input.text, "0");
  widget_2->input.limit = 2;
  widget_2->input.flags.letters = 0;
  widget_2->input.flags.special_chars = 0;
  widget_2->event_function = enemy_parameter;
  
  return window;
}

static void rand_button(const g_event *event, struct g_widget *widget, void *data)
{
  if(event->type != G_MOUSE_MOTION)
    sprintf(widget->button.text, "%c%c%c%c%c%c%c%c%c%c",
            rand() % 94 + 32, rand() % 94 + 32, rand() % 94 + 32, rand() % 94 + 32, rand() % 94 + 32,
            rand() % 94 + 32, rand() % 94 + 32, rand() % 94 + 32, rand() % 94 + 32, rand() % 94 + 32);
}

void create_developers_panel(void)
{
  SDL_Surface *screen = SDL_GetVideoSurface();
  
  g_window *panel = g_create_window(0, screen->h - 32, screen->w, 32, "");
  panel->flags.title_bar = 0;
  panel->margin = 6;
  
  g_widget *widget = NULL;
  
  widget = g_attach_button(panel, 0, 0, "Weapon Menu");
  widget->event_function = show_window;
  widget->event_data = (void *)create_weapon_window();
  
  widget = g_attach_button(panel, 119, 0, "Powerup Menu");
  widget->event_function = show_window;
  widget->event_data = (void *)create_powerup_window();
  
  widget = g_attach_button(panel, 241, 0, "Spaceship Properties");
  widget->event_function = show_window;
  widget->event_data = (void *)create_spaceship_window();
  
  widget = g_attach_button(panel, 426, 0, "AI Features");
  widget->event_function = show_window;
  widget->event_data = (void *)create_ai_window();
  
  widget = g_attach_button(panel, 539 , 0, "Engine Functions");
  widget->event_function = show_window;
  widget->event_data = (void *)create_function_window();
  
  widget = g_attach_button(panel, 692 , 0, " Trololo! ");
  widget->event_function = rand_button;
}