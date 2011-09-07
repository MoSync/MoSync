/*
Copyright (C) 2011 MoSync AB

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
 * UnixTimeStamp.cpp
 */


#include "UnixTimeStamp.h"

/*
 * default constructor
 */
UnixTimeStamp::UnixTimeStamp()
{

}

/*
 * constructor
 * @param date - the calendar Date
 * @param time - the time point
 */
UnixTimeStamp::UnixTimeStamp(const Date &date, const Time &time)
{
	set(date, time);
}

/*
 * initializes a UnixTimeStamp object.
 * @param date - the calendar Date
 * @param time - the time point
 */
void UnixTimeStamp::set(const Date &date, const Time &time)
{
	MAUtil::String dateStr = compose_ISO_8601_Date(date);
	MAUtil::String timeStr = compose_ISO_8601_Time(time);

	mUnixTimeStamp = dateStr + "T" + timeStr;
}

/*
 * returns the time point
 */
const MAUtil::String &UnixTimeStamp::getTimeStamp() const
{
	return mUnixTimeStamp;
}

/*
 * returns true if the UnixTimeStamp object is not initialized.
 */
bool UnixTimeStamp::isEmpty() const
{
	return (0 == mUnixTimeStamp.size());
}

/*
 * helper function that creates a string holding a calendar date in the form: YYYY-MM-DD (according to the
 * ISO_8601 standard).
 * @param date - the calendar date, from which the string is formed.
 */
MAUtil::String UnixTimeStamp::compose_ISO_8601_Date(const Date &date)
{
	MAUtil::String temp;
	temp = date.mYear;
	if(date.mMonth.size()>0)
	{
		temp += "-";
		temp += date.mMonth;
	}
	if(date.mDay.size()>0)
	{
		temp += "-";
		temp += date.mDay;
	}
	return temp;
}

/*
 * helper function that creates a string holding a time point in the form: hh:mm:ss (according to the
 * ISO_8601 standard).
 * @param time - the time point, from which the string is formed.
 */
MAUtil::String UnixTimeStamp::compose_ISO_8601_Time(const Time &time)
{
	MAUtil::String temp;
	temp = time.mHour;

	temp += ":";
	if(time.mMinute.size()>0)
	{
		temp += time.mMinute;
	}
	else
	{
		temp += "00";
	}

	temp += ":";
	if(time.mSecond.size()>0)
	{
		temp += time.mSecond;
	}
	else
	{
		temp += "00";
	}
	return temp;
}
