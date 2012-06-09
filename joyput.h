#ifndef __JOYPUT_H__
#define __JOYPUT_H__

#define DEBUG
#define DEFAULT_INPUT_FILENAME "joystick0"

typedef struct joydata
{
	int					fd_in;
	int					fd_out;
	struct input_event	event_in;
	int					read_pending;
	struct input_event	event_out;
	int					stop_now;
} joydata_t;

void die(const char *message);

void open_keyboard_device(joydata_t *joydata);

#endif /* ifndef __JOYPUT_H__ */
