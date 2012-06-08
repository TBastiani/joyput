#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <linux/input.h>

#define DEBUG
#define DEFAULT_INPUT_FILENAME "joystick0"

typedef struct joydata
{
	int					fd_in;
	struct input_event	event_in;
	int					read_pending;
	struct input_event	event_out;
	int					stop_now;
} joydata_t;

joydata_t *global_data = 0;

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

void open_output(joydata_t *joydata)
{
	/* TODO */
}

void read_event(joydata_t *joydata)
{
	int size;
	size = read(joydata->fd_in, &joydata->event_in, sizeof(struct input_event));

	if (size < sizeof(struct input_event))
		die("Read a length lower than sizeof(struct input_event). Case not handled.");
}

void translate_event(joydata_t *joydata)
{
	/* TODO */
	joydata->event_out = joydata->event_in;
}

void write_event(joydata_t *joydata)
{
	/* TODO */
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
}

int main(int argc, char **argv)
{
	joydata_t joydata = {0};
	global_data = &joydata;

	open_input(&joydata, argc, argv);
	open_output(&joydata);

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
