/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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

#ifndef _MSC_VER
typedef int time_t;
#endif
#endif	//_TIME_H

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

/**
* Converts the structure pointed to by \a tim_p into a time value.
* \see maTime, maLocalTime
*/
time_t mktime(struct tm* tim_p);

#ifdef __cplusplus
}
#endif

#endif //_MATIME_H_
