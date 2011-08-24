/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/*
 * UnixTimeStamp.h
 *
 *  Created on: Jul 14, 2011
 *      Author: gabi
 */

#ifndef UNIXTIMESTAMP_H_
#define UNIXTIMESTAMP_H_

#include <MAUtil/String.h>

/*
 * Helper structure for holding the day, month and year values, that compose a date.
 */
struct Date
{
	Date(const MAUtil::String &year, const MAUtil::String &month="", const MAUtil::String &day="")
		: mYear(year), mMonth(month), mDay(day) {}
	MAUtil::String mYear;
	MAUtil::String mMonth;
	MAUtil::String mDay;
};

/*
 * Helper structure for holding the hour, minute and the second values, that compose a time point.
 */
struct Time
{
	Time(const MAUtil::String &hour, const MAUtil::String &minute="", const MAUtil::String &second="")
		: mHour(hour), mMinute(minute), mSecond(second) {}
	MAUtil::String mHour;
	MAUtil::String mMinute;
	MAUtil::String mSecond;
};

/*
 * Class that implements a Unix time stamp.
 */
class UnixTimeStamp
{

public:
	UnixTimeStamp();

	/*
	 * constructor
	 * @param date - the calendar Date
	 * @param time - the time point
	 */
	UnixTimeStamp(const Date &date, const Time &time);

	/*
	 * initializes a UnixTimeStamp object.
	 * @param date - the calendar Date
	 * @param time - the time point
	 */
	void set(const Date &date, const Time &time);

	/*
	 * returns the time point
	 */
	const MAUtil::String &getTimeStamp() const;

	/*
	 * returns true if the UnixTimeStamp object is not initialized.
	 */
	bool isEmpty() const;

private:
	/*
	 * helper function that creates a string holding a calendar date in the form: YYYY-MM-DD (according to the
	 * ISO_8601 standard).
	 * @param date - the calendar date, from which the string is formed.
	 */
	MAUtil::String compose_ISO_8601_Date(const Date &date);

	/*
	 * helper function that creates a string holding a time point in the form: hh:mm:ss (according to the
	 * ISO_8601 standard).
	 * @param time - the time point, from which the string is formed.
	 */
	MAUtil::String compose_ISO_8601_Time(const Time &time);

private:
	MAUtil::String mUnixTimeStamp;
};

#endif /* UNIXTIMESTAMP_H_ */
