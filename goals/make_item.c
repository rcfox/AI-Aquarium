#include <stdio.h>
#include "goals/make_item.h"
#include "goals/search.h"
#include "entity.h"
#include "item.h"

static bool pre_make_item_goal(entity* e, TCOD_list_t params)
{
	return true;
}

static bool make_item_goal(entity* e, TCOD_list_t params)
{
	item_type type = (item_type)TCOD_list_get(params,0);
	entity_make_item(e,type);
	return true;
}

static bool post_make_item_goal(entity* e, TCOD_list_t params)
{
	return true;
}

goal* goal_new_make_item(struct entity* owner, item_type type)
{
	int components[NUM_ITEM_TYPES] = {0};
	for(int i = 0; i < MAX_RECIPE_COMPONENTS; ++i)
	{
		++components[recipes[type][i]];
	}
	goal* g = goal_new("make_item",owner,&pre_make_item_goal,&make_item_goal,&post_make_item_goal,type,NULL);
	for(int i = 1; i < NUM_ITEM_TYPES; ++i)
	{
		if(components[i] > 0)
		{
			goal_add_subgoal(g,goal_new_search(owner,i,components[i]));
		}
	}
	return g;
}
