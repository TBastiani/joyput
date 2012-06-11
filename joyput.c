#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include "utils.h"
#include "joyput.h"

static joydata_t *global_data = 0;

void signal_handler(int signal)
{
	if (global_data)
	{
		global_data->stop_now = 1;
	}
}

void die(const char *message)
{
	printf("Fatal error : %s\n", message);
	exit(-1);
}

void open_input(joydata_t *joydata, int argc, char **argv)
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

	joydata->fd_in = fd;
}

void read_event(joydata_t *joydata)
{
	int size;
	size = read(joydata->fd_in, &joydata->event_in, sizeof(struct js_event));

	if (size < sizeof(struct js_event))
		die("Read a length lower than sizeof(struct input_event). Case not handled.");

	// FIXME joydata->read_pending = 1;
}

void translate_event(joydata_t *joydata)
{
	/* TODO */
}

void *log_keyboard_event(struct input_event *event)
{
	printf("Keyboard event\n");
	printf("\ttimestamp: %llu uS\n", timeval_to_microseconds(event->time));
	printf("\ttype: %s\n", type_from_int(event->type));
	printf("\tcode: %u\n", event->code);
	printf("\tvalue: %u\n", event->value);
}

void write_event(joydata_t *joydata)
{
	/* TODO */
	if (!joydata->read_pending)
	{
		return; /* FIXME remove statement */
#ifdef DEBUG
		/* WTF?! */
		printf("In write_event(...) with no event pending. Something is wrong.\n");
#endif
		return;
	}

	joydata->read_pending = 0;

#ifdef DEBUG
	log_keyboard_event(&joydata->event_out);
#endif
}

void rate_limit(joydata_t *joydata)
{
	/* TODO */
}

void close_fds(joydata_t *joydata)
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
}

int main(int argc, char **argv)
{
	joydata_t joydata = {0};
	global_data = &joydata;

	open_input(&joydata, argc, argv);
	open_keyboard_device(&joydata);

	signal(SIGINT, signal_handler);

	while (!joydata.stop_now)
	{
		read_event(&joydata);

		translate_event(&joydata);

		write_event(&joydata);

		rate_limit(&joydata);
	}

#ifdef DEBUG
	printf("Exiting properly...\n");
#endif

    close_fds(&joydata);
    return 0;
}
