#include "matime.h"
#include <stdio.h>

char* sprint_tm(const struct tm *tim_p, char* buf)
{
	static const char day_name[7][3] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	static const char mon_name[12][3] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};

	sprintf(buf, "%.3s %.3s%3d %.2d:%.2d:%.2d %d",
		day_name[tim_p->tm_wday], 
		mon_name[tim_p->tm_mon],
		tim_p->tm_mday, tim_p->tm_hour, tim_p->tm_min,
		tim_p->tm_sec, 1900 + tim_p->tm_year);
	return buf;
}

static char stbuf[26];

char* sprint_time(time_t timer) {
	struct tm temp;
	return sprint_tm(gmtime_r(&timer, &temp), stbuf);
}
