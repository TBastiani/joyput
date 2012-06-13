#ifndef __JOYSTICK_READ_H__
#define __JOYSTICK_READ_H__

#include <linux/joystick.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>

#include "joyput.h"

#define DEFAULT_INPUT_FILENAME "js0"

void open_joystick_input(int *out_fd, int argc, char **argv);
void read_joystick_event(joydata_t *joydata);
void log_joystick_event(struct js_event *event);

#endif /* ifndef __JOYSTICK_READ_H__ */
