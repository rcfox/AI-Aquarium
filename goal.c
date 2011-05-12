#include <stdlib.h>
#include <stdarg.h>
#include <libtcod.h>
#include <assert.h>
#include "goal.h"
#include "entity.h"

goal* goal_new(struct entity* e, goal_func pre, goal_func gf, goal_func post, ...)
{
	goal* g = malloc(sizeof(goal));	
	g->owner = e;
	g->pre_goal = pre;
	g->goal_func = gf;
	g->post_goal = post;
	g->parameters = TCOD_list_new();
	g->first_time = true;

	va_list ap;
	void* p;
	va_start(ap,post);
	while((p = va_arg(ap,void*)))
	{
		TCOD_list_push(g->parameters,p);
	}
	va_end(ap);

	return g;
}

void goal_delete(goal* g)
{
	TCOD_list_delete(g->parameters);
	free(g);
}

bool goal_do(goal* g)
{
	bool ret = true;
	if(!g) return false;
	if(g->first_time && g->pre_goal)
	{
		ret &= g->pre_goal(g->owner,g->parameters);
		g->first_time = false;
	}
	if(ret)
	{
		ret &= g->goal_func(g->owner,g->parameters);
	}
	if(ret && g->post_goal)
	{
		ret &= g->post_goal(g->owner,g->parameters);
		
		if(ret)
		{
			g->owner->goal = NULL;
			goal_delete(g);
		}
	}
	return ret;
}
