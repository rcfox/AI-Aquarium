#ifndef EXPLORE_H
#define EXPLORE_H

#include "goals/goals.h"

bool pre_explore_goal(struct entity* e, TCOD_list_t params);
bool explore_goal(struct entity* e, TCOD_list_t params);
bool post_explore_goal(struct entity* e, TCOD_list_t params);

#endif
