all: joyput

joyput: joyput.c utils.c uinput.c
	$(CC) -O0 -g -std=c99 -o joyput joyput.c utils.c uinput.c

PHONY: all
