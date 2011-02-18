#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libtcod.h>
#include "map.h"
#include "entity.h"

int main (int argc, char* argv[])
{
	TCOD_console_init_root(80,60,"Test!",0,TCOD_RENDERER_GLSL);

	map m;
	create_map(&m,80,60);
	randomize_map(&m,6);

	int num_entities = 5;
	TCOD_list_t entities = TCOD_list_new(num_entities);
	for(int i = 0; i < num_entities; ++i)
	{
		int x, y;
		random_free_map_spot(&m,&x,&y);
		entity* e = create_entity(&m,x,y,'@',TCOD_color_RGB(rand()%255,rand()%255,rand()%255));
		TCOD_list_push(entities,e);
	}

	while(1)
	{
		draw_map(&m,NULL);
		for(int i = 0; i < TCOD_list_size(entities); ++i)
		{
			entity* e = TCOD_list_get(entities,i);
			draw_entity(e,NULL);
		}
		
		TCOD_console_flush();
		TCOD_key_t key = TCOD_console_wait_for_keypress(1);
		if(key.c == 'q') { break; }
		TCOD_console_put_char(NULL,40,30,key.c,TCOD_BKGND_NONE);
	}

	while(!TCOD_list_is_empty(entities))
	{
		entity* e = TCOD_list_pop(entities);
		free_entity(e);
	}
	TCOD_list_delete(entities);
	
	free_map(&m);
}
