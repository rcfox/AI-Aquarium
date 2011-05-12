#ifndef MOVE_H
#define MOVE_H

#include "goals/goals.h"

bool pre_move_goal(struct entity* e, TCOD_list_t params);
bool move_goal(struct entity* e, TCOD_list_t params);
bool post_move_goal(struct entity* e, TCOD_list_t params);

#endif
