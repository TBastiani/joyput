#include <linux/uinput.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "joyput.h"
#include "utils.h"

void open_keyboard_device(joydata_t *joydata)
{
	int fd;
    struct uinput_user_dev uinput_device = {{0}};
	
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if(fd < 0)
        die("Could not open uinput device /dev/uinput");

	/* Register keyboard */
    if(ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
        die("Could not complete ioctl call.");

	/* Register keyboard */
	for (int i = 0; i < KEY_CNT; i++)
	{
    	if(ioctl(fd, UI_SET_KEYBIT, i) < 0)
        	die("Could not complete ioctl call.");
	}

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
