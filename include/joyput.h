#ifndef __JOYPUT_H__
#define __JOYPUT_H__

#include <linux/uinput.h>
#include <linux/joystick.h>

typedef struct joydata
{
	int					fd_in;
	int					fd_out;
	struct js_event		event_in;
	int					read_pending;
	int					write_pending;
	struct input_event	event_out;
	int					stop_now;
} joydata_t;

void open_keyboard_device(joydata_t *joydata);

#endif /* ifndef __JOYPUT_H__ */
