#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libtcod.h>
#include "map.h"
#include "entity.h"

int main (int argc, char* argv[])
{
	TCOD_console_init_root(80,60,"Test!",0,TCOD_RENDERER_GLSL);

	map* m = map_new(80,60);
	map_randomize(m,6);

	int num_entities = 5;
	TCOD_list_t entities = TCOD_list_new(num_entities);
	for(int i = 0; i < num_entities; ++i)
	{
		int x, y;
		map_random_free_spot(m,&x,&y);
		entity* e = entity_new(m,x,y,'@',TCOD_color_RGB(rand()%255,rand()%255,rand()%255));
		TCOD_list_push(entities,e);
	}

	while(1)
	{
		map_draw(m,NULL);
		for(int i = 0; i < TCOD_list_size(entities); ++i)
		{
			entity* e = TCOD_list_get(entities,i);
			entity_draw(e,NULL);
		}
		
		TCOD_console_flush();
		TCOD_key_t key = TCOD_console_wait_for_keypress(1);
		if(key.c == 'q') { break; }
	}

	while(!TCOD_list_is_empty(entities))
	{
		entity* e = TCOD_list_pop(entities);
		entity_delete(e);
	}
	TCOD_list_delete(entities);
	
	map_delete(m);
}
