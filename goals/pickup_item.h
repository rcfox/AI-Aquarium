#ifndef GOALS_PICKUP_ITEM_H
#define GOALS_PICKUP_ITEM_H

#include "goals/goals.h"

struct item;

goal* goal_new_pickup_item(struct entity* owner, struct item* item);

#endif
