all: joyput

joyput: joyput.c utils.c
	$(CC) -O0 -g -o joyput joyput.c utils.c

PHONY: all
