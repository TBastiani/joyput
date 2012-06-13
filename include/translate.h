#ifndef __TRANSLATE_H__
#define __TRANSLATE_H__
#include "joyput.h"

typedef struct button_map
{
	int8_t button_index;
	int8_t axis_index;
	uint16_t axis_threshold;
	uint16_t key_index;
	struct button_map *next;
} button_map;

void read_config();
void translate_event(joydata_t *joydata);
void set_button_map(button_map *list);
#endif /* ifndef __TRANSLATE_H__ */
