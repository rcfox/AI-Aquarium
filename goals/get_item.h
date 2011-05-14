#ifndef GOALS_GET_ITEM_H
#define GOALS_GET_ITEM_H

#include "goals/goals.h"

struct item;

goal* goal_new_get_item(struct entity* owner, struct item* item);

#endif
