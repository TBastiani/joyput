#include <linux/uinput.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include "joyput.h"

int get_next_keyboard_index()
{
	DIR *input_dir;
	struct dirent *entry;
	int index = 0;

	input_dir = opendir ("/dev/input");
	if (input_dir == NULL)
		die("Couldn't open the directory");

	while (entry = readdir (input_dir))
	{
		int event_value;
		if (strncmp(entry->d_name, "event", 5))
		{
			continue;
		}

		event_value = atoi(entry->d_name + 5);
		if (event_value >= index + 1)
		{
			index = event_value + 1;
		}
	}

	(void) closedir (input_dir);
	return index;
}

void open_keyboard_device(joydata_t *joydata)
{
	int device_index;
	char device_name[20];

	device_index = get_next_keyboard_index();
	snprintf(device_name, 20, "/dev/input/event%d", device_index);
#ifdef DEBUG
	printf("Creating fake input device at: %s\n", device_name);
#endif
}
