#include <fcntl.h>
#include <strings.h>

#include "joystick_read.h"
#include "utils.h"

void configure_joystick(int fd)
{
	uint8_t number;
	/* Query button number and register them */
    if(ioctl(fd, JSIOCGBUTTONS, &number) < 0)
        die("Could not complete ioctl call.");
#ifdef DEBUG
	printf("Found %d button(s) on joypad device.\n", number);
#endif
	if (number > KEY_MAX - BTN_MISC)
	{
		die("More buttons on joypad than allowed. This shouldn't happen.");
	}

	uint16_t button_map[KEY_MAX - BTN_MISC + 1] = {0};
	for (int i = 0; i < number; i++)
	{
		button_map[i] = 1;
	}
	if (ioctl(fd, JSIOCSBTNMAP, button_map) < 0)
    	die("Could not complete ioctl call.");

	/* Query axes number and register them. */
    if(ioctl(fd, JSIOCGAXES, &number) < 0)
        die("Could not complete ioctl call.");
#ifdef DEBUG
	printf("Found %d axe(s) on joypad device.\n", number);
#endif
	if (number > ABS_CNT)
	{
		die("More axes on joypad than allowed. This shouldn't happen.");
	}

	uint8_t axes_map[ABS_CNT] = {0};
	for (int i = 0; i < number; i++)
	{
		axes_map[i] = 1;
	}
	if (ioctl(fd, JSIOCSAXMAP, axes_map) < 0)
    	die("Could not complete ioctl call.");
}

void open_joystick_input(int *out_fd, int argc, char **argv)
{
	char input_filename[200];
	int fd;
	if (argc >= 2)
	{
		snprintf(input_filename, 200, "/dev/input/%s", argv[1]);
	}
	else
	{
		snprintf(input_filename, 200, "/dev/input/%s", DEFAULT_INPUT_FILENAME);
	}
#ifdef DEBUG
	printf("Trying to open file at : %s\n", input_filename);
#endif

    fd = open(input_filename, O_RDONLY);
    if (fd < 0)
        die("error: could not open file");

	*out_fd = fd;

	/**
	 * FIXME Not called because it fails for some reason.
	 * It's possible that the sixaxis driver was compiled against 
	 * a kernel that is too old
	configure_joystick(fd);
	 */
}

void read_joystick_event(int fd, struct js_event *input_event)
{
	int size;
	size = read(fd, input_event, sizeof(struct js_event));

	if (size < sizeof(struct js_event))
		die("Read a length lower than sizeof(struct js_event). Case not handled.");

#ifdef DEBUG
	log_joystick_event(input_event);
#endif

	// FIXME joydata->read_pending = 1;
}

char *js_type_from_int(uint8_t type)
{
	switch(type)
	{
		case JS_EVENT_BUTTON:
			return "JS_EVENT_BUTTON";
		case JS_EVENT_AXIS:
			return "JS_EVENT_AXIS";
		case JS_EVENT_INIT + JS_EVENT_BUTTON:
			return "JS_EVENT_INIT - BUTTON";
		case JS_EVENT_INIT + JS_EVENT_AXIS:
			return "JS_EVENT_INIT - AXIS";
		default:
			die("Unrecognised joystick event type.");
			return NULL;
	}
}

void log_joystick_event(struct js_event *event)
{
	printf("Keyboard event\n");
	printf("\ttimestamp: %u uS\n", event->time);
	printf("\ttype: %s\n", js_type_from_int(event->type));
	printf("\taxis number: %u\n", event->number);
	printf("\tvalue: %d\n", (int) event->value);
}
