#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include "utils.h"
#include "joyput.h"
#include "joystick_read.h"
#include "translate.h"

static joydata_t *global_data = 0;

void signal_handler(int signal)
{
	if (global_data)
	{
		global_data->stop_now = 1;
	}
}

void log_keyboard_event(struct input_event *event)
{
	printf("Keyboard event\n");
	printf("\ttimestamp: %llu uS\n", (unsigned long long) timeval_to_microseconds(event->time));
	printf("\ttype: %s\n", type_from_int(event->type));
	printf("\tcode: %u\n", event->code);
	printf("\tvalue: %u\n", event->value);
}

void write_event(joydata_t *joydata)
{
	/* TODO */
	if (!joydata->write_pending)
	{
		return;
#ifdef DEBUG
		/* WTF?! */
		printf("In write_event(...) with no event pending. Something is wrong.\n");
#endif
		return;
	}

	joydata->write_pending = 0;

#ifdef DEBUG
	log_keyboard_event(&joydata->event_out);
#endif
}

void rate_limit(joydata_t *joydata)
{
	/* TODO */
}

void teardown(joydata_t *joydata)
{
	if (joydata->fd_in)
	{
		close(joydata->fd_in);
		joydata->fd_in = 0;
	}

	if (joydata->fd_out)
	{
    	if(ioctl(joydata->fd_out, UI_DEV_DESTROY) < 0)
        	die("Could not destroy uinput keyboard.");
    	close(joydata->fd_out);
		joydata->fd_out = 0;
#ifdef DEBUG
		printf("Successfully destroyed uinput device\n");
#endif
	}

	set_button_map(NULL);
}

int main(int argc, char **argv)
{
	joydata_t joydata = {0};
	global_data = &joydata;

	open_joystick_input(&joydata.fd_in, argc, argv);
	open_keyboard_device(&joydata);

	signal(SIGINT, signal_handler);

	while (!joydata.stop_now)
	{
		read_joystick_event(&joydata);

		translate_event(&joydata);

		write_event(&joydata);

		rate_limit(&joydata);
	}

#ifdef DEBUG
	printf("Exiting properly...\n");
#endif

    teardown(&joydata);
    return 0;
}
