#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libtcod.h>
#include "camera.h"
#include "map.h"
#include "entity.h"
#include "item.h"
#include "util.h"
#include "goal.h"
#include "goals/move.h"
#include "goals/explore.h"
#include "goals/get_item.h"
#include "goals/make_item.h"

int main (int argc, char* argv[])
{
	TCOD_console_init_root(80,60,"Test!",0,TCOD_RENDERER_GLSL);
	TCOD_sys_set_fps(30);

	map* m = map_new(80,60,'.');
	map_randomize(m,6);

	int num_entities = 32;
	for(int a = 0; a < num_entities; ++a)
	{
		int x, y;
		map_random_free_spot(m,&x,&y);
		entity* e = entity_new(x,y,'@',TCOD_color_RGB(rand()%255,rand()%255,rand()%255));
		map_add_entity(m,e);
		entity_add_goal(e,goal_new_explore(e));
		
		/* goal* g = goal_new_move(e,3,3); */
		/* goal_add_subgoal(g,goal_new_move(e,78,58)); */
		/* goal_add_subgoal(g,goal_new_move(e,78,3)); */
		/* goal_add_subgoal(g,goal_new_move(e,3,58)); */
		//entity_add_goal(e,goal_new_search(e,ITEM_rock,3));
		entity_add_goal(e,goal_new_get_item(e,ITEM_house,1));
	}
	for(int a = 0; a < 500; ++a)
	{
		int x, y;
		map_random_free_spot(m,&x,&y);
		item* i;
		int val = rand()%3;
		if(val == 0)
		{
			i = item_new_branch(x,y);
		}
		if(val == 1)
		{
			i = item_new_rock(x,y);
		}
		if(val == 2)
		{
			i = item_new_tree(x,y);
		}
		item_set_map(i,m);
	}

	TCOD_list_t cameras = TCOD_list_new(num_entities);
	TCOD_list_push(cameras,camera_new(m,TCOD_list_get(m->entities,0),40,30,0,0));
	TCOD_list_push(cameras,camera_new(m,TCOD_list_get(m->entities,1),40,30,40,0));
	TCOD_list_push(cameras,camera_new(m,TCOD_list_get(m->entities,2),40,30,0,30));
	TCOD_list_push(cameras,camera_new(m,TCOD_list_get(m->entities,3),40,30,40,30));
	int lookat = 0;
	entity* e = TCOD_list_get(m->entities,lookat);
	while(1)
	{
		foreach(entity,m->entities)
		{
			entity* e = *itr;
			entity_do_goal(e);
		}

		TCOD_console_clear(NULL);
		foreach(camera,cameras)
		{
			camera_draw(*itr);
		}
		TCOD_console_flush();

		TCOD_key_t key = TCOD_console_check_for_keypress(TCOD_KEY_PRESSED);
		if(key.c == 'q') { break; }
		if(key.c == 'n') { lookat = (lookat+1)%TCOD_list_size(m->entities);
			               e = TCOD_list_get(m->entities,lookat); }
		if(key.c == 'i')
		{
			printf("------\n");
			foreach(item,e->inventory)
			{
				printf("%s\n",item_names[(*itr)->type]);
			}
			printf("------\n");
		}
		if(key.c == 'g')
		{
			printf("------\n");
			foreach(goal,e->goal_stack)
			{
				goal_display(*itr);
			}
			printf("------\n");
		}
		if(key.c == 'x')
		{
			printf("(%d,%d)\n",e->x,e->y);
		}
	}
	map_delete(m);
}
