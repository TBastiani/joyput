#include <stdint.h>
#include <sys/time.h>

void die(const char *message);
uint64_t timeval_to_microseconds(struct timeval timestamp);
char *type_from_int(uint16_t value);
