#include <stdlib.h>
#include "item.h"
#include "entity.h"
#include "map.h"

item* item_new(int x, int y, char c, TCOD_color_t color, item_type type, item_func effect)
{
	item* i = malloc(sizeof(item));
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
