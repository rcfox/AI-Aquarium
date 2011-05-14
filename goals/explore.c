#include <stdio.h>
#include <math.h>
#include "goals/explore.h"
#include "goal.h"
#include "entity.h"
#include "map.h"

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
			if(i < width && i >= 0 && j < height && j >=0)
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
	find_closest_unexplored(e,&x,&y);
	if(x != -1 && y != -1)
	{
		entity_set_destination(e,x,y);
		return true;
	}
	return false;
}

static bool explore_goal(entity* e, TCOD_list_t params)
{
	if(!entity_at_destination(e))
	{
		entity_follow_path(e);
		return false;
	}
	return !pre_explore_goal(e,params);
}

static bool post_explore_goal(entity* e, TCOD_list_t params)
{
	return true;
}

goal* goal_new_explore(entity* owner)
{
	return goal_new(owner,&pre_explore_goal,&explore_goal,&post_explore_goal,NULL);
}
