#include <stdlib.h>
#include "entity.h"

entity* entity_new(map* m, int x, int y, char c, TCOD_color_t color)
{
	entity* e = malloc(sizeof(entity));
	e->x = x;
	e->y = y;
	e->c = c;
	e->color = color;
	e->host_map = m;
	e->known_map = map_new(TCOD_map_get_width(m->data),TCOD_map_get_height(m->data),' ');
	e->path = TCOD_path_new_using_map(e->known_map->data,1.41f);
	return e;
}

void entity_delete(entity* e)
{
	TCOD_path_delete(e->path);
	map_delete(e->known_map);
	free(e);
}

void entity_draw(entity* e, TCOD_console_t console)
{
	TCOD_console_put_char_ex(console,e->x,e->y,e->c,e->color,TCOD_black);
}

bool entity_move(entity* e, int x, int y)
{
	if(!TCOD_map_is_walkable(e->host_map->data,x,y)) { return false; }

	e->x = x;
	e->y = y;

	return true;
}

void entity_look(entity* e)
{
	int radius = 5;
	TCOD_map_compute_fov(e->host_map->data,e->x,e->y,radius,1,FOV_BASIC);
	for(int y = -radius; y < radius; ++y)
	{
		for(int x = -radius; x < radius; ++x)
		{
			if(TCOD_map_is_in_fov(e->host_map->data,e->x+x,e->y+y))
			{				
				map_copy_data(e->host_map,e->known_map,e->x+x,e->y+y);
			}
		}
	}
}

bool entity_set_destination(entity* e, int x, int y)
{
	return TCOD_path_compute(e->path,e->x,e->y,x,y);
}

bool entity_follow_path(entity* e)
{
	int x,y;
	bool ret = TCOD_path_walk(e->path,&x,&y,1);
	if(ret)
	{
		ret &= entity_move(e,x,y);
	}
	return ret;
}

bool entity_at_destination(entity* e)
{
	return TCOD_path_is_empty(e->path);
}
