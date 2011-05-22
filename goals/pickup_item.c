#include <stdio.h>
#include "goals/pickup_item.h"
#include "goals/move.h"
#include "entity.h"
#include "item.h"
#include "util.h"

static bool pickup_item_completed(goal* g, entity* e, TCOD_list_t params)
{
	item* i = TCOD_list_get(params,0);
	return (i->owner == e);
}

static bool pickup_item_failed(goal* g, entity* e, TCOD_list_t params)
{
	item* i = TCOD_list_get(params,0);
	return (i->host_map != e->host_map && i->owner != e);
}

static bool pickup_item_doit(goal* g, entity* e, TCOD_list_t params)
{
	item* i = TCOD_list_get(params,0);
	if(i->host_map == e->host_map && (e->x == i->x && e->y == i->y))
	{
		entity_get_item(e,i);
		e->getting_item = false;
		return true;
	}
	else
	{
		return goal_do_subgoal(g);
	}
}

goal* goal_new_pickup_item(entity* owner, item* item)
{
	goal* g = goal_new(owner,&pickup_item_completed,&pickup_item_failed,&pickup_item_doit,item,NULL);
	sprintf(g->name,"pickup %s at (%d,%d)",item_names[item->type],item->x,item->y);
	goal_add_subgoal(g,goal_new_move(owner,item->x,item->y));
	return g;
}
