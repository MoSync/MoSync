/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/** \file matime.h
* \brief Time management.

* Time values are defined as seconds elapsed from midnight, January 1, 1970.
*/

#ifndef _MATIME_H_
#define _MATIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
#include <crtdefs.h>
#endif

#ifndef _TIME_H
#ifndef MAPIP
#define _TIME_H	//to avoid conflicting types for time_t
#endif

#ifndef MOSYNC_NATIVE
#ifndef _MSC_VER
typedef int time_t;
#endif

/// Components of a calendar date and time.
struct tm
{
	///Seconds after the minute. Range: 0-59
	int tm_sec;
	///Minutes after the hour. Range: 0-59
	int tm_min;
	///Hours since midnight. Range: 0-23
	int tm_hour;
	///Day of the month. Range: 1-31
	int tm_mday;
	///Months since January. Range: 0-11
	int tm_mon;
	///Years since 1900.
	int tm_year;
	///Days since Sunday. Range: 0-6
	int tm_wday;
	///Days since January 1. Range: 0-365
	int tm_yday;
	///Daylight Saving Time flag. Ignored by MoSync functions.
	int tm_isdst;
};
#endif
#endif	//_TIME_H

#ifdef MOSYNC_NATIVE
#include <time.h>
#endif

/**
* Converts a time value to the structure pointed to by \a tim_p.
* \return The \a tim_p argument.
* \see maTime, maLocalTime
*/
struct tm* split_time(time_t timer, struct tm* tim_p);

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

#ifndef MOSYNC_NATIVE
/**
* Converts the structure pointed to by \a tim_p into a time value.
* \see maTime, maLocalTime
*/
time_t mktime(struct tm* tim_p);
#endif

#ifdef __cplusplus
}
#endif

#endif //_MATIME_H_
