#include <stdio.h>
#include <math.h>
#include "goals/explore.h"
#include "goals/pickup_item.h"
#include "goals/move.h"
#include "goal.h"
#include "entity.h"
#include "map.h"
#include "item.h"
#include "util.h"

static void find_closest_unexplored(entity* e, int* x, int* y)
{
	int width = TCOD_map_get_width(e->known_map->data);
	int height = TCOD_map_get_height(e->known_map->data);
	for(int r = 1; r < width; ++r)
	{
		for(float a = 0; a < 2*M_PI; a += 0.01)
		{
			int i = e->x + r*cos(a);
			int j = e->y + r*sin(a);
			if(i < width && i > 0 && j < height && j > 0)
			{
				if(e->known_map->display[i+width*j].c == ' ')
				{
					*x = i;
					*y = j;
					return;
				}
			}
		}
	}
	*x = -1;
	*y = -1;	
}

static bool pre_explore_goal(entity* e, TCOD_list_t params)
{
	int x,y;
	bool found_item = false;
	foreach(item,e->seen_items)
	{
		entity_add_goal(e,goal_new_pickup_item(e,*itr));
		found_item = true;
	}
	if(found_item) return true;
	
	find_closest_unexplored(e,&x,&y);
	if(x != -1 && y != -1)
	{
		entity_add_goal(e,goal_new_move(e,x,y));
		return true;
	}
	return false;
}

static bool explore_goal(entity* e, TCOD_list_t params)
{
	return !pre_explore_goal(e,params);
}

static bool post_explore_goal(entity* e, TCOD_list_t params)
{
	return true;
}

goal* goal_new_explore(entity* owner)
{
	return goal_new("explore",owner,&pre_explore_goal,&explore_goal,&post_explore_goal,NULL);
}
