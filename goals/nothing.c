#include "goals/nothing.h"
#include "goal.h"
#include "entity.h"

static bool pre_nothing_goal(entity* e, TCOD_list_t params)
{
	return true;
}

static bool nothing_goal(entity* e, TCOD_list_t params)
{
	return false;
}

static bool post_nothing_goal(entity* e, TCOD_list_t params)
{
	return true;
}

goal* goal_new_nothing(entity* owner)
{
	return goal_new("nothing",owner,&pre_nothing_goal,&nothing_goal,&post_nothing_goal,NULL);
}
