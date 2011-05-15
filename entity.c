#include <stdlib.h>
#include <libtcod.h>
#include <assert.h>
#include "entity.h"
#include "map.h"
#include "item.h"
#include "util.h"
#include "goal.h"
#include "goals/nothing.h"

entity* entity_new(int x, int y, char c, TCOD_color_t color)
{
	entity* e = malloc(sizeof(entity));
	e->x = x;
	e->y = y;
	e->c = c;
	e->color = color;
	e->host_map = NULL;
	e->known_map = NULL;
	e->path = NULL;
	e->seen = TCOD_list_new();
	e->seen_items = TCOD_list_new();
	e->goal_stack = TCOD_list_new();
	e->inventory = TCOD_list_new();

	// Make sure the entity always has something to do so that we don't crash!
	entity_add_goal(e,goal_new_nothing(e));
	return e;
}

void entity_delete(entity* e)
{
	foreach(item,e->inventory)
	{
		item_delete(*itr);
	}
	foreach(goal,e->goal_stack)
	{
		goal_delete(*itr);
	}
	if(e->path) TCOD_path_delete(e->path);
	if(e->known_map)
	{
		// The known_map doesn't own the items, so we don't want to
		// delete the items twice!
		TCOD_list_clear(e->known_map->items);
		map_delete(e->known_map);
	}
	TCOD_list_delete(e->seen);
	TCOD_list_delete(e->seen_items);
	TCOD_list_delete(e->goal_stack);
	TCOD_list_delete(e->inventory);
	free(e);
}

void entity_set_map(entity* e, map* m)
{
	e->host_map = m;
	if(e->known_map) map_delete(e->known_map);
	if(e->path) TCOD_path_delete(e->path);
	if(m)
	{
		e->known_map = map_new(TCOD_map_get_width(m->data),TCOD_map_get_height(m->data),' ');
		e->path = TCOD_path_new_using_map(e->known_map->data,1.41f);
	}
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
	TCOD_list_t nearby = TCOD_list_new();
	foreach(entity,e->host_map->entities)
	{
		entity* o = *itr;
		int dx = (e->x - o->x);
		int dy = (e->y - o->y);
		if(dx*dx+dy*dy <= radius*radius)
		{
			TCOD_list_push(nearby,o);
		}
	}
	TCOD_list_clear(e->seen);
	TCOD_list_t nearby_items = TCOD_list_new();
	foreach(item,e->host_map->items)
	{
		item* o = *itr;
		int dx = (e->x - o->x);
		int dy = (e->y - o->y);
		if(dx*dx+dy*dy <= radius*radius)
		{
			TCOD_list_push(nearby_items,o);
		}
	}
	TCOD_list_clear(e->seen_items);
	
	TCOD_map_compute_fov(e->host_map->data,e->x,e->y,radius,1,FOV_BASIC);
	
	for(int y = -radius; y <= radius; ++y)
	{
		for(int x = -radius; x <= radius; ++x)
		{
			if(TCOD_map_is_in_fov(e->host_map->data,e->x+x,e->y+y))
			{				
				map_copy_data(e->host_map,e->known_map,e->x+x,e->y+y);
				
				foreach(entity,nearby)
				{
					if((*itr)->x == x+e->x && (*itr)->y == y+e->y)
					{
						TCOD_list_push(e->seen,*itr);
					}
				}
				foreach(item,nearby_items)
				{
					if((*itr)->x == x+e->x && (*itr)->y == y+e->y)
					{
						TCOD_list_push(e->seen_items,*itr);
					}
				}
			}
		}
	}
	TCOD_list_delete(nearby);
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

void entity_add_goal(entity* e, struct goal* g)
{
	TCOD_list_push(e->goal_stack,g);
	foreach(goal,g->subgoals)
	{
		entity_add_goal(e,*itr);
	}
}

void entity_do_goal(entity* e)
{
	goal_do(TCOD_list_peek(e->goal_stack));
}

void entity_get_item(entity* e, item* i)
{
	assert(i->owner == NULL);
	assert(e->x == i->x && e->y == i->y);

	map_remove_item(e->host_map,i);
	TCOD_list_push(e->inventory,i);
	item_set_owner(i,e);
}

void entity_drop_item(entity* e, item* i)
{
	i->x = e->x;
	i->y = e->y;
	TCOD_list_remove_fast(e->inventory,i);
	item_set_map(i,e->host_map);
}

void entity_make_item(entity* e, item_type it)
{
	bool have_requirements = true;
	item* components[MAX_RECIPE_COMPONENTS] = {NULL};
	for(int i = 0; i < MAX_RECIPE_COMPONENTS; ++i)
	{
		if(recipes[it][i] == ITEM_nothing)
		{
			components[i] = NULL;
		}
		else
		{
			foreach(item,e->inventory)
			{
				item* item = *itr;
				if(item->type == recipes[it][i])
				{
					components[i] = item;
					TCOD_list_remove_fast(e->inventory,item);
					break;
				}
			}
			if(components[i] == NULL)
			{
				have_requirements = false;
				break;
			}
		}
	}
	if(have_requirements)
	{
		item* new_item = item_constructors[it](e->x,e->y);
		TCOD_list_push(e->inventory,new_item);
		item_set_owner(new_item,e);
		for(int i = 0; i < MAX_RECIPE_COMPONENTS; ++i)
		{
			if(components[i])
			{
				item_delete(components[i]);
			}
		}
	}
	else
	{
		for(int i = 0; i < MAX_RECIPE_COMPONENTS; ++i)
		{
			if(components[i])
			{
				TCOD_list_push(e->inventory,components[i]);
			}
		}
	}
}
