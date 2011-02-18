#include <string.h>
#include <stdlib.h>
#include "map.h"

map* map_new(int width, int height, char init_char)
{
	map* m = malloc(sizeof(map));
	m->data = TCOD_map_new(width,height);
	m->display = malloc(width*height*sizeof(map_display));
	TCOD_map_clear(m->data,1,1);
	for(int y = 0; y < height; ++y)
	{
		for(int x = 0; x < width; ++x)
		{
			m->display[x+y*width].c = init_char;
			m->display[x+y*width].color = TCOD_white;
		}
	}
	return m;
}

void map_delete(map* m)
{
	free(m->display);
	free(m);
}

void map_draw(map* m, TCOD_console_t console)
{
	int width = TCOD_map_get_width(m->data);
	int height = TCOD_map_get_height(m->data);
	for(int y = 0; y < height; ++y)
	{
		for(int x = 0; x < width; ++x)
		{
			char c = m->display[x+y*width].c;
			TCOD_color_t color = m->display[x+y*width].color;
			TCOD_console_put_char_ex(console,x,y,c,color,TCOD_black);
		}
	}
}

void map_copy_data(map* src, map* dest, int x, int y)
{
	map_display md = src->display[x+TCOD_map_get_width(src->data)*y];
	map_set_data(dest,x,y,md.c,md.color,
	             TCOD_map_is_transparent(src->data,x,y),
	             TCOD_map_is_walkable(src->data,x,y));
}

void map_set_data(map* m, int x, int y, char c, TCOD_color_t color, bool transparent, bool walkable)
{
	int width = TCOD_map_get_width(m->data);
	TCOD_map_set_properties(m->data,x,y,transparent,walkable);
	m->display[x+y*width].color = color;
	m->display[x+y*width].c = c;
}

static bool draw_box(TCOD_bsp_t *node, void *userData)
{
	map* m = (map*)((void**)userData)[0];
	TCOD_list_t* l = (TCOD_list_t*)((void**)userData)[1];
	TCOD_color_t colour = TCOD_white;
	if(TCOD_bsp_is_leaf(node))
	{
		colour = TCOD_color_RGB(node->x*3,node->y*3,node->h*3);
		TCOD_list_push(*l,node);
	}
	for(int y = 0; y < node->h; ++y)
	{
		if(y == 0 || y == node->h-1)
		{
			for(int x = 0; x < node->w; ++x)
			{
				map_set_data(m,x+node->x,y+node->y,'#',colour,0,0);
			}
		}
		else
		{
			map_set_data(m,node->x,y+node->y,'#',colour,0,0);
			map_set_data(m,node->x+node->w-1,y+node->y,'#',colour,0,0);
		}
	}
	return true;
}

void map_randomize(map* m, int depth)
{
	int width = TCOD_map_get_width(m->data);
	int height = TCOD_map_get_height(m->data);
	TCOD_bsp_t* bsp = TCOD_bsp_new_with_size(0,0,width,height);
	TCOD_list_t l = TCOD_list_new(1<<depth); // The BSP will have 2^depth leaf nodes
	void* args[] = {m,&l}; // Send these to draw_box()

	TCOD_bsp_split_recursive(bsp,NULL,depth,5,5,1.5f,1.5f);
	TCOD_bsp_traverse_pre_order(bsp, &draw_box, args);

	while(!TCOD_list_is_empty(l))
	{
		TCOD_bsp_t* r1 = TCOD_list_pop(l);
		TCOD_bsp_t* r2 = TCOD_list_pop(l);
		int x1 = r1->x + r1->w/2;
		int x2 = r2->x + r2->w/2;
		int y1 = r1->y + r1->h/2;
		int y2 = r2->y + r2->h/2;
		int x;
		int y;
		TCOD_line_init(x1,y1,x2,y2);
		while(!TCOD_line_step(&x,&y))
		{
			map_set_data(m,x,y,'.',TCOD_white,1,1);
		}
		if(TCOD_list_size(l) > 0)
		{
			TCOD_list_push(l,r2); // Pushing r1 seems to make all paths come from one place.
		}
	}

	TCOD_list_delete(l);
	TCOD_bsp_delete(bsp);
}

void map_random_free_spot(map* m, int* x, int* y)
{
	int width = TCOD_map_get_width(m->data);
	int height = TCOD_map_get_height(m->data);
	do {
		*x = rand()%width;
		*y = rand()%height;
	} while(!TCOD_map_is_walkable(m->data,*x,*y));
}
