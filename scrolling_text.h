#ifndef SCROLLING_TEXT_H
#define SCROLLING_TEXT_H

#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>

#include "bullet.h"

#define s_text_duration 500
#define s_text_frames 25
#define TEXT_BUFFER 256

#define COLOR_ENEMY_DAMAGE 255, 0, 0
#define COLOR_SHIP_DAMAGE 0, 0, 255

#define COLOR_WEAPON 0, 0, 255
#define COLOR_UPGRADE 0, 128, 0
#define COLOR_DOWNGRADE 255, 0, 0

typedef struct scrolling_text scrolling_text;
struct scrolling_text
{
  char text[TEXT_BUFFER];
  SDL_Color color;
  
  double x;
  double y;
  
  Uint32 start_time;
  
  scrolling_text *next;
  scrolling_text *prev;
};

extern int init_scrolling_text();

extern scrolling_text *new_scrolling_text(const double x, const double y, const char *text,
                                          const Uint8 r, const Uint8 g, const Uint8 b);
extern scrolling_text *new_scrolling_int(const double x, const double y, const int value,
                                         const Uint8 r, const Uint8 g, const Uint8 b);
extern void free_scrolling_text(scrolling_text *scr_text);

extern void blit_all_scrolling_texts(SDL_Surface *dst);
extern void extend_all_scrolling_texts_timestamp(const Uint32 time);
extern void free_all_scrolling_texts(void);

#endif