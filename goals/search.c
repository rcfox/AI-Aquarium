#include <libtcod.h>
#include <stdlib.h>
#include <stdio.h>
#include "goals/search.h"
#include "goals/get_item.h"
#include "goals/move.h"
#include "entity.h"
#include "item.h"
#include "util.h"

static bool search_goal(entity* e, TCOD_list_t params)
{
	item_type type = (item_type)TCOD_list_get(params,0);
	int how_many = (long)TCOD_list_get(params,1);
	int count = 0;
	foreach(item,e->inventory)
	{
		item* i = *itr;
		if(i->type == type) ++count;
	}
	if(count >= how_many) return true;
	
	foreach(item,e->seen_items)
	{
		item* i = *itr;
		if(i->type == type)
		{
			entity_add_goal(e,goal_new_get_item(e,i));
			return false;
		}
	}

	entity_add_goal(e,goal_new_move(e,e->x+5*(-1+rand()%3),e->y+5*(-1+rand()%3)));
	return false;
}

goal* goal_new_search(struct entity* owner, item_type type, int how_many)
{
	return goal_new("search",owner,NULL,&search_goal,NULL,type,how_many,NULL);
}
