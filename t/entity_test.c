#include "CuTest.h"
#include "entity.h"
#include "map.h"
#include "item.h"

#include <libtcod.h>

entity* e;
map* m;

void TestEntityCreation(CuTest* tc)
{
	m = map_new(50,50,'.');
	e = entity_new(3,3,'@',TCOD_red);
	CuAssertPtrNotNull(tc,e);
	CuAssertIntEquals(tc,3,e->x);
	CuAssertIntEquals(tc,3,e->y);
	CuAssertIntEquals(tc,'@',e->c);
	CuAssertTrue(tc,TCOD_color_equals(e->color,TCOD_red));
	CuAssertTrue(tc,TCOD_list_size(e->goal_stack) > 0);

	map_add_entity(m,e);
	CuAssertPtrEquals(tc,m,e->host_map);
	CuAssertTrue(tc,TCOD_list_contains(m->entities,e));
	int host_width = TCOD_map_get_width(m->data);
	int host_height = TCOD_map_get_height(m->data);
	int known_width = TCOD_map_get_width(e->known_map->data);
	int known_height = TCOD_map_get_height(e->known_map->data);
	CuAssertIntEquals(tc,host_width,known_width);
	CuAssertIntEquals(tc,host_height,known_height);
}

void TestEntityLookEntities(CuTest* tc)
{
	// TODO: Put more effort into testing the edge cases
	entity* x[] = {entity_new(e->x,e->y,'@',TCOD_blue),
	               entity_new(e->x+1,e->y,'@',TCOD_blue),
	               entity_new(e->x+20,e->y,'@',TCOD_blue),
	               entity_new(e->x,e->y+1,'@',TCOD_blue),
	               entity_new(e->x+1,e->y+20,'@',TCOD_blue),
	               entity_new(e->x+5,e->y+5,'@',TCOD_blue)};
	for(int i = 0; i < sizeof(x)/sizeof(entity*); ++i)
	{
		map_add_entity(m,x[i]);
	}

	entity_look(e);
	CuAssertIntEquals(tc,4,TCOD_list_size(e->seen));
	CuAssertTrue(tc,TCOD_list_contains(e->seen,e)); // Self
	CuAssertTrue(tc,TCOD_list_contains(e->seen,x[0]));
	CuAssertTrue(tc,TCOD_list_contains(e->seen,x[1]));
	CuAssertTrue(tc,TCOD_list_contains(e->seen,x[3]));
	
	CuAssertTrue(tc,!TCOD_list_contains(e->seen,x[2]));
	CuAssertTrue(tc,!TCOD_list_contains(e->seen,x[4]));
	CuAssertTrue(tc,!TCOD_list_contains(e->seen,x[5]));

	for(int i = 0; i < sizeof(x)/sizeof(entity*); ++i)
	{
		entity_delete(x[i]);
	}
}

void TestEntityLookItems(CuTest* tc)
{
	item* x[] = {item_new_rock(e->x,e->y),
	             item_new_rock(e->x+1,e->y),
	             item_new_rock(e->x+20,e->y),
	             item_new_rock(e->x,e->y+1),
	             item_new_rock(e->x+1,e->y+20),
	             item_new_rock(e->x+5,e->y+5)};
	for(int i = 0; i < sizeof(x)/sizeof(item*); ++i)
	{
		item_set_map(x[i],m);
	}

	entity_look(e);
	CuAssertIntEquals(tc,3,TCOD_list_size(e->seen_items));
	CuAssertTrue(tc,TCOD_list_contains(e->seen_items,x[0]));
	CuAssertTrue(tc,TCOD_list_contains(e->seen_items,x[1]));
	CuAssertTrue(tc,TCOD_list_contains(e->seen_items,x[3]));
	
	CuAssertTrue(tc,!TCOD_list_contains(e->seen_items,x[2]));
	CuAssertTrue(tc,!TCOD_list_contains(e->seen_items,x[4]));
	CuAssertTrue(tc,!TCOD_list_contains(e->seen_items,x[5]));

	for(int i = 0; i < sizeof(x)/sizeof(item*); ++i)
	{
		item_delete(x[i]);
	}
}

void TestEntityMove(CuTest* tc)
{
	CuAssertTrue(tc, entity_move(e,e->x+1,e->y));
	CuAssertTrue(tc, entity_move(e,e->x-1,e->y));
	CuAssertTrue(tc, entity_move(e,e->x,e->y+1));
	CuAssertTrue(tc, entity_move(e,e->x,e->y-1));

	map_set_data(m,e->x-1,e->y-1,'#',TCOD_grey,false,false);
	map_set_data(m,e->x,e->y-1,'#',TCOD_grey,false,false);
	map_set_data(m,e->x+1,e->y-1,'#',TCOD_grey,false,false);
	map_set_data(m,e->x-1,e->y,'#',TCOD_grey,false,false);
	map_set_data(m,e->x+1,e->y,'#',TCOD_grey,false,false);
	map_set_data(m,e->x-1,e->y+1,'#',TCOD_grey,false,false);
	map_set_data(m,e->x,e->y+1,'#',TCOD_grey,false,false);
	map_set_data(m,e->x+1,e->y+1,'#',TCOD_grey,false,false);

	CuAssertTrue(tc, !entity_move(e,e->x+1,e->y));
	CuAssertTrue(tc, !entity_move(e,e->x-1,e->y));
	CuAssertTrue(tc, !entity_move(e,e->x,e->y+1));
	CuAssertTrue(tc, !entity_move(e,e->x,e->y-1));

	map_set_data(m,e->x-1,e->y-1,'.',TCOD_grey,true,true);
	map_set_data(m,e->x,e->y-1,'.',TCOD_grey,true,true);
	map_set_data(m,e->x+1,e->y-1,'.',TCOD_grey,true,true);
	map_set_data(m,e->x-1,e->y,'.',TCOD_grey,true,true);
	map_set_data(m,e->x+1,e->y,'.',TCOD_grey,true,true);
	map_set_data(m,e->x-1,e->y+1,'.',TCOD_grey,true,true);
	map_set_data(m,e->x,e->y+1,'.',TCOD_grey,true,true);
	map_set_data(m,e->x+1,e->y+1,'.',TCOD_grey,true,true);
}

void TestEntityDelete(CuTest* tc)
{
	// We can't really test anything here, but the memory needs to be freed.
	entity_delete(e);
	map_delete(m);
}
