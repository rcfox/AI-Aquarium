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

static bool explore_completed(goal* g, entity* e, TCOD_list_t params)
{
	return false;
}

static bool explore_failed(goal* g, entity* e, TCOD_list_t params)
{
	return false;
}

static bool explore_doit(goal* g, entity* e, TCOD_list_t params)
{
	int x,y;
	if(TCOD_list_size(g->subgoals) == 0)
	{
		find_closest_unexplored(e,&x,&y);
		if(x != -1 && y != -1)
		{
			goal_add_subgoal(g,goal_new_move(e,x,y));
		}
	}
	return goal_do_subgoal(g);
}

goal* goal_new_explore(entity* owner)
{
	goal* g = goal_new(owner,&explore_completed,&explore_failed,&explore_doit,NULL);
	sprintf(g->name,"explore");
	return g;
}
