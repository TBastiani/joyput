#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/joystick.h>
#include <linux/input.h>
#include <sys/time.h>

#include "joyput.h"
#include "translate.h"

static button_map *button_map_list;

void read_config()
{
	/* This is only a fake config */
	button_map *button_map_node;
	button_map_node = (button_map *) malloc(sizeof(button_map));

	button_map_node->is_an_axis = 0;
	button_map_node->joy_index = 5;
	button_map_node->axis_threshold = 0;
	button_map_node->key_index = KEY_A;
	button_map_node->next = NULL;

	set_button_map(button_map_node);
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

