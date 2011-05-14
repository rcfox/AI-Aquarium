#include <stdio.h>
#include "goals/get_item.h"
#include "goals/move.h"
#include "entity.h"
#include "item.h"

static bool pre_get_item_goal(entity* e, TCOD_list_t params)
{
	item* i = TCOD_list_get(params,0);
	if(e->x == i->x && e->y == i->y) return true;
	
	entity_add_goal(e,goal_new_move(e,i->x,i->y));
	return false;
}

static bool get_item_goal(entity* e, TCOD_list_t params)
{
	item* i = TCOD_list_get(params,0);
	if(i->host_map == e->host_map && (e->x == i->x && e->y == i->y))
	{
		entity_get_item(e,i);
	}
	// I feel like there should be a way to convey complete failure of this goal.
	return true;
}

static bool post_get_item_goal(entity* e, TCOD_list_t params)
{
	return true;
}

goal* goal_new_get_item(entity* owner, item* item)
{
	return goal_new(owner,&pre_get_item_goal,&get_item_goal,&post_get_item_goal,item,NULL);
}
