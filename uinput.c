#include <linux/uinput.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "joyput.h"

/*
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
*/

void open_keyboard_device(joydata_t *joydata)
{
	int fd;
    struct uinput_user_dev uinput_device = {0};
	/*
	int device_index;
	char device_name[20];

	device_index = get_next_keyboard_index();
	snprintf(device_name, 20, "/dev/input/event%d", device_index);
#ifdef DEBUG
	printf("Creating fake input device at: %s\n", device_name);
#endif
	*/
	
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if(fd < 0)
        die("Could not open uinput device /dev/uinput");

	/* Register keyboard */
    if(ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
        die("Could not complete ioctl call.");
	/* Register mouse left button */
    if(ioctl(fd, UI_SET_KEYBIT, BTN_LEFT) < 0)
        die("Could not complete ioctl call.");

	/* Register relative position device */
    if(ioctl(fd, UI_SET_EVBIT, EV_REL) < 0)
        die("Could not complete ioctl call.");
    if(ioctl(fd, UI_SET_RELBIT, REL_X) < 0)
        die("Could not complete ioctl call.");
    if(ioctl(fd, UI_SET_RELBIT, REL_Y) < 0)
        die("Could not complete ioctl call.");

    snprintf(uinput_device.name, UINPUT_MAX_NAME_SIZE, "Joypad emulated keyboard");
    uinput_device.id.bustype = BUS_USB;
    uinput_device.id.vendor  = 0x1;
    uinput_device.id.product = 0x1;
    uinput_device.id.version = 1;

    if(write(fd, &uinput_device, sizeof(uinput_device)) < 0)
        die("Could not complete write call to uinput device.");

    if(ioctl(fd, UI_DEV_CREATE) < 0)
        die("Could not create uinput keyboard.");

	joydata->fd_out = fd;
#ifdef DEBUG
	printf("Successfully created uinput keyboard.\n");
#endif
}
