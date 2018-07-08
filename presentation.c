#include "presentation.h"

static bool pres_running = true;

/* needed for presentation */
static g_window *pres_window = NULL;
static TTF_Font *pres_font_big = NULL;
static TTF_Font *pres_font_normal = NULL;
static TTF_Font *pres_font_small = NULL;
static SDL_Surface *sdl_code_example = NULL;
static SDL_Surface *module_diagram = NULL;
static SDL_Surface *tile = NULL;
static SDL_Surface *spaceship_struct = NULL;
static SDL_Surface *ai_image = NULL;
static SDL_Surface *spaceship_image = NULL;
static SDL_Surface *enemy_image = NULL;
static SDL_Surface *collision_image = NULL;
static spaceship *ship = NULL;
static page_fun current_fun;

/* some usefull functions */
static void free_surface_widget(g_widget *widget);
static void button_event_goto_page(const g_event *event, g_widget *widget, void *data);
static void button_unhide_window(const g_event *event, g_widget *widget, void *data);
static void button_toggle_module(const g_event *event, g_widget *widget, void *data);
static void quit_presentation(const g_event *event, g_widget *window, void *data);
static void hide_window_on_close(const g_event *event, g_window *window, void *data);
static g_widget *attach_pres_button(const pres_button_type type, const page_fun fun);

/* page functions */
static void page_1(const fun_call_type type);
static void page_2(const fun_call_type type);
static void page_3(const fun_call_type type);
static void page_4(const fun_call_type type);
static void page_5(const fun_call_type type);
static void page_6(const fun_call_type type);
static void page_7(const fun_call_type type);

/* core functions */
static void exit_presentation(void)
{
  TTF_CloseFont(pres_font_big);
  TTF_CloseFont(pres_font_normal);
  TTF_CloseFont(pres_font_small);
  
  SDL_FreeSurface(sdl_code_example);
  SDL_FreeSurface(module_diagram);
  SDL_FreeSurface(tile);
  SDL_FreeSurface(spaceship_struct);
  SDL_FreeSurface(ai_image);
  SDL_FreeSurface(spaceship_image);
  SDL_FreeSurface(enemy_image);
  SDL_FreeSurface(collision_image);
  
  if(current_fun)
    current_fun(clean_up);
}
int init_presentation(void)
{
  if(!init_scrolling_map())
    return 0;
  
  if(!init_bullet())
    return 0;
  
  if(!init_spaceship())
    return 0;
  ship = get_spaceship();
  
  if(!init_enemys())
    return 0;
  
  if(!init_scrolling_text())
    return 0;
  
  if(!init_weapons())
    return 0;
  
  if(!init_powerups())
    return 0;
  
  if(!init_interface())
    return 0;
  
  if(!g_init_everything())
    return 0;
  set_gui_theme();
  
  if(!(pres_font_big = TTF_OpenFont("gfx/LiberationSans-Regular.ttf", 50)))
    return 0;
  if(!(pres_font_normal = TTF_OpenFont("gfx/LiberationSans-Regular.ttf", 25)))
    return 0;
  if(!(pres_font_small = TTF_OpenFont("gfx/LiberationSans-Regular.ttf", 18)))
    return 0;
  
  if(!(sdl_code_example = load_image("sdl_code_example.png")))
    return 0;
  if(!(module_diagram = load_image("diagram.png")))
    return 0;
  if(!(tile = load_image("gfx/tile.png")))
    return 0;
  if(!(spaceship_struct = load_image("spaceship_struct.png")))
    return 0;
  if(!(ai_image = load_image("ai.png")))
    return 0;
  if(!(spaceship_image = load_image("spaceship.png")))
    return 0;
  if(!(enemy_image = load_image("enemy.png")))
    return 0;
  if(!(collision_image = load_image("collision.png")))
    return 0;
  
  if(!(pres_window = g_create_window(0, 0, 0, 0, "")))
    return 0;
  pres_window->flags.title_bar = 0;
  pres_window->margin = 0;
  pres_window->color.background.r = 255;
  pres_window->color.background.g = 255;
  pres_window->color.background.b = 255;
  pres_window->color.background.a = 255;
  g_maximize_window(pres_window);
  
  page_1(init);
  
  atexit(exit_presentation);
  
  return 1;
}
void run_presentation(SDL_Surface *screen)
{
  SDL_Event event;
  while(pres_running)
  {
    while(SDL_PollEvent(&event))
    {
      if(event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
        pres_running = false;
    }
    
    /* render */
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
    
    g_draw_everything(screen);
    current_fun(run);
    
    SDL_Flip(screen);
    controll_framerate();
  }
}

/* page functions */
static void page_1(const fun_call_type type)
{
  static g_widget *background_widget = NULL;
  static g_widget *title_text_widget = NULL;
  static g_widget *name_text_widget = NULL;
  static g_widget *next_button = NULL;
  
  static int dir_y = -1;
  SDL_Color color;
  
  if(type == init)
  {
    /* setup background and text */
    background_widget = g_attach_surface(pres_window, 0, 0, 0, 0, SDL_ConvertSurface(SDL_GetVideoSurface(),
                                                                                     SDL_GetVideoSurface()->format,
                                                                                     SDL_GetVideoSurface()->flags));
    color.r = color.g = color.b = 0;
    title_text_widget = g_attach_surface(pres_window, 230, 150, 0, 0,
                                         TTF_RenderUTF8_Blended(pres_font_big, "Digital Invaders", color));
    
    name_text_widget = g_attach_surface(pres_window, 300, 215, 0, 0,
                                        TTF_RenderUTF8_Blended(pres_font_normal, "Alexander Heinrich", color));
    
    next_button = attach_pres_button(next, page_2);
    
    /* set page properties */
    current_fun = page_1;
    dir_y = -1;
    
    reset_spaceship();
    ship->speed = 0.05;
    ship->acceleration = 0.000075;
    set_scrolling_speed(-0.02, 0.0);
  }
  else if(type == clean_up)
  {
    free_surface_widget(background_widget);
    free_surface_widget(title_text_widget);
    free_surface_widget(name_text_widget);
    g_destroy_widget(next_button);
    
    reset_spaceship();
    reset_scrolling_map();
  }
  else if(type == run)
  {
    scroll_map();
    blit_scrolling_map(background_widget->surface.surface);
    
    /* move spaceship */
    if(ship->y < 150 && dir_y < 0)
      dir_y = 1;
    if(ship->y > background_widget->surface.surface->h - 150 && dir_y > 0)
      dir_y = -1;
    accelerate_spaceship(0, dir_y);
    
    handle_spaceship();
    blit_spaceship(background_widget->surface.surface);
    
    /* grey out screen */
    boxRGBA(background_widget->surface.surface, 0, 0, background_widget->surface.surface->w,
             background_widget->surface.surface->h, 0, 0, 0, 20);
  }
}
static void page_2(const fun_call_type type)
{
  static g_widget *background_widget = NULL;
  static g_widget *title_text_widget = NULL;
  static g_widget *sdl_text_widget = NULL;
  static g_widget *lines_text_widget = NULL;
  static g_widget *movement_text_widget = NULL;
  static g_widget *controll_text_widget = NULL;
  static g_widget *controll_text_widget_2 = NULL;
  static g_widget *module_widget = NULL;
  
  static g_widget *button_code_example = NULL;
  static g_widget *button_code_run_example = NULL;
  static g_widget *button_module_toggle = NULL;
  
  static g_window *sdl_code_window = NULL;
  static g_window *sdl_code_run_window = NULL;
  static g_widget *sdl_code_run_surface = NULL;
  
  static g_widget *prev_button = NULL;
  static g_widget *next_button = NULL;
  SDL_Color color;
  
  if(type == init)
  {
    background_widget = g_attach_surface(pres_window, 0, 0, 0, 0, SDL_ConvertSurface(SDL_GetVideoSurface(),
                                                                                     SDL_GetVideoSurface()->format,
                                                                                     SDL_GetVideoSurface()->flags));
    color.r = color.g = color.b = 0;
    title_text_widget = g_attach_surface(pres_window, 220, 20, 0, 0,
                                         TTF_RenderUTF8_Blended(pres_font_big, "Digital Invaders", color));
    sdl_text_widget = g_attach_surface(pres_window, 15, 94, 0, 0,
                                         TTF_RenderUTF8_Blended(pres_font_normal, "- SDL", color));
    
    lines_text_widget = g_attach_surface(pres_window, 15, 142, 0, 0,
                                         TTF_RenderUTF8_Blended(pres_font_normal, "- Ca. 4000 Zeilen Quellcode in 29 Dateien", color));
    movement_text_widget = g_attach_surface(pres_window, 15, 190, 0, 0,
                                         TTF_RenderUTF8_Blended(pres_font_normal, "- Bewegung im Spiel Zeitabhängig", color));
    controll_text_widget = g_attach_surface(pres_window, 15, 238, 0, 0,
                                         TTF_RenderUTF8_Blended(pres_font_normal, "- Steuerung mit Pfeiltasten, Pause mit Escape,", color));
    controll_text_widget_2 = g_attach_surface(pres_window, 15, 262, 0, 0,
                                         TTF_RenderUTF8_Blended(pres_font_normal, "  Schießen mit Leertaste und Autofire mit Tab", color));
    
    module_widget = g_attach_surface(pres_window, 1000, 350, 0, 0, module_diagram);
    
    sdl_code_window = g_create_window(200, 50, sdl_code_example->w, sdl_code_example->h, "SDL Codebeispiel");
    sdl_code_window->event_function = hide_window_on_close;
    sdl_code_window->flags.visible = 0;
    sdl_code_window->margin = 0;
    g_attach_surface(sdl_code_window, 0, 0, 0, 0, sdl_code_example);
    
    sdl_code_run_window = g_create_window(300, 280, 320, 240, "SDL Window");
    sdl_code_run_window->event_function = hide_window_on_close;
    sdl_code_run_window->flags.visible = 0; 
    sdl_code_run_window->margin = 0;
    sdl_code_run_surface = g_attach_surface(sdl_code_run_window, 0, 0, 0, 0, SDL_ConvertSurface(SDL_GetVideoSurface(),
                                                                             SDL_GetVideoSurface()->format,
                                                                             SDL_GetVideoSurface()->flags));
    SDL_FillRect(sdl_code_run_surface->surface.surface, NULL, SDL_MapRGB(sdl_code_run_surface->surface.surface->format, 0, 0, 0));
    boxRGBA(sdl_code_run_surface->surface.surface, 50, 50, 150, 150, 255, 255, 0, 255);
    
    button_code_example = g_attach_button(pres_window, 100, 570, "Code Beispiel");
    button_code_example->event_function = button_unhide_window;
    button_code_example->event_data = sdl_code_window;
    
    button_code_run_example = g_attach_button(pres_window, 241, 570, "Programmfenster");
    button_code_run_example->event_function = button_unhide_window;
    button_code_run_example->event_data = sdl_code_run_window;
    
    button_module_toggle = g_attach_button(pres_window, 395, 570, "Module");
    button_module_toggle->event_function = button_toggle_module;
    button_module_toggle->event_data = module_widget;
    
    prev_button = attach_pres_button(prev, page_1);
    next_button = attach_pres_button(next, page_3);
    
    /* set page properties */
    current_fun = page_2;
  }
  else if(type == clean_up)
  {
    free_surface_widget(background_widget);
    free_surface_widget(title_text_widget);
    free_surface_widget(sdl_text_widget);
    free_surface_widget(lines_text_widget);
    free_surface_widget(movement_text_widget);
    free_surface_widget(controll_text_widget);
    free_surface_widget(controll_text_widget_2);
    
    g_destroy_window(sdl_code_window);
    free_surface_widget(sdl_code_run_surface);
    g_destroy_window(sdl_code_run_window);
    
    g_destroy_widget(button_code_example);
    g_destroy_widget(button_code_run_example);
    g_destroy_widget(module_widget);
    g_destroy_widget(button_module_toggle);
    
    g_destroy_widget(prev_button);
    g_destroy_widget(next_button);
  }
  else if(type == run)
  {
    /* draw background */
    blit_scrolling_map(background_widget->surface.surface);
    boxRGBA(background_widget->surface.surface, 0, 0, background_widget->surface.surface->w,
             background_widget->surface.surface->h, 240, 240, 240, 150);
    
    g_raise_window(sdl_code_window);
    g_raise_window(sdl_code_run_window);
  }
}
static void page_3(const fun_call_type type)
{
  static g_widget *background_widget = NULL;
  static g_widget *title_text_widget = NULL;
  static g_widget *tile_text_widget = NULL;
  static g_widget *pos_widget = NULL;
  static g_widget *shift_widget = NULL;
  static g_widget *dst_widget = NULL;
  
  static g_widget *prev_button = NULL;
  static g_widget *next_button = NULL;
  
  static int render_mode = 0;
  static int tile_count, tmp_count;
  static Uint32 time_stop = 0;
  static Uint32 time_stop_y = 0;
  static SDL_Surface *background_surface = NULL;
  SDL_Rect tmp_rect, tmp_src;
  Uint8 *key_map = SDL_GetKeyState(NULL);
  
  SDL_Color color;
  
  if(type == init)
  {
    background_widget = g_attach_surface(pres_window, 0, 0, 0, 0, SDL_ConvertSurface(SDL_GetVideoSurface(),
                                                                                     SDL_GetVideoSurface()->format,
                                                                                     SDL_GetVideoSurface()->flags));
    background_surface = SDL_ConvertSurface(SDL_GetVideoSurface(),
                                            SDL_GetVideoSurface()->format,
                                            SDL_GetVideoSurface()->flags);
    color.r = color.g = color.b = 0;
    title_text_widget = g_attach_surface(pres_window, 220, 20, 0, 0,
                                         TTF_RenderUTF8_Blended(pres_font_big, "Hintergrund", color));
    tile_text_widget = g_attach_surface(pres_window, 15, 94, 0, 0,
                                         TTF_RenderUTF8_Blended(pres_font_normal, "- Wird mit Tiles gezeichnet", color));
    dst_widget = g_attach_surface(pres_window, 15, 142, 0, 0,
                                         TTF_RenderUTF8_Blended(pres_font_normal, "- 2 Schleifen: horizontale in vertikaler Schleife", color));
    pos_widget = g_attach_surface(pres_window, 15, 190, 0, 0,
                                         TTF_RenderUTF8_Blended(pres_font_normal, "- Position der Map wird im Gleitkommabereich gespeichert", color));
    shift_widget = g_attach_surface(pres_window, 15, 238, 0, 0,
                                         TTF_RenderUTF8_Blended(pres_font_normal, "- Abhängig von der Position wird das Tile verschoben", color));
    
    prev_button = attach_pres_button(prev, page_2);
    next_button = attach_pres_button(next, page_4);
    
    render_mode = 0;
    current_fun = page_3;
  }
  else if(type == clean_up)
  {
    free_surface_widget(background_widget);
    free_surface_widget(title_text_widget);
    free_surface_widget(tile_text_widget);
    free_surface_widget(pos_widget);
    free_surface_widget(shift_widget);
    free_surface_widget(dst_widget);
    
    SDL_FreeSurface(background_surface);
    
    g_destroy_widget(prev_button);
    g_destroy_widget(next_button);
  }
  else if(type == run)
  {
    /* draw background */
    blit_scrolling_map(background_widget->surface.surface);
    boxRGBA(background_widget->surface.surface, 0, 0, background_widget->surface.surface->w,
            background_widget->surface.surface->h, 240, 240, 240, 150);
    
    if(key_map[SDLK_1] && render_mode != 1)
    {
      render_mode = 1;
      tile_count = 0;
      time_stop = SDL_GetTicks();
    }
    else if(key_map[SDLK_2] && render_mode != 2)
    {
      render_mode = 2;
      time_stop = SDL_GetTicks();
    }
    else if(key_map[SDLK_3] && render_mode != 3)
    {
      time_stop_y = SDL_GetTicks();
      
      render_mode = 3;
    }
    else if(key_map[SDLK_4] && render_mode != 4)
    {
      render_mode = 4;
      time_stop = SDL_GetTicks();
    }
    else if(key_map[SDLK_5] && render_mode != 0)
    {
      render_mode = 0;
    }
    
    if(render_mode == 1)
    {
      rectangleRGBA(background_widget->surface.surface, 269, 349, 510, 494, 0, 0, 0, 255);
      boxRGBA(background_widget->surface.surface, 270, 350, 509, 493, 200, 255, 100, 255);
      
      /* animate */
      if(SDL_GetTicks() - time_stop > 50)
      {
        tile_count++;
        if(tile_count > 60)
          tile_count = 0;
        
        time_stop = SDL_GetTicks();
      }
      
      /* render loop */
      tmp_count = 0;
      for(tmp_rect.y = 350; tmp_count < tile_count; tmp_rect.y += tile->h)
      {
        for(tmp_rect.x = 270; tmp_rect.x < 494 && tmp_count < tile_count; tmp_rect.x += tile->w)
        {
          SDL_BlitSurface(tile, NULL, background_widget->surface.surface, &tmp_rect);
          tmp_count++;
        }
      }
    }
    else if(render_mode == 2 || render_mode == 3)
    {
      /* animate */
      if(SDL_GetTicks() - time_stop > 1000)
      {
        time_stop = SDL_GetTicks();
      }
      if(render_mode == 3 && SDL_GetTicks() - time_stop_y > 1000)
      {
        time_stop_y = SDL_GetTicks();
      }
      
      tmp_rect.x = 246 + (SDL_GetTicks() - time_stop)/42;
      tmp_rect.y = 326;
      
      if(render_mode == 3)
        tmp_rect.y += (SDL_GetTicks() - time_stop_y)/42;
      
      tmp_src.x = tmp_src.y = 0;
      tmp_src.w = 264;
      tmp_src.h = 168;
      
      blit_scrolling_map(background_surface);
      SDL_BlitSurface(background_surface, &tmp_src, background_widget->surface.surface, &tmp_rect);
      
      rectangleRGBA(background_widget->surface.surface, tmp_rect.x - 1, tmp_rect.y - 1,
                    tmp_rect.x + tmp_rect.w, tmp_rect.y + tmp_rect.h, 0, 0, 255, 255);
      rectangleRGBA(background_widget->surface.surface, 269, 349, 510, 494, 0, 0, 0, 255);
    }
    else if(render_mode == 4)
    {
      /* animate */
      if(SDL_GetTicks() - time_stop > 1000)
      {
        time_stop = SDL_GetTicks();
      }
      
      tmp_rect.x = 270;
      tmp_rect.y = 350;
      
      tmp_src.x = tmp_src.y = 24 - (SDL_GetTicks() - time_stop)/42;
      tmp_src.w = 240;
      tmp_src.h = 144;
      
      blit_scrolling_map(background_surface);
      SDL_BlitSurface(background_surface, &tmp_src, background_widget->surface.surface, &tmp_rect);
      
      rectangleRGBA(background_widget->surface.surface, 269, 349, 510, 494, 0, 0, 0, 255);
    }
  }
}
static void page_4(const fun_call_type type)
{
  static g_widget *background_widget = NULL;
  static g_widget *title_text_widget = NULL;
  static g_widget *struct_widget = NULL;
  static g_widget *text_1 = NULL;
  static g_widget *text_2 = NULL;
  static g_widget *text_3 = NULL;
  static g_widget *text_4 = NULL;
  
  static g_widget *prev_button = NULL;
  static g_widget *next_button = NULL;
  
  SDL_Color color;
  
  if(type == init)
  {
    /* setup background and text */
    background_widget = g_attach_surface(pres_window, 0, 0, 0, 0, SDL_ConvertSurface(SDL_GetVideoSurface(),
                                                                                     SDL_GetVideoSurface()->format,
                                                                                     SDL_GetVideoSurface()->flags));
    color.r = color.g = color.b = 0;
    title_text_widget = g_attach_surface(pres_window, 220, 20, 0, 0,
                                         TTF_RenderUTF8_Blended(pres_font_big, "Das Spaceship", color));
    
    text_1 = g_attach_surface(pres_window, 15, 94, 0, 0,
                              TTF_RenderUTF8_Blended(pres_font_normal, "- Typisches Beispiel für ein Modul", color));
    
    text_2 = g_attach_surface(pres_window, 15, 142, 0, 0,
                              TTF_RenderUTF8_Blended(pres_font_normal, "- Versteckt alles hinter Funktionen", color));
    text_3 = g_attach_surface(pres_window, 15, 190, 0, 0,
                              TTF_RenderUTF8_Blended(pres_font_normal, "- Spaceship komplett verwaltbar mit nur 5 Funktionen", color));
    text_4 = g_attach_surface(pres_window, 15, 238, 0, 0,
                              TTF_RenderUTF8_Blended(pres_font_normal, "- Spaceship muss beschleunigen", color));
    
    struct_widget = g_attach_surface(pres_window, 104, 288, 0, 0, spaceship_struct);
    
    prev_button = attach_pres_button(prev, page_3);
    next_button = attach_pres_button(next, page_5);
    
    /* set page properties */
    current_fun = page_4;
  }
  else if(type == clean_up)
  {
    free_surface_widget(background_widget);
    free_surface_widget(title_text_widget);
    
    free_surface_widget(text_1);
    free_surface_widget(text_2);
    free_surface_widget(text_3);
    free_surface_widget(text_4);
    
    g_destroy_widget(struct_widget);
    
    g_destroy_widget(prev_button);
    g_destroy_widget(next_button);
  }
  else if(type == run)
  {
    blit_scrolling_map(background_widget->surface.surface);
    
    /* grey out screen */
    boxRGBA(background_widget->surface.surface, 0, 0, background_widget->surface.surface->w,
             background_widget->surface.surface->h, 240, 240, 240, 150);
  }
}
static void page_5(const fun_call_type type)
{
  static g_widget *background_widget = NULL;
  static g_widget *title_text_widget = NULL;
  
  static g_widget *button_ai = NULL;
  static g_window *ai_window = NULL;
  
  static g_widget *button_demo = NULL;
  static g_window *demo_window = NULL;
  static g_widget *demo_surface = NULL;
  
  static g_widget *text_1 = NULL;
  static g_widget *text_2 = NULL;
  static g_widget *text_3 = NULL;
  static g_widget *text_4 = NULL;
  static g_widget *text_5 = NULL;
  
  static g_widget *prev_button = NULL;
  static g_widget *next_button = NULL;
  
  SDL_Color color;
  Uint8 *key_map = SDL_GetKeyState(NULL);
  int dir_x, dir_y;
  
  if(type == init)
  {
    /* setup background and text */
    background_widget = g_attach_surface(pres_window, 0, 0, 0, 0, SDL_ConvertSurface(SDL_GetVideoSurface(),
                                                                                     SDL_GetVideoSurface()->format,
                                                                                     SDL_GetVideoSurface()->flags));
    color.r = color.g = color.b = 0;
    title_text_widget = g_attach_surface(pres_window, 250, 20, 0, 0,
                                         TTF_RenderUTF8_Blended(pres_font_big, "Gegner / KI", color));
    
    text_1 = g_attach_surface(pres_window, 15, 94, 0, 0,
                              TTF_RenderUTF8_Blended(pres_font_normal, "- Jeder Gegner hat eine KI", color));
    text_2 = g_attach_surface(pres_window, 15, 142, 0, 0,
                              TTF_RenderUTF8_Blended(pres_font_normal, "- Können die selben Waffen wie das Ship benutzen", color));
    text_3 = g_attach_surface(pres_window, 15, 190, 0, 0,
                              TTF_RenderUTF8_Blended(pres_font_normal, "- Verhalten abhängig von Statuswerten", color));
    text_4 = g_attach_surface(pres_window, 15, 238, 0, 0,
                              TTF_RenderUTF8_Blended(pres_font_normal, "- Bösse sind \"normale\" Gegner mit besonderer KI,", color));
    text_5 = g_attach_surface(pres_window, 15, 262, 0, 0,
                              TTF_RenderUTF8_Blended(pres_font_normal, "  hohen Statuswerten und Spezialwaffen", color));
    
    ai_window = g_create_window(130, 100, 576, 329, "Summenzeichen KI");
    ai_window->flags.visible = 0;
    ai_window->margin = 0;
    ai_window->event_function = hide_window_on_close;
    g_attach_surface(ai_window, 0, 0, 0, 0, ai_image);
    
    button_ai = g_attach_button(pres_window, 70, 570, "Summenzeichen KI");
    button_ai->event_function = button_unhide_window;
    button_ai->event_data = ai_window;
    
    demo_window = g_create_window(130, 200, 576, 329, "Demo Fenster");
    demo_window->flags.visible = 0;
    demo_window->margin = 0;
    demo_window->event_function = hide_window_on_close;
    
    demo_surface = g_attach_surface(demo_window, 0, 0, 0, 0, SDL_ConvertSurface(ai_image,
                                                                                ai_image->format,
                                                                                ai_image->flags));
    
    button_demo = g_attach_button(pres_window, 240, 570, "Demo");
    button_demo->event_function = button_unhide_window;
    button_demo->event_data = demo_window;
    
    prev_button = attach_pres_button(prev, page_4);
    next_button = attach_pres_button(next, page_6);
    
    /* set page properties */
    reset_scrolling_map();
    reset_spaceship();
    ship->x = 20;
    ship->y = 165;
    ship->wpn_type = blue_laser;
    enemy *tmp_enm = spawn_enemy(2);
    tmp_enm->y = 50;
    tmp_enm->x = demo_surface->surface.surface->w - get_enemy_w(2) - 10;
    
    current_fun = page_5;
  }
  else if(type == clean_up)
  {
    free_surface_widget(background_widget);
    free_surface_widget(title_text_widget);
    free_surface_widget(text_1);
    free_surface_widget(text_2);
    free_surface_widget(text_3);
    free_surface_widget(text_4);
    free_surface_widget(text_5);
    
    g_destroy_widget(button_ai);
    g_destroy_window(ai_window);
    
    g_destroy_widget(button_demo);
    free_surface_widget(demo_surface);
    g_destroy_window(demo_window);
    
    g_destroy_widget(prev_button);
    g_destroy_widget(next_button);
    
    free_all_bullets();
    free_all_enemys();
  }
  else if(type == run)
  {
    blit_scrolling_map(background_widget->surface.surface);
    
    /* grey out screen */
    boxRGBA(background_widget->surface.surface, 0, 0, background_widget->surface.surface->w,
             background_widget->surface.surface->h, 240, 240, 240, 150);
    
    /* handle input */
    if(key_map[SDLK_LEFT])
      dir_x = -1;
    else if(key_map[SDLK_RIGHT])
      dir_x = 1;
    else
      dir_x = 0;
    
    if(key_map[SDLK_UP])
      dir_y = -1;
    else if(key_map[SDLK_DOWN])
      dir_y = 1;
    else
      dir_y = 0;
    
    if(key_map[SDLK_SPACE])
      spaceship_shot();
    
    /* handle game */
    accelerate_spaceship(dir_x, dir_y);
    handle_spaceship();
    move_all_bullets();
    handle_all_enemys();
    
    /* render game */
    blit_scrolling_map(demo_surface->surface.surface);
    blit_spaceship(demo_surface->surface.surface);
    blit_all_bullets(demo_surface->surface.surface);
    blit_all_enemys(demo_surface->surface.surface);
    
    /* raise windows */
    g_raise_window(ai_window);
    g_raise_window(demo_window);
  }
}
static void page_6(const fun_call_type type)
{
  static g_widget *background_widget = NULL;
  static g_widget *title_text_widget = NULL;
  
  static g_widget *text_1 = NULL;
  static g_widget *text_2 = NULL;
  static g_widget *text_3 = NULL;
  static g_widget *text_4 = NULL;
  static g_widget *text_5 = NULL;
  
  static g_widget *prev_button = NULL;
  static g_widget *next_button = NULL;
  static Uint32 time_stop = 0;
  static int render_mode;
  static Uint8 *key_map = NULL;
  SDL_Rect tmp_rect;
  
  SDL_Color color;
  
  if(type == init)
  {
    /* setup background and text */
    background_widget = g_attach_surface(pres_window, 0, 0, 0, 0, SDL_ConvertSurface(SDL_GetVideoSurface(),
                                                                                     SDL_GetVideoSurface()->format,
                                                                                     SDL_GetVideoSurface()->flags));
    color.r = color.g = color.b = 0;
    title_text_widget = g_attach_surface(pres_window, 290, 20, 0, 0,
                                         TTF_RenderUTF8_Blended(pres_font_big, "Kollision", color));
    
    text_1 = g_attach_surface(pres_window, 15, 94, 0, 0,
                              TTF_RenderUTF8_Blended(pres_font_normal, "- Überprüfung ob 2 Rechtecke sich berühren", color));
    text_2 = g_attach_surface(pres_window, 15, 142, 0, 0,
                              TTF_RenderUTF8_Blended(pres_font_normal, "- x1, y1 und x2, y2 Notwendig für jeweils beide Rechtecke", color));
    text_3 = g_attach_surface(pres_window, 15, 190, 0, 0,
                              TTF_RenderUTF8_Blended(pres_font_normal, "- Kollision wenn alle 4 Bedingungen wahr:", color));
    text_4 = g_attach_surface(pres_window, 15, 214, 0, 0,
                              TTF_RenderUTF8_Blended(pres_font_normal, "      Ship.x1 < Enemy.x2  &&  Ship.x2 > Enemy.x1  &&", color));
    text_5 = g_attach_surface(pres_window, 15, 238, 0, 0,
                              TTF_RenderUTF8_Blended(pres_font_normal, "      Ship.y1 < Enemy.y2  &&  Ship.y2 > Enemy.y1", color));
    
    prev_button = attach_pres_button(prev, page_5);
    next_button = attach_pres_button(next, page_7);
    
    /* set page properties */
    render_mode = 0;
    key_map = SDL_GetKeyState(NULL);
    current_fun = page_6;
    time_stop = SDL_GetTicks();
  }
  else if(type == clean_up)
  {
    free_surface_widget(background_widget);
    free_surface_widget(title_text_widget);
    
    free_surface_widget(text_1);
    free_surface_widget(text_2);
    free_surface_widget(text_3);
    free_surface_widget(text_4);
    free_surface_widget(text_5);
    //~ free_surface_widget(text_6);
    //~ free_surface_widget(text_7);
    
    g_destroy_widget(prev_button);
    g_destroy_widget(next_button);
  }
  else if(type == run)
  {
    blit_scrolling_map(background_widget->surface.surface);
    
    /* grey out screen */
    boxRGBA(background_widget->surface.surface, 0, 0, background_widget->surface.surface->w,
             background_widget->surface.surface->h, 240, 240, 240, 150);
    
    /* handle input */
    if(key_map[SDLK_1] && render_mode != 1)
    {
      render_mode = 1;
    }
    else if(key_map[SDLK_2] && render_mode != 2)
    {
      render_mode = 2;
      time_stop = SDL_GetTicks();
    }
    else if(key_map[SDLK_3] && render_mode != 0)
    {
      render_mode = 0;
    }
    
    /* render collision */
    if(render_mode == 1)
    {
      tmp_rect.x = 220;
      tmp_rect.y = 315;
      
      SDL_BlitSurface(collision_image, NULL, background_widget->surface.surface, &tmp_rect);
    }
    else if(render_mode == 2)
    {
      /* animate collision */
      if(SDL_GetTicks() - time_stop > 1250)
        time_stop = SDL_GetTicks();
      
      /* draw collision */
      tmp_rect.x = 250 + (SDL_GetTicks() - time_stop)/40;
      tmp_rect.y = 400 - (SDL_GetTicks() - time_stop)/60;
      
      SDL_BlitSurface(spaceship_image, NULL, background_widget->surface.surface, &tmp_rect);
      
      if(SDL_GetTicks() - time_stop < 700)
        rectangleRGBA(background_widget->surface.surface, tmp_rect.x, tmp_rect.y,
                      tmp_rect.x + spaceship_image->w, tmp_rect.y + spaceship_image->h, 0, 127, 0, 255);
      else
        rectangleRGBA(background_widget->surface.surface, tmp_rect.x, tmp_rect.y,
                      tmp_rect.x + spaceship_image->w, tmp_rect.y + spaceship_image->h, 255, 0, 0, 255);
      
      tmp_rect.x = 410 - (SDL_GetTicks() - time_stop)/40;
      tmp_rect.y = 320 + (SDL_GetTicks() - time_stop)/60;
      
      SDL_BlitSurface(enemy_image, NULL, background_widget->surface.surface, &tmp_rect);
      
      if(SDL_GetTicks() - time_stop < 700)
        rectangleRGBA(background_widget->surface.surface, tmp_rect.x, tmp_rect.y,
                      tmp_rect.x + enemy_image->w, tmp_rect.y + enemy_image->h, 0, 127, 0, 255);
      else
        rectangleRGBA(background_widget->surface.surface, tmp_rect.x, tmp_rect.y,
                      tmp_rect.x + enemy_image->w, tmp_rect.y + enemy_image->h, 255, 0, 0, 255);
    }
  }
}
static void page_7(const fun_call_type type)
{
  static g_widget *background_widget = NULL;
  
  static g_widget *prev_button = NULL;
  static g_widget *quit_button = NULL;
  
  static SDL_Surface *title_text_widget = NULL;
  static SDL_Surface *tmp_surface = NULL;
  static Uint32 timer = 0;
  SDL_Rect tmp_rect;
  
  SDL_Color color;
  
  if(type == init)
  {
    /* setup background and text */
    background_widget = g_attach_surface(pres_window, 0, 0, 0, 0, SDL_ConvertSurface(SDL_GetVideoSurface(),
                                                                                     SDL_GetVideoSurface()->format,
                                                                                     SDL_GetVideoSurface()->flags));
    color.r = color.g = color.b = 0;
    
    prev_button = attach_pres_button(prev, page_6);
    quit_button = g_attach_button(pres_window, 735, 570, "Ende");
    quit_button->event_function = quit_presentation;
    
    /* set page properties */
    timer = SDL_GetTicks();
    current_fun = page_7;
  }
  else if(type == clean_up)
  {
    free_surface_widget(background_widget);
    
    g_destroy_widget(prev_button);
    g_destroy_widget(quit_button);
  }
  else if(type == run)
  {
    blit_scrolling_map(background_widget->surface.surface);
    
    /* grey out screen */
    boxRGBA(background_widget->surface.surface, 0, 0, background_widget->surface.surface->w,
             background_widget->surface.surface->h, 240, 240, 240, 150);
    
    if(SDL_GetTicks() - timer > 10000)
    {
      tmp_surface = TTF_RenderUTF8_Blended(pres_font_big, "Ende", color);
      title_text_widget = rotozoomSurface(tmp_surface, 0 - (SDL_GetTicks() - timer - 10000)/10.0, 1.0, SMOOTHING_ON);
      
      SDL_FreeSurface(tmp_surface);
      
      tmp_rect.x = 320;
      tmp_rect.y = 220;
      
      if(SDL_GetTicks() - timer > 10300)
      {
        tmp_rect.x += pow(SDL_GetTicks() - timer - 10300, 2)/1000.0;
        tmp_rect.y += pow(SDL_GetTicks() - timer - 10300, 2)/700.0;
      }
      
      SDL_BlitSurface(title_text_widget, NULL, background_widget->surface.surface, &tmp_rect);
      SDL_FreeSurface(title_text_widget);
    }
    else
    {
      title_text_widget = TTF_RenderUTF8_Blended(pres_font_big, "Ende", color);
      
      tmp_rect.x = 320;
      tmp_rect.y = 220;
      SDL_BlitSurface(title_text_widget, NULL, background_widget->surface.surface, &tmp_rect);
      SDL_FreeSurface(title_text_widget);
    }
  }
}

/* some usefull functions */
static void free_surface_widget(g_widget *widget)
{
  SDL_FreeSurface(widget->surface.surface);
  g_destroy_widget(widget);
}
static void button_event_goto_page(const g_event *event, g_widget *widget, void *data)
{
  page_fun tmp_fun;
  
  if(event->type == G_CLICK_LEFT)
  {
    current_fun(clean_up);
    tmp_fun = (page_fun)data;
    tmp_fun(init);
  }
}
static void button_unhide_window(const g_event *event, g_widget *widget, void *data)
{
  g_window *window = (g_window *)data;
  if(event->type == G_CLICK_LEFT)
  {
    g_toggle(window->flags.visible);
  }
}
static void button_toggle_module(const g_event *event, g_widget *widget, void *data)
{
  g_widget *surface_widget = (g_widget *)data;
  if(event->type == G_CLICK_LEFT)
  {
    if(surface_widget->x == 1000)
      surface_widget->x = 100;
    else
      surface_widget->x = 1000;
  }
}
static void hide_window_on_close(const g_event *event, g_window *window, void *data)
{
  if(event->type == G_WINDOW_CLOSE)
  {
    window->flags.visible = 0;
  }
}
static void quit_presentation(const g_event *event, g_widget *widget, void *data)
{
  if(event->type == G_CLICK_LEFT)
  {
    pres_running = false;
  }
}
static g_widget *attach_pres_button(const pres_button_type type, const page_fun fun)
{
  g_widget *tmp_button = NULL;
  
  if(type == prev)
  {
    tmp_button = g_attach_button(pres_window, PREV_BUTTON_POS, PREV_BUTTON_TEXT);
  }
  else if(type == next)
  {
    tmp_button = g_attach_button(pres_window, NEXT_BUTTON_POS, NEXT_BUTTON_TEXT);
  }
  else
  {
    return NULL;
  }
  
  if(!tmp_button)
    return NULL;
  
  tmp_button->event_function = button_event_goto_page;
  tmp_button->event_data = (void *)fun;
  return tmp_button;
}