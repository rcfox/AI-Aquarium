#include <stdio.h>
#include <assert.h>
#include "goals/make_item.h"
#include "goals/get_item.h"
#include "entity.h"
#include "item.h"
#include "util.h"

static bool make_item_completed(goal* g, entity* e, TCOD_list_t params)
{
	item_type type = (item_type)TCOD_list_get(params,0);

	if(entity_make_item(e,type))
	{
//		printf("Make a %s\n",item_names[type]);
		return true;
	}
	return false;
}

static bool make_item_failed(goal* g, entity* e, TCOD_list_t params)
{
	return false;
	bool failed = true;
	foreach(goal,g->subgoals)
	{
		goal* f = (*itr);
		failed &= f->failed(f,f->owner,f->parameters);
	}
	return failed;
}

static bool make_item_doit(goal* g, entity* e, TCOD_list_t params)
{
	item_type type = (item_type)TCOD_list_get(params,0);
	if(TCOD_list_size(g->subgoals) == 0 && !entity_can_make_item(e,type))
	{
		int components[NUM_ITEM_TYPES] = {0};
		for(int i = 0; i < MAX_RECIPE_COMPONENTS; ++i)
		{
			++components[recipes[type][i]];
		}
		for(int i = 1; i < NUM_ITEM_TYPES; ++i)
		{
			if(components[i] > 0)
			{
				goal_add_subgoal(g,goal_new_get_item(e,i,components[i]));
			}
		}
	}
	return goal_do_subgoal(g);
}

goal* goal_new_make_item(struct entity* owner, item_type type)
{
	int components[NUM_ITEM_TYPES] = {0};
	for(int i = 0; i < MAX_RECIPE_COMPONENTS; ++i)
	{
		++components[recipes[type][i]];
	}
	goal* g = goal_new(owner,&make_item_completed,&make_item_failed,&make_item_doit,type,NULL);
	sprintf(g->name,"make item: %s",item_names[type]);
	for(int i = 1; i < NUM_ITEM_TYPES; ++i)
	{
		if(components[i] > 0)
		{
			goal_add_subgoal(g,goal_new_get_item(owner,i,components[i]));
		}
	}
	return g;
}
