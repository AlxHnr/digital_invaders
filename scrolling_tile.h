#ifndef SCROLLING_TILE_H
#define SCROLLING_TILE_H

#include <SDL.h>
#include <SDL_image.h>

#include "graphic.h"

extern int init_scrolling_map(void);
extern void reset_scrolling_map(void);

extern void set_scrolling_speed(const double x, const double y);
extern double get_scrolling_speed_x(void);
extern double get_scrolling_speed_y(void);
extern void increase_scrolling_speed(const double x, const double y);

extern void scroll_map(void);
extern void blit_scrolling_map(SDL_Surface *dst);
extern void extend_scrolling_map_timestamp(const Uint32 time);

#endif