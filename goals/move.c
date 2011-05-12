#include <stdio.h>
#include "goals/move.h"
#include "entity.h"

bool pre_move_goal(entity* e, TCOD_list_t params)
{
	int x = (long)TCOD_list_get(params,0);
	int y = (long)TCOD_list_get(params,1);
	entity_set_destination(e,x,y);
	return true;
}

bool move_goal(entity* e, TCOD_list_t params)
{
	if(!entity_at_destination(e))
	{
		entity_follow_path(e);
		return false;
	}
	return true;
}

bool post_move_goal(entity* e, TCOD_list_t params)
{
	int x = (long)TCOD_list_get(params,0);
	int y = (long)TCOD_list_get(params,1);
	printf("Arrived at (%d,%d)!\n",x,y);
	return true;
}
