#ifndef ENTITY_H
#define ENTITY_H

#include <libtcod.h>

struct map;

typedef struct entity
{
	int x; // Position in the map
	int y;
	struct map* host_map; // The real map the entity lives in
	struct map* known_map; // What the entity knows about host_map
	TCOD_path_t path; // The path that the entity is currently following

	TCOD_color_t color; // Colour to draw with
	char c; // The character to represent the entity
} entity;

entity* entity_new(int x, int y, char c, TCOD_color_t color);
void entity_delete(entity* e);

void entity_draw(entity* e, TCOD_console_t console);

bool entity_move(entity* e, int x, int y);
void entity_look(entity* e);

void entity_set_map(entity* e, struct map* m);

bool entity_set_destination(entity* e, int x, int y);
bool entity_follow_path(entity* e);
bool entity_at_destination(entity* e);

#endif
