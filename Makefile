CC=clang

all: joyput

joyput: joyput.c utils.c uinput.c joystick_read.c
	$(CC) -O0 -g -std=c99 -DDEBUG -o joyput utils.c uinput.c joystick_read.c joyput.c

PHONY: all
