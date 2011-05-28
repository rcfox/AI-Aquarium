#include "CuTest.h"
#include "entity.h"
#include "map.h"
#include "goals/move.h"

#include <libtcod.h>

entity* e;
map* m;

void TestMoveGoalInit(CuTest* tc)
{
	m = map_new(50,50,'.');
	CuAssertPtrNotNull(tc,m);
	e = entity_new(3,3,'@',TCOD_red);
	CuAssertPtrNotNull(tc,e);
	map_add_entity(m,e);

	// Everyone should have a "nothing" goal.
	CuAssertIntEquals(tc,1,TCOD_list_size(e->goal_stack));
   
	for(int i = 0; i < 10; ++i)
	{
		map_set_data(m,i,5,'#',TCOD_grey,false,false);
	}
}

void TestMoveGoalComplete(CuTest* tc)
{
	goal* g = goal_new_move(e,3,3);
	entity_add_goal(e,g);
	CuAssertIntEquals(tc,2,TCOD_list_size(e->goal_stack));
	CuAssertTrue(tc,g->completed(g,e,g->parameters));
	CuAssertTrue(tc,!g->failed(g,e,g->parameters));
	entity_do_goal(e);
	CuAssertIntEquals(tc,1,TCOD_list_size(e->goal_stack));

	g = goal_new_move(e,5,3);
	entity_add_goal(e,g);
	CuAssertTrue(tc,!g->completed(g,e,g->parameters));
	CuAssertTrue(tc,!g->failed(g,e,g->parameters));
	entity_do_goal(e);
	entity_do_goal(e);
	CuAssertTrue(tc,g->completed(g,e,g->parameters));
	entity_do_goal(e);
	CuAssertIntEquals(tc,1,TCOD_list_size(e->goal_stack));
	CuAssertIntEquals(tc,5,e->x);
	CuAssertIntEquals(tc,3,e->y);
}

void TestMoveGoalFailed(CuTest* tc)
{
	goal* g = goal_new_move(e,3,5);
	entity_add_goal(e,g);
	CuAssertIntEquals(tc,2,TCOD_list_size(e->goal_stack));
	CuAssertTrue(tc,!g->completed(g,e,g->parameters));
	CuAssertTrue(tc,!g->failed(g,e,g->parameters));
	entity_do_goal(e);
	entity_do_goal(e);
	CuAssertTrue(tc,g->failed(g,e,g->parameters));
	entity_do_goal(e);
	CuAssertIntEquals(tc,1,TCOD_list_size(e->goal_stack));
}

void TestMoveGoalDelete(CuTest* tc)
{
	entity_delete(e);
	map_delete(m);
}
