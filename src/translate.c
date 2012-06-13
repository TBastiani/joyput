#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/joystick.h>
#include <linux/input.h>
#include <sys/time.h>
#define __USE_POSIX
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "joyput.h"
#include "translate.h"
#include "utils.h"

static button_map *button_map_list;

void read_config()
{
	button_map *current_button_map;
	button_map *button_map_list = NULL;
	int fd = open("/etc/joyput.conf", O_RDONLY);
	if (fd == -1)
		die("Could not open config file /etc/joyput.c.");

	/* FIXME Not supporting any conf files bigger than 10000 bytes. */
	char buffer[10000];
	int size = read(fd, buffer, 10000);
	if (size < 0)
		die("Config file at /etc/joyput.c is empty.");

	close(fd);

	char *line;
	char *save_line;
	line = strtok_r(buffer, "\n", &save_line);

	while (line != NULL)
	{
		char *save_field; 
		char *field;
		int is_an_axis;
		int joy_index;
		int key_index;

		field = strtok_r(line, ":", &save_field);
		is_an_axis = atoi(field);

		field = strtok_r(NULL, ":", &save_field);
		joy_index = atoi(field);
		field = strtok_r(NULL, ":", &save_field);
		key_index = atoi(field);

		if (button_map_list == NULL)
		{
			current_button_map = (button_map *) calloc(1, sizeof(button_map));
			button_map_list = current_button_map;
		}
		else
		{
			current_button_map->next = (button_map *) calloc(1, sizeof(button_map));
			current_button_map = current_button_map->next;
		}

		current_button_map->is_an_axis = is_an_axis;
		current_button_map->joy_index = joy_index;
		current_button_map->key_index = key_index;

		/* Next line */
		line = strtok_r(NULL, "\n", &save_line);
		/* FIXME How is strtok_r() context destroyed for sure? */
	}

	set_button_map(button_map_list);
}

static button_map *get_button_map(void)
{
	return button_map_list;
}

static void apply_map(joydata_t *joydata)
{
	button_map *current_button_map;
	current_button_map = get_button_map();

	while (current_button_map != NULL)
	{
		uint8_t skip = 0;

		if (current_button_map->is_an_axis &&
			(joydata->event_in.type  & ~JS_EVENT_INIT) != JS_EVENT_AXIS)
			skip = 1;

		if (!current_button_map->is_an_axis &&
			(joydata->event_in.type & ~JS_EVENT_INIT) != JS_EVENT_BUTTON)
			skip = 1;

		if (!skip && current_button_map->joy_index == joydata->event_in.number)
		{
			struct timeval time;
			gettimeofday(&time, NULL);

			joydata->event_out.time = time;
			joydata->event_out.type = EV_KEY;
			joydata->event_out.code = current_button_map->key_index;
			joydata->event_out.value = joydata->event_in.value;

			joydata->write_pending = 1;
			break;
		}

		current_button_map = current_button_map->next;
	}
}

void translate_event(joydata_t *joydata)
{
	if (!joydata->read_pending)
	{
#ifdef DEBUG
		/* WTF?! */
		printf("In translate_event(...) with no event pending. Something is wrong.\n");
#endif
		return;
	}
	joydata->read_pending = 0;

	apply_map(joydata);
}

static void free_button_map(button_map *list)
{
	if (list->next != NULL)
		free_button_map(list->next);
	free(list);
}

void set_button_map(button_map *list)
{
	if (list != button_map_list)
	{
		if (button_map_list != NULL)
		{
			free_button_map(button_map_list);
		}
		button_map_list = list;
	}
}

