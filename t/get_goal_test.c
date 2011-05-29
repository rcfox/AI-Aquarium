#include "CuTest.h"
#include "entity.h"
#include "map.h"
#include "goals/get_item.h"
#include "util.h"

#include <libtcod.h>

map* m;

void TestGetGoalInit(CuTest* tc)
{
	m = map_new(50,50,'.');
	CuAssertPtrNotNull(tc,m);
}

void TestGetGoalPickupItem(CuTest* tc)
{
	entity* e = entity_new(3,3,'@',TCOD_red);
	map_add_entity(m,e);
	item* i = item_new_rock(5,3);
	item_set_map(i,m);
	goal* g = goal_new_get_item(e,ITEM_rock,1);
	entity_add_goal(e,g);
	CuAssertIntEquals(tc,2,TCOD_list_size(e->goal_stack));
	entity_do_goal(e);
	CuAssertIntEquals(tc,1,TCOD_list_size(g->subgoals));
	entity_do_goal(e); // Should this be needed twice?
	CuAssertIntEquals(tc,5,e->x);
	CuAssertIntEquals(tc,3,e->y);
	entity_do_goal(e);
	entity_do_goal(e);
	CuAssertTrue(tc,g->completed(g,e,g->parameters));
	entity_do_goal(e);
	entity_do_goal(e);
	CuAssertIntEquals(tc,1,TCOD_list_size(e->goal_stack));
	entity_delete(e);
}

void TestGetGoalPickupCraftableItem(CuTest* tc)
{
	entity* e = entity_new(3,3,'@',TCOD_red);
	map_add_entity(m,e);
	item* i = item_new_crude_hammer(5,3);
	item_set_map(i,m);
	goal* g = goal_new_get_item(e,ITEM_crude_hammer,1);
	entity_add_goal(e,g);
	CuAssertIntEquals(tc,2,TCOD_list_size(e->goal_stack));
	entity_do_goal(e);
	CuAssertIntEquals(tc,2,TCOD_list_size(g->subgoals)); // pickup and make
	entity_do_goal(e); // Should this be needed twice?
	CuAssertIntEquals(tc,5,e->x);
	CuAssertIntEquals(tc,3,e->y);
	entity_do_goal(e);
	entity_do_goal(e);
	CuAssertTrue(tc,g->completed(g,e,g->parameters));
	entity_do_goal(e);
	entity_do_goal(e);
	CuAssertIntEquals(tc,1,TCOD_list_size(e->goal_stack));
	entity_delete(e);
}

void TestGetGoalCraftItem(CuTest* tc)
{
	entity* e = entity_new(3,3,'@',TCOD_red);
	map_add_entity(m,e);
	item* i1 = item_new_rock(4,3);
	item_set_map(i1,m);
	item* i2 = item_new_branch(5,3);
	item_set_map(i2,m);
	goal* g = goal_new_get_item(e,ITEM_crude_hammer,1);
	entity_add_goal(e,g);
	CuAssertIntEquals(tc,2,TCOD_list_size(e->goal_stack));
	entity_do_goal(e);
	entity_do_goal(e);
	CuAssertTrue(tc, entity_has_item(e,i1));
	entity_do_goal(e);
	CuAssertIntEquals(tc,5,e->x);
	CuAssertIntEquals(tc,3,e->y);
	entity_do_goal(e);
	entity_do_goal(e);
	CuAssertTrue(tc, entity_has_item(e,i2));
	entity_do_goal(e);
	entity_do_goal(e);
	CuAssertTrue(tc,g->completed(g,e,g->parameters));
	CuAssertTrue(tc, entity_has_item_type(e,ITEM_crude_hammer));
	entity_do_goal(e);
	entity_do_goal(e);
	CuAssertIntEquals(tc,1,TCOD_list_size(e->goal_stack));
	entity_delete(e);
}

void TestGetGoalFailed(CuTest* tc)
{
	// Never fails?
}

void TestGetGoalDelete(CuTest* tc)
{
	map_delete(m);
}
