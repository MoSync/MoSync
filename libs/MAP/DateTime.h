/* Copyright (C) 2010 Mobile Sorcery AB

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

#ifndef DATETIME_H_
#define DATETIME_H_

#include <maapi.h>
#include <maassert.h>
#include <mastring.h>
#include <matime.h>

#include "TimeSpan.h"

namespace MAPUtil
{
	//
	// Immutable.
	//
	class DateTime
	{
	public:
		//
		// Constructs a DateTime with default value.
		//
		DateTime( );
		//
		// Constructs a DateTime from the specified ticks.
		//
		DateTime( time_t t );
		//
		// Constructs a DateTime from the specified tm structure.
		//
		DateTime( tm* tim );
		//
		// Constructs a DateTime from the specified year, month, and day.
		// Time of day is set to 00:00:00.
		//
		DateTime( int year, int month, int day );
		//
		// Constructs a DateTime from the specified year, month, day, hour, minute, and second.
		//
		DateTime( int year, int month, int day, int hour, int minute, int second );
		//
		// Constructs a DateTime by parsing the specified string.
		//
		DateTime( const char* dateString );
		//
		// Destructs a DateTime.
		//
		~DateTime( ); // MUST NOT be virtual, to avoid vtable
		//
		// Returns the maximum value that can be represented by a DateTime.
		//
		static DateTime maxValue( );
		//
		// Returns the smallest value that can be represented by a DateTime.
		//
		static DateTime minValue( );
		//
		// returns current local time as a DateTime.
		//
		static DateTime now( );
		//
		// returns today's date in local time, as a DateTime.
		//
		static DateTime today( );
		//
		// Returns time of day as a TimeSpan from midnight.
		//
		TimeSpan timeOfDay( ) const;
		//
		// Returns number of ticks since January 1, 1970.
		// A tick is one second.
		//
		time_t getTicks( ) const;
		//
		// Returns the year component of a DateTime.
		//
		int getYear( ) const;
		//
		// Returns the month component of a DateTime.
		//
		int getMonth( ) const;
		//
		// Returns the weekday component of a DateTime.
		//
		int getWeekday( ) const;
		//
		// Returns the day of month component of a DateTime.
		//
		int getDay( ) const;
		//
		// Returns the hour component of a DateTime.
		//
		int getHour( ) const;
		//
		// Returns the minute component of a DateTime.
		//
		int getMinute( ) const;
		//
		// Returns the second component of a DateTime.
		//
		int getSecond( ) const;
		//
		// Adds a TimeSpan to current value and returns as a new DateTime.
		//
		DateTime add( TimeSpan timeSpan ) const;
		//
		// Adds specified number of seconds to current value and returns as a new DateTime.
		//
		DateTime addYears( int years ) const;
		//
		// Adds specified number of monts to current value and returns as a new DateTime.
		//
		DateTime addMonths( int months ) const;
		//
		// Adds specified number of days to current value and returns as a new Datetime.
		//
		DateTime addDays( double days ) const;
		//
		// Adds specified number of hours to current value and returns as a new DateTime.
		//
		DateTime addHours( double hours ) const;
		//
		// Adds specified number of minutes to current value and returns as a new DateTime.
		//
		DateTime addMinutes( double minutes ) const;
		//
		// Adds specified number of seconds to current value and returns as a new DateTime.
		//
		DateTime addSeconds( double seconds ) const;
		//
		// Adds ticks to current value and returns as a new DateTime.
		//
		DateTime addTicks( int ticks ) const;
		//
		// Returns time span between current value and specified DateTime as a TimeSpan.
		//
		TimeSpan subtract( DateTime ts ) const;
		//
		// Subtracts a timespan from current value and returns as a new DateTime.
		//
		DateTime subtract( TimeSpan span ) const;
	private:
		//
		// Fields
		//
		time_t mTicks;
	};
	//
	// Comparison operators.
	//
	inline bool operator > ( const DateTime& a, const DateTime& b ) 
	{
		return a.getTicks( ) > b.getTicks( ); 
	}

	inline bool operator < ( const DateTime& a, const DateTime& b ) 
	{
		return a.getTicks( ) < b.getTicks( ); 
	}

	inline bool operator >= ( const DateTime& a, const DateTime& b ) 
	{
		return a.getTicks( ) >= b.getTicks( ); 
	}

	inline bool operator <= ( const DateTime& a, const DateTime& b ) 
	{
		return a.getTicks( ) <= b.getTicks( ); 
	}

	inline bool operator == ( const DateTime& a, const DateTime& b ) 
	{
		return a.getTicks( ) == b.getTicks( ); 
	}

	inline bool operator != ( const DateTime& a, const DateTime& b ) 
	{
		return a.getTicks( ) != b.getTicks( ); 
	}
	//
	// Arithmetic binary operators.
	//
	inline TimeSpan operator - ( const DateTime d1, const DateTime d2 ) 
	{
		return TimeSpan( (int)d1.getTicks( ) - (int)d2.getTicks( ) ); 
	}
	
	inline DateTime operator - ( const DateTime d, const TimeSpan t ) 
	{ 
		return DateTime( (int)d.getTicks( ) - (int)t.getTicks( ) ); 
	}

	inline DateTime operator + ( const DateTime d, const TimeSpan t ) 
	{ 
		return DateTime( d.getTicks( ) + t.getTicks( ) ); 
	}
}

#endif // DATETIME_H_
