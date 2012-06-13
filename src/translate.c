#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "joyput.h"
#include "translate.h"

static button_map *button_map_list;

void read_config()
{
	/* TODO */
}

static void apply_map(joydata_t *joydata)
{
	/* TODO */
	//joydata->write_pending = 1;
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

static button_map *get_button_map(void)
{
	return button_map_list;
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

