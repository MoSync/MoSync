/*
* matime.c
* Original Author:	Adapted from tzcode maintained by Arthur David Olson.
* Modifications: Changed to mktm_r and added __tzcalc_limits - 04/10/02, Jeff Johnston
*                Fixed bug in mday computations - 08/12/04, Alex Mogilnikov <alx@intellectronika.ru>
*                Fixed bug in __tzcalc_limits - 08/12/04, Alex Mogilnikov <alx@intellectronika.ru>
*                Adapted for MoSync - 21/02/07, Fredrik Eldh <fredrik.eldh@mobilesorcery.com>
*/

#include "matime.h"
#include "mavsprintf.h"
#include "mastdlib.h"

#define SECSPERMIN	60
#define MINSPERHOUR	60
#define HOURSPERDAY	24
#define SECSPERHOUR	(SECSPERMIN * MINSPERHOUR)
#define SECSPERDAY	(SECSPERHOUR * HOURSPERDAY)
#define DAYSPERWEEK	7
#define MONSPERYEAR	12

#define YEAR_BASE	1900
#define EPOCH_YEAR 1970
#define EPOCH_WDAY 4
#define EPOCH_YEARS_SINCE_LEAP 2
#define EPOCH_YEARS_SINCE_CENTURY 70
#define EPOCH_YEARS_SINCE_LEAP_CENTURY 370

static const int DAYS_IN_MONTH[12] =
{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

#define _DAYS_IN_MONTH(x) ((x == 1) ? days_in_feb : DAYS_IN_MONTH[x])

static const int _DAYS_BEFORE_MONTH[12] =
{0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

#define _ISLEAP(y) (((y) % 4) == 0 && (((y) % 100) != 0 || (((y)+1900) % 400) == 0))
#define _DAYS_IN_YEAR(year) (_ISLEAP(year) ? 366 : 365)
 
static const int mon_lengths[2][MONSPERYEAR] = {
	{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};


static const int year_lengths[2] = {
	365,
	366
};

struct tm* split_time(time_t timer, struct tm* res)
{
	long days, rem;
	time_t lcltime;
	int y;
	int yleap;
	const int *ip;

	/* base decision about std/dst time on current time */
	lcltime = timer;

	days = ((long)lcltime) / SECSPERDAY;
	rem = ((long)lcltime) % SECSPERDAY;
	while (rem < 0) 
	{
		rem += SECSPERDAY;
		--days;
	}
	while (rem >= SECSPERDAY)
	{
		rem -= SECSPERDAY;
		++days;
	}

	/* compute hour, min, and sec */
	res->tm_hour = (int) (rem / SECSPERHOUR);
	rem %= SECSPERHOUR;
	res->tm_min = (int) (rem / SECSPERMIN);
	res->tm_sec = (int) (rem % SECSPERMIN);

	/* compute day of week */
	if ((res->tm_wday = ((EPOCH_WDAY + days) % DAYSPERWEEK)) < 0)
		res->tm_wday += DAYSPERWEEK;

	/* compute year & day of year */
	y = EPOCH_YEAR;
	if (days >= 0)
	{
		for (;;)
		{
			yleap = _ISLEAP(y);
			if (days < year_lengths[yleap])
				break;
			y++;
			days -= year_lengths[yleap];
		}
	}
	else
	{
		do
		{
			--y;
			yleap = _ISLEAP(y);
			days += year_lengths[yleap];
		} while (days < 0);
	}

	res->tm_year = y - YEAR_BASE;
	res->tm_yday = days;
	ip = mon_lengths[yleap];
	for (res->tm_mon = 0; days >= ip[res->tm_mon]; ++res->tm_mon)
		days -= ip[res->tm_mon];
	res->tm_mday = days + 1; 

	return res;
}

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
	return sprint_tm(split_time(timer, &temp), stbuf);
}


static void validate_structure(struct tm *tim_p)
{
	div_t res;
	int days_in_feb = 28;

	/* calculate time & date to account for out of range values */
	if (tim_p->tm_sec < 0 || tim_p->tm_sec > 59)
	{
		res = div (tim_p->tm_sec, 60);
		tim_p->tm_min += res.quot;
		if ((tim_p->tm_sec = res.rem) < 0)
		{
			tim_p->tm_sec += 60;
			--tim_p->tm_min;
		}
	}

	if (tim_p->tm_min < 0 || tim_p->tm_min > 59)
	{
		res = div (tim_p->tm_min, 60);
		tim_p->tm_hour += res.quot;
		if ((tim_p->tm_min = res.rem) < 0)
		{
			tim_p->tm_min += 60;
			--tim_p->tm_hour;
		}
	}

	if (tim_p->tm_hour < 0 || tim_p->tm_hour > 23)
	{
		res = div (tim_p->tm_hour, 24);
		tim_p->tm_mday += res.quot;
		if ((tim_p->tm_hour = res.rem) < 0)
		{
			tim_p->tm_hour += 24;
			--tim_p->tm_mday;
		}
	}

	if (tim_p->tm_mon > 11)
	{
		res = div (tim_p->tm_mon, 12);
		tim_p->tm_year += res.quot;
		if ((tim_p->tm_mon = res.rem) < 0)
		{
			tim_p->tm_mon += 12;
			--tim_p->tm_year;
		}
	}

	if (_DAYS_IN_YEAR (tim_p->tm_year) == 366)
		days_in_feb = 29;

	if (tim_p->tm_mday <= 0)
	{
		while (tim_p->tm_mday <= 0)
		{
			if (--tim_p->tm_mon == -1)
			{
				tim_p->tm_year--;
				tim_p->tm_mon = 11;
				days_in_feb =
					((_DAYS_IN_YEAR (tim_p->tm_year) == 366) ?
					29 : 28);
			}
			tim_p->tm_mday += _DAYS_IN_MONTH (tim_p->tm_mon);
		}
	}
	else
	{
		while (tim_p->tm_mday > _DAYS_IN_MONTH (tim_p->tm_mon))
		{
			tim_p->tm_mday -= _DAYS_IN_MONTH (tim_p->tm_mon);
			if (++tim_p->tm_mon == 12)
			{
				tim_p->tm_year++;
				tim_p->tm_mon = 0;
				days_in_feb =
					((_DAYS_IN_YEAR (tim_p->tm_year) == 366) ?
					29 : 28);
			}
		}
	}
}

time_t mktime(struct tm *tim_p)
{
	time_t tim = 0;
	long days = 0;
	int year;

	/* validate structure */
	validate_structure (tim_p);

	/* compute hours, minutes, seconds */
	tim += tim_p->tm_sec + (tim_p->tm_min * SECSPERMIN) +
		(tim_p->tm_hour * SECSPERHOUR);

	/* compute days in year */
	days += tim_p->tm_mday - 1;
	days += _DAYS_BEFORE_MONTH[tim_p->tm_mon];
	if (tim_p->tm_mon > 1 && _DAYS_IN_YEAR (tim_p->tm_year) == 366)
		days++;

	/* compute day of the year */
	tim_p->tm_yday = days;

	if (tim_p->tm_year > 10000
		|| tim_p->tm_year < -10000)
	{
		return (time_t) -1;
	}

	/* compute days in other years */
	if (tim_p->tm_year > 70)
	{
		for (year = 70; year < tim_p->tm_year; year++)
			days += _DAYS_IN_YEAR (year);
	}
	else if (tim_p->tm_year < 70)
	{
		for (year = 69; year > tim_p->tm_year; year--)
			days -= _DAYS_IN_YEAR (year);
		days -= _DAYS_IN_YEAR (year);
	}

	/* compute day of the week */
	if ((tim_p->tm_wday = (days + 4) % 7) < 0)
		tim_p->tm_wday += 7;

	/* compute total seconds */
	tim += (days * SECSPERDAY);

	return tim;
}
