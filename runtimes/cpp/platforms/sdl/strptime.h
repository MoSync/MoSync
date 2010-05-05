#ifndef STRPTIME_H
#define STRPTIME_H

#include <time.h>

#ifdef __cplusplus
extern "C"
#endif
const char* strptime(const char *buf, const char *fmt, struct tm *tm);

#endif	//STRPTIME_H
