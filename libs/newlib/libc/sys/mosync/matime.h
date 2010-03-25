#ifndef MATIME_H
#define MATIME_H

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

inline static struct tm* split_time(time_t timer, struct tm* tim_p) {
	return gmtime_r(&timer, tim_p);
}

/**
* Converts a @c tm structure to the ascii string buffer pointed to by \a buf.
* The buffer must be at least 25 bytes long.
* \return The \a buf argument.
*/
char* sprint_tm(const struct tm* tim_p, char* buf);

/**
* Converts a time value to a statically allocated string.
* \return A pointer to the string.
* \see maTime, maLocalTime
*/
char* sprint_time(time_t timer);

#ifdef __cplusplus
}
#endif

#endif	//MATIME_H
