#ifndef MAP_H
#define MAP_H

#include <libtcod.h>

struct entity;

typedef struct map_display
{
	TCOD_color_t color;
	char c;
} map_display;

typedef struct map
{
	map_display* display;
	TCOD_map_t data;
	TCOD_list_t entities;
} map;

map* map_new(int width, int height, char init_char);
void map_delete(map* m);

void map_draw(map* m, TCOD_console_t console);

void map_copy_data(map* src, map* dest, int x, int y);
void map_set_data(map* m, int x, int y, char c, TCOD_color_t color, bool transparent, bool walkable);

void map_add_entity(map* m, struct entity* e);
void map_remove_entity(map* m, struct entity* e);

void map_randomize(map* m, int depth);

void map_random_free_spot(map* m, int* x, int* y);

#endif
