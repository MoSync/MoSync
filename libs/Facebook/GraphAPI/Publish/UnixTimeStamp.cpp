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
 * UnixTimeStamp.cpp
 *
 *  Created on: Jul 14, 2011
 *      Author: gabi
 */

#include "UnixTimeStamp.h"

UnixTimeStamp::UnixTimeStamp()
{
}

UnixTimeStamp::UnixTimeStamp(const Date &date, const Time &time)
{
	set(date, time);
}

void UnixTimeStamp::set(const Date &date, const Time &time)
{
	MAUtil::String dateStr = compose_ISO_8601_Date(date);
	MAUtil::String timeStr = compose_ISO_8601_Time(time);

	mUnixTimeStamp = dateStr + "T" + timeStr;
}

const MAUtil::String &UnixTimeStamp::getTimeStamp() const
{
	return mUnixTimeStamp;
}

bool UnixTimeStamp::isEmpty() const
{
	return (0 == mUnixTimeStamp.size());
}

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
