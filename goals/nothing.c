#include <stdio.h>
#include "goals/nothing.h"
#include "goal.h"
#include "entity.h"

static bool nothing_completed(goal* g, entity* e, TCOD_list_t params)
{
	return false;
}

static bool nothing_failed(goal* g, entity* e, TCOD_list_t params)
{
	return false;
}

static bool nothing_doit(goal* g, entity* e, TCOD_list_t params)
{
	return true;
}

goal* goal_new_nothing(entity* owner)
{
	goal* g = goal_new(owner,&nothing_completed,&nothing_failed,&nothing_doit,NULL);
	sprintf(g->name,"nothing");
	return g;
}
