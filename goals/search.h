#ifndef GOAL_SEARCH_H
#define GOAL_SEARCH_H

#include "goals/goals.h"
#include "item.h"

goal* goal_new_search(struct entity* owner, item_type type, int how_many);

#endif
