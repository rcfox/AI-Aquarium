#include <stdio.h>
#include <libtcod.h>
#include "goals/move.h"
#include "entity.h"

static bool move_completed(goal* g, entity* e, TCOD_list_t params)
{
	int x = (long)TCOD_list_get(params,0);
	int y = (long)TCOD_list_get(params,1);
	return (e->x == x && e->y == y);
}

static bool move_failed(goal* g, entity* e, TCOD_list_t params)
{
	int x = (long)TCOD_list_get(params,0);
	int y = (long)TCOD_list_get(params,1);

	TCOD_console_put_char_ex(NULL,x,y,'X',TCOD_red,TCOD_black);
	
	return (!g->first_time && (e->x != x || e->y != y) && TCOD_path_size(e->path) == 0 );
}

static bool move_doit(goal* g, entity* e, TCOD_list_t params)
{
	int x = (long)TCOD_list_get(params,0);
	int y = (long)TCOD_list_get(params,1);
	if(g->first_time)
	{
		entity_set_destination(e,x,y);
		g->first_time = false;
	}
	entity_follow_path(e);
	return true;
}

goal* goal_new_move(struct entity* owner, int x, int y)
{
	goal* g = goal_new(owner,&move_completed,&move_failed,&move_doit,x,y,NULL);
	sprintf(g->name,"move from (%d,%d) to (%d,%d)",owner->x,owner->y,x,y);
	return g;
}
