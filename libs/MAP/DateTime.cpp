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

#include <matime.h>
#include <mastdlib.h>

#include "DateTime.h"

namespace MAPUtil
{
	//
	// Constants
	//
	static const int TicksPerDay = 86400;
	static const int TicksPerHour = 3600;
	static const int TicksPerMinute = 60;
	static const int TicksPerSecond = 1;
	//
	// Months
	//
	static const char* sMonth[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	//static const int sMonth_to_day[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

	//-------------------------------------------------------------------------
	//
	// Parses a string of the format "Tue, 19 May 2009 18:06:18 +0200"
	//
	static time_t parse( const char* dateString )
	//-------------------------------------------------------------------------
	{
		const char* p = dateString;

		// Skip day of week
		p = strchr( p, ' ' );
		p++;

		// Get date
		int d = (int)strtod( p, (char**)&p );
		p++;

		// Get month as string
		char buf[30];
		char* b = buf;
		while (*p != ' ' )
			*b++ = *p++;
		*b = '\0';

		// Convert month to integer
		int m; // zero-based
		for ( m = 0; m < 11; m++ )
			if ( strcmp( buf, sMonth[m] ) == 0 )
				break;
		p++;

		// Get year
		int y = (int)strtod( p, (char**)&p );
		// normalize
		if ( y > 1900 ) y -= 1900;
		p++;

		// Get hours
		int hh = (int)strtod( p, (char**)&p );
		p++;

		// Get minutes
		int mm = (int)strtod( p, (char**)&p );
		p++;

		// Get seconds
		int ss = (int)strtod( p, (char**)&p );
		p++;

		tm tim;
		tim.tm_hour = hh;
		tim.tm_min = mm;
		tim.tm_sec = ss;
		tim.tm_year = y;
		tim.tm_mon = m;
		tim.tm_mday = d;

		tim.tm_isdst = -1;
		return mktime( &tim );
	}

	// NOTE: Keep for now

	////-------------------------------------------------------------------------
	//static void test( )
	////-------------------------------------------------------------------------
	//{
	//	tm tim;
	//	tim.tm_hour = 0;
	//	tim.tm_min = 0;
	//	tim.tm_sec = 0;
	//	tim.tm_year = 70;
	//	tim.tm_mon = 0;
	//	tim.tm_mday = 1;
	//	time_t ticks = mktime( &tim );
	//	tm tim2;
	//	split_time( ticks, &tim2 );
	//}

	// END NOTE

	//-------------------------------------------------------------------------
	//
	// Constructs a DateTime with default value.
	//
	DateTime::DateTime( ) :
	//-------------------------------------------------------------------------
		mTicks( 0 )
	{
	}

	//-------------------------------------------------------------------------
	//
	// Constructs a DateTime from the specified ticks.
	//
	DateTime::DateTime( time_t t ) :
	//-------------------------------------------------------------------------
		mTicks( t )
	{
	}

	//-------------------------------------------------------------------------
	//
	// Constructs a DateTime from the specified tm structure.
	//
	DateTime::DateTime( tm* tim ) :
	//-------------------------------------------------------------------------
		mTicks( mktime( tim ) )
	{
	}

	//-------------------------------------------------------------------------
	//
	// Constructs a DateTime from the specified year, month, and day.
	// Time of day is set to 00:00:00.
	//
	DateTime::DateTime( int year, int month, int day )
	//-------------------------------------------------------------------------
	{
		tm tim;
		tim.tm_hour = 0;
		tim.tm_min = 0;
		tim.tm_sec = 0;
		tim.tm_year = year;
		tim.tm_mon = month;
		tim.tm_mday = day;
		mTicks = mktime( &tim );
	}

	//-------------------------------------------------------------------------
	//
	// Constructs a DateTime from the specified year, month, day, hour, minute, and second.
	//
	DateTime::DateTime( int year, int month, int day, int hour, int minute, int second )
	//-------------------------------------------------------------------------
	{
		tm tim;
		tim.tm_hour = hour;
		tim.tm_min = minute;
		tim.tm_sec = second;
		tim.tm_year = year;
		tim.tm_mon = month;
		tim.tm_mday = day;
		mTicks = mktime( &tim );
	}

	//-------------------------------------------------------------------------
	//
	// Constructs a DateTime by parsing the specified string.
	//
	DateTime::DateTime( const char* dateString )
	//-------------------------------------------------------------------------
	{
		mTicks = parse( dateString );
	}

	//-------------------------------------------------------------------------
	//
	// Destructs a DateTime.
	//
	DateTime::~DateTime( )
	//-------------------------------------------------------------------------
	{
	}

	//-------------------------------------------------------------------------
	//
	// Returns the maximum value that can be represented by a DateTime.
	//
	DateTime DateTime::maxValue( )
	//-------------------------------------------------------------------------
	{
		return DateTime( 0x7fffffff );
	}

	//-------------------------------------------------------------------------
	//
	// Returns the smallest value that can be represented by a DateTime.
	//
	DateTime DateTime::minValue( )
	//-------------------------------------------------------------------------
	{
		return DateTime( (time_t)0 );
	}

	//-------------------------------------------------------------------------
	//
	// returns current local time as a DateTime.
	//
	DateTime DateTime::now( )
	//-------------------------------------------------------------------------
	{
		return DateTime( maLocalTime( ) );
	}

	//-------------------------------------------------------------------------
	//
	// returns today's date in local time, as a DateTime.
	//
	DateTime DateTime::today( )
	//-------------------------------------------------------------------------
	{
		time_t now = maLocalTime( );
		tm tim;
		split_time( now, &tim );
		tim.tm_hour = 0;
		tim.tm_min = 0;
		tim.tm_sec = 0;
		return DateTime( mktime( &tim ) );
	}

	//-------------------------------------------------------------------------
	//
	// Returns time of day as a TimeSpan from midnight.
	//
	TimeSpan DateTime::timeOfDay( ) const
	//-------------------------------------------------------------------------
	{
		return TimeSpan( getHour( ), getMinute( ), getSecond( ) );
	}

	//-------------------------------------------------------------------------
	//
	// Returns number of ticks since January 1, 1970.
	// A tick is one second.
	//
	time_t DateTime::getTicks( ) const
	//-------------------------------------------------------------------------
	{
		return mTicks;
	}

	//-------------------------------------------------------------------------
	//
	// Returns the year component of a DateTime.
	//
	int DateTime::getYear( ) const
	//-------------------------------------------------------------------------
	{
		tm tim;
		split_time( mTicks, &tim );
		return tim.tm_year;
	}

	//-------------------------------------------------------------------------
	//
	// Returns the month component of a DateTime.
	//
	int DateTime::getMonth( ) const
	//-------------------------------------------------------------------------
	{
		tm tim;
		split_time( mTicks, &tim );
		return tim.tm_mon;
	}

	//-------------------------------------------------------------------------
	//
	// Returns the weekday component of a DateTime.
	//
	int DateTime::getWeekday( ) const
	//-------------------------------------------------------------------------
	{
		tm tim;
		split_time( mTicks, &tim );
		return tim.tm_wday;
	}

	//-------------------------------------------------------------------------
	//
	// Returns the day component of a DateTime.
	//
	int DateTime::getDay( ) const
	//-------------------------------------------------------------------------
	{
		tm tim;
		split_time( mTicks, &tim );
		return tim.tm_mday;
	}

	//-------------------------------------------------------------------------
	//
	// Returns the hour component of a DateTime.
	//
	int DateTime::getHour( ) const
	//-------------------------------------------------------------------------
	{
		tm tim;
		split_time( mTicks, &tim );
		return tim.tm_hour;
	}

	//-------------------------------------------------------------------------
	//
	// Returns the minute component of a DateTime.
	//
	int DateTime::getMinute( ) const
	//-------------------------------------------------------------------------
	{
		tm tim;
		split_time( mTicks, &tim );
		return tim.tm_min;
	}

	//-------------------------------------------------------------------------
	//
	// Returns the second component of a DateTime.
	//
	int DateTime::getSecond( ) const
	//-------------------------------------------------------------------------
	{
		tm tim;
		split_time( mTicks, &tim );
		return tim.tm_sec;
	}

	//-------------------------------------------------------------------------
	//
	// Adds a TimeSpan to current value and returns as a new DateTime.
	//
	DateTime DateTime::add( TimeSpan timeSpan ) const
	//-------------------------------------------------------------------------
	{
		return DateTime( mTicks + timeSpan.getTicks( ) );
	}

	//-------------------------------------------------------------------------
	//
	// Adds specified number of seconds to current value and returns as a new DateTime.
	//
	DateTime DateTime::addYears( int years ) const
	//-------------------------------------------------------------------------
	{
		tm tim;
		split_time( mTicks, &tim );
		tim.tm_year += years;
		return DateTime( mktime( &tim ) );
	}

	//-------------------------------------------------------------------------
	//
	// Adds specified number of monts to current value and returns as a new DateTime.
	//
	DateTime DateTime::addMonths( int months ) const
	//-------------------------------------------------------------------------
	{
		tm tim;
		split_time( mTicks, &tim );
		tim.tm_mon += months;
		while ( tim.tm_mon >= 12 )
        {
			tim.tm_mon -= 12;
            tim.tm_year++;
        }
		return DateTime( mktime( &tim ) );
	}

	//-------------------------------------------------------------------------
	//
	// Adds specified number of days to current value and returns as a new Datetime.
	//
	DateTime DateTime::addDays( double days ) const
	//-------------------------------------------------------------------------
	{
		return DateTime( mTicks + (int)( days * TicksPerDay ) );
	}

	//-------------------------------------------------------------------------
	//
	// Adds specified number of hours to current value and returns as a new DateTime.
	//
	DateTime DateTime::addHours( double hours ) const
	//-------------------------------------------------------------------------
	{
		return DateTime( mTicks + (int)( hours * TicksPerHour ) );
	}

	//-------------------------------------------------------------------------
	//
	// Adds specified number of minutes to current value and returns as a new DateTime.
	//
	DateTime DateTime::addMinutes( double minutes ) const
	//-------------------------------------------------------------------------
	{
		return DateTime( mTicks + (int)( minutes * TicksPerMinute ) );
	}

	//-------------------------------------------------------------------------
	//
	// Adds specified number of seconds to current value and returns as a new DateTime.
	//
	DateTime DateTime::addSeconds( double seconds ) const
	//-------------------------------------------------------------------------
	{
		return DateTime( mTicks + (int)( seconds * TicksPerSecond ) );
	}

	//-------------------------------------------------------------------------
	//
	// Adds ticks to current value and returns as a new DateTime.
	//
	DateTime DateTime::addTicks( int ticks ) const
	//-------------------------------------------------------------------------
	{
		return DateTime( mTicks + ticks );
	}

	//-------------------------------------------------------------------------
	//
	// Returns time span between current value and specified DateTime as a TimeSpan.
	//
	TimeSpan DateTime::subtract( DateTime ts ) const
	//-------------------------------------------------------------------------
	{
		return TimeSpan( (int)mTicks - (int)ts.getTicks( ) );
	}

	//-------------------------------------------------------------------------
	//
	// Subtracts a timespan from current value and returns as a new DateTime.
	//
	DateTime DateTime::subtract( TimeSpan span ) const
	//-------------------------------------------------------------------------
	{
		return DateTime( mTicks - span.getTicks( ) );
	}
}
