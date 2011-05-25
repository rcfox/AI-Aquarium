#include <libtcod.h>
#include <stdlib.h>
#include "camera.h"
#include "map.h"
#include "entity.h"
#include "item.h"
#include "util.h"

camera* camera_new(map* map, entity* target, int width, int height, int screen_x, int screen_y)
{
	camera* c = malloc(sizeof(camera));
	c->map = map;
	c->target = target;
	c->width = width;
	c->height = height;
	c->screen_x = screen_x;
	c->screen_y = screen_y;
	c->console = NULL;
	return c;
}

static bool check_bounds(camera* c, int x, int y)
{
	if(x < c->screen_x || x > c->screen_x+c->width) return false;
	if(y < c->screen_y || y > c->screen_y+c->height) return false;
	if(x < 0 || x > TCOD_console_get_width(c->console))return false;
	if(y < 0 || y > TCOD_console_get_height(c->console)) return false;
	return true;
}

void camera_draw(camera* c)
{
	entity* target = c->target;
	int width = c->width;
	int height = c->height;
	int screen_x = c->screen_x;
	int screen_y = c->screen_y;
	int map_width = TCOD_map_get_width(c->map->data);
	for(int y = -height/2; y < height/2; ++y)
	{
		for(int x = -width/2; x < width/2; ++x)
		{
			int tx = target->x+x;
			int ty = target->y+y;
			int draw_x = x+screen_x+width/2;
			int draw_y = y+screen_y+height/2;
			if(check_bounds(c,draw_x,draw_y) && map_check_bounds(c->map,tx,ty))
			{
				char ch = c->target->known_map->display[tx+ty*map_width].c;
				TCOD_color_t color = c->target->known_map->display[tx+ty*map_width].color;
				TCOD_console_put_char_ex(c->console,draw_x,draw_y,ch,color,TCOD_black);
			}
		}
	}
	foreach(item,target->seen_items)
	{
		item* i = *itr;
		int tx = i->x-target->x;
		int ty = i->y-target->y;
		int draw_x = screen_x+width/2+tx;
		int draw_y = screen_y+height/2+ty;
		if(check_bounds(c,draw_x,draw_y))
		{
			TCOD_console_put_char_ex(c->console,draw_x,draw_y,i->c,i->color,TCOD_black);
		}
	}
	foreach(entity,target->seen)
	{
		entity* e = *itr;
		int tx = e->x-target->x;
		int ty = e->y-target->y;
		int draw_x = screen_x+width/2+tx;
		int draw_y = screen_y+height/2+ty;
		if(check_bounds(c,draw_x,draw_y))
		{
			TCOD_console_put_char_ex(c->console,draw_x,draw_y,e->c,e->color,TCOD_black);
		}
	}
}
