#include <string.h>
#include <stdlib.h>
#include "map.h"

void create_map(map* m, int width, int height)
{
	m->data = TCOD_map_new(width,height);
	m->display = malloc(width*height*sizeof(map_display));
	TCOD_map_clear(m->data,1,1);
	for(int y = 0; y < height; ++y)
	{
		for(int x = 0; x < width; ++x)
		{
			m->display[x+y*width].c = '.';
			m->display[x+y*width].color = TCOD_white;
		}
	}
}

void free_map(map* m)
{
	free(m->display);	
}

void set_map_data(map* m, int x, int y, char c, TCOD_color_t color, bool transparent, bool walkable)
{
	int width = TCOD_map_get_width(m->data);
	TCOD_map_set_properties(m->data,x,y,transparent,walkable);
	m->display[x+y*width].color = color;
	m->display[x+y*width].c = c;
}

static bool draw_box(TCOD_bsp_t *node, void *userData)
{
	map* m = (map*)userData;
	for(int y = 0; y < node->h; ++y)
	{
		if(y == 0 || y == node->h-1)
		{
			for(int x = 0; x < node->w; ++x)
			{
				set_map_data(m,x+node->x,y+node->y,'#',TCOD_white,0,0);
			}
		}
		else
		{
			set_map_data(m,node->x,y+node->y,'#',TCOD_white,0,0);
			set_map_data(m,node->x+node->w-1,y+node->y,'#',TCOD_white,0,0);
		}
	}
	return true;
}

void randomize_map(map* m)
{
	int width = TCOD_map_get_width(m->data);
	int height = TCOD_map_get_height(m->data);
	TCOD_bsp_t* bsp = TCOD_bsp_new_with_size(0,0,width,height);

	TCOD_bsp_split_recursive(bsp,NULL,5,5,5,1.5f,1.5f);
	TCOD_bsp_traverse_pre_order(bsp, &draw_box, m);

	TCOD_bsp_delete(bsp);
}
