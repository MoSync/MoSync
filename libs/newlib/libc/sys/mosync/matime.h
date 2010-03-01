#ifndef MATIME_H
#define MATIME_H

#include <time.h>

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

#endif	//MATIME_H
