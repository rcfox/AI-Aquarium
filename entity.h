#ifndef ENTITY_H
#define ENTITY_H

#include <libtcod.h>
#include "map.h"

typedef struct
{
	int x;
	int y;
	char c;
	TCOD_color_t color;
	map map;
} entity;

entity* create_entity(map* m, int x, int y, char c, TCOD_color_t color);
void free_entity(entity* e);

void draw_entity(entity* e, TCOD_console_t console);

#endif
