#ifndef GOAL_H
#define GOAL_H

#include <libtcod.h>

struct entity;
struct goal;

typedef bool (*goal_func)(struct goal* g, struct entity* owner, TCOD_list_t params);

typedef struct goal
{
	char name[128];
	struct entity* owner;
	goal_func completed;
	goal_func failed;
	goal_func doit;
	bool first_time;
	TCOD_list_t parameters;
	TCOD_list_t subgoals;
} goal;

goal* goal_new(struct entity* owner, goal_func completed, goal_func failed, goal_func doit, ...);
void goal_delete(goal* g);

void goal_add_subgoal(goal* g, goal* subgoal);

bool goal_do(goal* g);
bool goal_do_subgoal(goal* g);

void goal_display(goal* g);

#endif
