#ifndef ITEM_H
#define ITEM_H

#include <libtcod.h>

struct entity;
struct map;
struct item;

typedef void (*item_func)(struct item* item, TCOD_list_t params);

typedef enum item_type
{
	// Make an enum entry for each item type.
#define DEFINE_ITEM(name,char,colour,recipe...) ITEM_##name,
#include "items.def"
#undef DEFINE_ITEM
} item_type;

typedef struct item
{
	int x; // Position in the map
	int y;
	struct map* host_map;
	struct entity* owner;

	item_type type;
	item_func use_effect;

	const char* name;
	TCOD_color_t color; // Colour to draw with
	char c; // The character to represent the item
} item;

item* item_new(const char* name, int x, int y, char c, TCOD_color_t color, item_type type, item_func effect);
void item_delete(item* i);

void item_draw(item* i, TCOD_console_t console);

void item_set_map(item* i, struct map* m);
void item_set_owner(item* i, struct entity* e);

// Define a "new" function for each item type.
#define DEFINE_ITEM(name,char,colour,recipe...) item* item_new_##name(int x, int y);
#include "items.def"
#undef DEFINE_ITEM

#endif
