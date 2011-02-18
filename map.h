#ifndef MAP_H
#define MAP_H

#include <libtcod.h>

typedef struct
{
	TCOD_color_t color;
	char c;
} map_display;

typedef struct
{
	map_display* display;
	TCOD_map_t data;
} map;

void create_map(map* m, int width, int height);
void free_map(map* m);

void set_map_data(map* m, int x, int y, char c, TCOD_color_t color, bool transparent, bool walkable);

void randomize_map(map* m, int depth);

#endif
