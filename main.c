#include "libtcod.h"

int main (int argc, char* argv[])
{
	TCOD_console_init_root(80,60,"Test!",0,TCOD_RENDERER_GLSL);

	while(1)
	{
		TCOD_key_t key = TCOD_console_wait_for_keypress(1);
		if(key.c == 'q') { break; }
	}
}
