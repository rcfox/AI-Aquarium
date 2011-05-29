#include "CuTest.h"
#include "entity.h"
#include "map.h"
#include "goals/pickup_item.h"

#include <libtcod.h>

entity* e;
map* m;

void TestPickupGoalInit(CuTest* tc)
{
	m = map_new(50,50,'.');
	CuAssertPtrNotNull(tc,m);
	e = entity_new(3,3,'@',TCOD_red);
	CuAssertPtrNotNull(tc,e);
	map_add_entity(m,e);
}

void TestPickupGoalComplete(CuTest* tc)
{
	{
		item* i = item_new_rock(3,3);
		item_set_map(i,m);
		goal* g = goal_new_pickup_item(e,i);
		entity_add_goal(e,g);
		CuAssertIntEquals(tc,2,TCOD_list_size(e->goal_stack));
		CuAssertTrue(tc,!g->failed(g,e,g->parameters));
		entity_do_goal(e);
		CuAssertTrue(tc,g->completed(g,e,g->parameters));
		entity_do_goal(e);
		CuAssertIntEquals(tc,1,TCOD_list_size(e->goal_stack));
	}
	{
		item* i = item_new_rock(5,3);
		item_set_map(i,m);
		goal* g = goal_new_pickup_item(e,i);
		entity_add_goal(e,g);
		CuAssertIntEquals(tc,2,TCOD_list_size(e->goal_stack));
		CuAssertTrue(tc,!g->failed(g,e,g->parameters));
		entity_do_goal(e);
		entity_do_goal(e);
		CuAssertIntEquals(tc,5,e->x);
		CuAssertIntEquals(tc,3,e->y);
		entity_do_goal(e);
		CuAssertTrue(tc,g->completed(g,e,g->parameters));
		entity_do_goal(e);
		CuAssertIntEquals(tc,1,TCOD_list_size(e->goal_stack));
	}
	
}

void TestPickupGoalFailed(CuTest* tc)
{
	// Fails due to item not being on the map
	{
		item* j = item_new_rock(3,3);

		// We don't want to do this, but I'm showing that we normally would.
		// If the item isn't on the same map as the entity, the goal fails.
		//item_set_map(j,m); 
		goal* g = goal_new_pickup_item(e,j);
		entity_add_goal(e,g);
		CuAssertTrue(tc,g->failed(g,e,g->parameters));
		entity_do_goal(e);
		CuAssertIntEquals(tc,1,TCOD_list_size(e->goal_stack));
		item_delete(j);
	}

	// Fails due to another entity picking up the item first
	{
		e->x = 3;
		e->y = 3;
		entity* f = entity_new(4,3,'@',TCOD_blue);
		map_add_entity(m,f);
		item* i = item_new_rock(5,3);
		item_set_map(i,m);
		goal* g = goal_new_pickup_item(e,i);
		goal* h = goal_new_pickup_item(f,i);
		entity_add_goal(e,g);
		entity_add_goal(f,h);

		entity_do_goal(e);
		entity_do_goal(f);
		CuAssertIntEquals(tc,5,f->x);
		CuAssertIntEquals(tc,3,f->y);
		CuAssertIntEquals(tc,4,e->x);
		CuAssertIntEquals(tc,3,e->y);
		
		entity_do_goal(e);
		entity_do_goal(f);
		CuAssertIntEquals(tc,5,e->x);
		CuAssertIntEquals(tc,3,e->y);
		CuAssertTrue(tc,h->completed(h,f,h->parameters));
		CuAssertTrue(tc,g->failed(g,e,g->parameters));
		
		entity_do_goal(e);
		entity_do_goal(f);
		CuAssertIntEquals(tc,1,TCOD_list_size(e->goal_stack));
		CuAssertIntEquals(tc,1,TCOD_list_size(f->goal_stack));
	}
}

void TestPickupGoalDelete(CuTest* tc)
{
	entity_delete(e);
	map_delete(m);
}
