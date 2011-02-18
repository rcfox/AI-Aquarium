#include <stdlib.h>
#include "entity.h"

entity* create_entity(map* m, int x, int y, char c, TCOD_color_t color)
{
	entity* e = malloc(sizeof(entity));
	e->x = x;
	e->y = y;
	e->c = c;
	e->color = color;
	create_map(&(e->map),TCOD_map_get_width(m->data),TCOD_map_get_height(m->data));
	return e;
}

void free_entity(entity* e)
{
	free_map(&(e->map));
	free(e);
}
