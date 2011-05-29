#ifndef ENTITY_H
#define ENTITY_H

#include <libtcod.h>
#include "item.h"

struct map;
struct goal;
struct item;

typedef struct entity
{
	int x; // Position in the map
	int y;
	struct map* host_map; // The real map the entity lives in
	struct map* known_map; // What the entity knows about host_map
	TCOD_path_t path; // The path that the entity is currently following
	TCOD_list_t seen; // Other seen entities;
	TCOD_list_t seen_items;

	TCOD_list_t inventory;
	bool getting_item;
	
	TCOD_list_t goal_stack;

	TCOD_color_t color; // Colour to draw with
	char c; // The character to represent the entity
} entity;

entity* entity_new(int x, int y, char c, TCOD_color_t color);
void entity_delete(entity* e);

bool entity_move(entity* e, int x, int y);
void entity_look(entity* e);

void entity_set_map(entity* e, struct map* m);

bool entity_set_destination(entity* e, int x, int y);
bool entity_follow_path(entity* e);
bool entity_at_destination(entity* e);

void entity_add_goal(entity* e, struct goal* g);
void entity_do_goal(entity* e);

void entity_get_item(entity* e, struct item* i);
void entity_drop_item(entity* e, struct item* i);
bool entity_can_make_item(entity* e, item_type it);
bool entity_make_item(entity* e, item_type it);

bool entity_has_item(entity* e, item* i);
int entity_has_item_type(entity* e, item_type type);

#endif
