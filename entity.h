#ifndef ENTITY_H
#define ENTITY_H

#include <libtcod.h>
#include "map.h"

typedef struct
{
	int x;
	int y;
	TCOD_color_t color;
	map* host_map;
	map* known_map;
	char c;
} entity;

entity* entity_new(map* m, int x, int y, char c, TCOD_color_t color);
void entity_delete(entity* e);

void entity_draw(entity* e, TCOD_console_t console);

bool entity_move(entity* e, int x, int y);

#endif
