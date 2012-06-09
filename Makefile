all: joyput

joyput: joyput.c utils.c uinput.c
	$(CC) -O0 -g -o joyput joyput.c utils.c uinput.c

PHONY: all
