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
