#include <stdio.h>
#include "goals/get_item.h"
#include "goals/make_item.h"
#include "goals/pickup_item.h"
#include "goals/move.h"
#include "entity.h"
#include "item.h"
#include "util.h"

static bool get_item_completed(goal* g, entity* e, TCOD_list_t params)
{
	item_type type = (item_type)TCOD_list_get(params,0);
	int how_many = (long)TCOD_list_get(params,1);
	int count = 0;
	foreach(item,e->inventory)
	{
		item* i = *itr;
		if(i->type == type) ++count;
		if(count >= how_many) return true;
	}
	return false;
}

static bool get_item_failed(goal* g, entity* e, TCOD_list_t params)
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

static bool get_item_doit(goal* g, entity* e, TCOD_list_t params)
{
	item_type type = (item_type)TCOD_list_get(params,0);

	if(!e->getting_item)
	{
		foreach(item,e->seen_items)
		{
			item* i = *itr;
			if(i->type == type)
			{
				goal_add_subgoal(g,goal_new_pickup_item(e,i));
				e->getting_item = true;
				break;
			}
		}
	}
	return goal_do_subgoal(g);
}

goal* goal_new_get_item(entity* owner, item_type type, int how_many)
{
	goal* g = goal_new(owner,&get_item_completed,&get_item_failed,&get_item_doit,type,how_many,NULL);
	sprintf(g->name,"get: %s x%d",item_names[type],how_many);
	if(recipes[type][0] != ITEM_nothing)
	{
		goal_add_subgoal(g,goal_new_make_item(owner,type));
	}
	return g;
}
