#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libtcod.h>
#include "map.h"
#include "entity.h"

int main (int argc, char* argv[])
{
	TCOD_console_init_root(80,60,"Test!",0,TCOD_RENDERER_GLSL);
	TCOD_sys_set_fps(30);

	map* m = map_new(80,60,'.');
	map_randomize(m,6);

	int num_entities = 5;
	for(int i = 0; i < num_entities; ++i)
	{
		int x, y;
		map_random_free_spot(m,&x,&y);
		entity* e = entity_new(x,y,'@',TCOD_color_RGB(rand()%255,rand()%255,rand()%255));
		map_add_entity(m,e);
	}

	int lookat = 0;
	while(1)
	{
		entity* e = TCOD_list_get(m->entities,lookat);
		map_draw(e->known_map,NULL);
		entity_draw(e,NULL);

		int num_entities = TCOD_list_size(m->entities);
		for(int i = 0; i < num_entities; ++i)
		{
			entity* e = TCOD_list_get(m->entities,i);

			if(entity_at_destination(e))
			{
				int x,y;
				map_random_free_spot(e->host_map,&x,&y);
				entity_set_destination(e,x,y);
			}			
			entity_follow_path(e);
			entity_look(e);
		}
		
		TCOD_console_flush();
		TCOD_key_t key = TCOD_console_check_for_keypress(TCOD_KEY_PRESSED);
		if(key.c == 'q') { break; }
		if(key.c == 'n') { lookat = (lookat+1)%TCOD_list_size(m->entities); }
	}
	
	map_delete(m);
}
