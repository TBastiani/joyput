all: joyput

joyput: joyput.c utils.c uinput.c joystick_read.c
	$(CC) -O0 -g -std=c99 -DDEBUG -o joyput joyput.c utils.c uinput.c joystick_read.c

PHONY: all
