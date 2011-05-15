#include <stdlib.h>
#include "item.h"
#include "entity.h"
#include "map.h"

item_type recipes[][MAX_RECIPE_COMPONENTS] =
{
	// Clean up for empty slots
#define ITEM_ ITEM_nothing
	// Ugly hack to get around the lack of recursive macros
#define RECIPE7(a,b...) ITEM_##a
#define RECIPE6(a,b...) ITEM_##a,RECIPE7(b)
#define RECIPE5(a,b...) ITEM_##a,RECIPE6(b)
#define RECIPE4(a,b...) ITEM_##a,RECIPE5(b)
#define RECIPE3(a,b...) ITEM_##a,RECIPE4(b)
#define RECIPE2(a,b...) ITEM_##a,RECIPE3(b)
#define RECIPE1(a,b...) ITEM_##a,RECIPE2(b)
	// Since recipe is a variadic argument, we need to operate on each element.
#define EXPAND_RECIPE(a,b...) ITEM_##a,RECIPE1(b)
#define DEFINE_ITEM(name,character,colour,recipe...) [ITEM_##name] = {EXPAND_RECIPE(recipe)},
#include "items.def"
#undef DEFINE_ITEM
};

item* item_new(const char* name, int x, int y, char c, TCOD_color_t color, item_type type, item_func effect)
{
	item* i = malloc(sizeof(item));
	i->name = name;
	i->x = x;
	i->y = y;
	i->c = c;
	i->color = color;
	i->type = type;
	i->use_effect = effect;
	
	i->host_map = NULL;
	i->owner = NULL;

	return i;
}

void item_delete(item* i)
{
	free(i);
}

void item_draw(item* i, TCOD_console_t console)
{
	TCOD_console_put_char_ex(console,i->x,i->y,i->c,i->color,TCOD_black);
}

void item_set_map(item* i, map* m)
{
	map_add_item(m,i);
	i->host_map = m;
	i->owner = NULL;
}

void item_set_owner(item* i, entity* e)
{
	i->owner = e;
	i->host_map = NULL;
}

// Make a "new" function for each item type.
#define DEFINE_ITEM(name,character,colour,recipe...)	  \
	item* item_new_##name(int x, int y) \
	{ \
		return item_new(#name,x,y,character,TCOD_##colour,ITEM_##name,NULL); \
	}
#include "items.def"
#undef DEFINE_ITEM

item_constructor item_constructors[] =
{
#define DEFINE_ITEM(name,character,colour,recipe...) [ITEM_##name] = &item_new_##name,
#include "items.def"
#undef DEFINE_ITEM
};
