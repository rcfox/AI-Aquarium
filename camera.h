#ifndef CAMERA_H
#define CAMERA_H

struct map;

typedef struct camera
{
	int screen_x;
	int screen_y;
	int width;
	int height;
	struct map* map;
	struct entity* target;
	TCOD_console_t console;
} camera;

camera* camera_new(struct map* map, struct entity* target, int width, int height, int screen_x, int screen_y);
void camera_draw(camera* c);

#endif
