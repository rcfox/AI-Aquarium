#include <stdio.h>
#include <string.h>
#include <libtcod.h>
#include "map.h"

int main (int argc, char* argv[])
{
	TCOD_console_init_root(80,60,"Test!",0,TCOD_RENDERER_GLSL);

	map m;
	create_map(&m,80,60);
	randomize_map(&m);
	while(1)
	{
		int width = TCOD_map_get_width(m.data);
		int height = TCOD_map_get_height(m.data);
		for(int y = 0; y < height; ++y)
		{
			for(int x = 0; x < width; ++x)
			{
				char c = m.display[x+y*width].c;
				TCOD_color_t color = m.display[x+y*width].color;
				TCOD_console_put_char_ex(NULL,x,y,c,color,TCOD_black);
			}
		}
		
		TCOD_console_flush();
		TCOD_key_t key = TCOD_console_wait_for_keypress(1);
		if(key.c == 'q') { break; }
		TCOD_console_put_char(NULL,40,30,key.c,TCOD_BKGND_NONE);
	}

	free_map(&m);
}
