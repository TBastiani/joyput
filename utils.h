#include <stdint.h>
#include <sys/time.h>

uint64_t timeval_to_microseconds(struct timeval timestamp);
char *type_from_int(uint16_t value);
