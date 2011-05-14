#ifndef GOAL_H
#define GOAL_H

#include <libtcod.h>

struct entity;

typedef bool (*goal_func)(struct entity* owner, TCOD_list_t params);

typedef struct goal
{
	struct entity* owner;
	goal_func pre_goal;
	goal_func goal_func;
	goal_func post_goal;
	bool first_time;
	TCOD_list_t parameters;
	TCOD_list_t subgoals;
} goal;

goal* goal_new(struct entity* owner, goal_func pre_goal, goal_func goal, goal_func post_goal, ...);
void goal_delete(goal* g);

void goal_add_subgoal(goal* g, goal* subgoal);

bool goal_do(goal* g);

#endif
