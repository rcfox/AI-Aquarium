#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <libtcod.h>
#include <assert.h>
#include <string.h>
#include "goal.h"
#include "entity.h"
#include "util.h"

goal* goal_new(struct entity* owner, goal_func completed, goal_func failed, goal_func doit, ...)
{
	goal* g = malloc(sizeof(goal));
	g->name[0] = '\0';
	g->owner = owner;
	g->first_time = true;
	g->completed = completed;
	g->failed = failed;
	g->doit = doit;
	g->parameters = TCOD_list_new();
	g->subgoals = TCOD_list_new();

	va_list ap;
	void* p;
	va_start(ap,doit);
	while((p = va_arg(ap,void*)))
	{
		TCOD_list_push(g->parameters,p);
	}
	va_end(ap);

	return g;
}

void goal_delete(goal* g)
{
	foreach(goal,g->subgoals)
	{
		goal_delete(*itr);
	}
	TCOD_list_delete(g->subgoals);
	TCOD_list_delete(g->parameters);
	free(g);
}

void goal_add_subgoal(goal* g, goal* subgoal)
{
	TCOD_list_push(g->subgoals,subgoal);
}

void goal_remove_subgoal(goal* g, goal* subgoal)
{
	
}

bool goal_do(goal* g)
{
	bool progressed = false;
	assert(g);
	if(g->completed(g,g->owner,g->parameters))
	{
		goal_delete(TCOD_list_pop(g->owner->goal_stack));
		return true;
	}

	progressed = g->doit(g,g->owner,g->parameters);

	if(!progressed)
	{
		goal* tmp = TCOD_list_pop(g->owner->goal_stack);
		entity_do_goal(g->owner);
		TCOD_list_push(g->owner->goal_stack,tmp);
	}	

	return false;
}

bool goal_do_subgoal(goal* g)
{
	foreach(goal,g->subgoals)
	{
		goal* next = *itr;
		if(next)
		{
			if(next->completed(next,next->owner,next->parameters) || next->failed(next,next->owner,next->parameters))
			{
				itr = (goal**)TCOD_list_remove_iterator(g->subgoals,(void**)itr);
				goal_delete(next);
			}
			else
			{
				if(next->doit(next,next->owner,next->parameters)) return true;
			}
		}
	}
	return false;
}

static void _display(goal* g, int depth)
{
	for(int i = 0; i < depth; ++i)
	{
		printf("  ");
	}
	printf("%s\n",g->name);
	foreach(goal,g->subgoals)
	{
		_display(*itr,depth+1);
	}
}

void goal_display(goal* g)
{
	_display(g,0);
}
