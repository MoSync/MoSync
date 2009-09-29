#include <matime.h>

struct tm* gmtime(const time_t* timer);
struct tm* localtime(const time_t* timer);
char *asctime(const struct tm *timeptr);
