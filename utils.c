#include <linux/input.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

void die(const char *message)
{
	printf("Fatal error : %s\n", message);
	exit(-1);
}

uint64_t timeval_to_microseconds(struct timeval timestamp)
{
	return timestamp.tv_usec + timestamp.tv_sec * 1000000;
}

char *type_from_int(uint16_t value)
{
	switch (value)
	{
		case EV_SYN:
			return "EV_SYN";
		case EV_KEY:
			return "EV_KEY";
		case EV_REL:
			return "EV_REL";
		case EV_ABS:
			return "EV_ABS";
		case EV_MSC:
			return "EV_MSC";
		case EV_SW:
			return "EV_SW";
		case EV_LED:
			return "EV_LED";
		case EV_SND:
			return "EV_SND";
		case EV_REP:
			return "EV_REP";
		case EV_FF:
			return "EV_FF";
		case EV_PWR:
			return "EV_PWR";
		case EV_FF_STATUS:
			return "EV_FF_STATUS";
		default:
			return NULL;
	}
}
