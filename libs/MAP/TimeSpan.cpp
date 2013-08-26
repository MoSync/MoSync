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

#include "TimeSpan.h"

namespace MAPUtil
{
	static const int TicksPerDay = 86400;
	static const int TicksPerHour = 3600;
	static const int TicksPerMinute = 60;
	static const int TicksPerSecond = 1;

	//-------------------------------------------------------------------------
	//
	// Returns a new Timespan from the specified ticks.
	//
	TimeSpan::TimeSpan( int ticks ) :
	//-------------------------------------------------------------------------
		mTicks( ticks )
	{
	}

	//-------------------------------------------------------------------------
	//
	// Returns a new TimeSpan from the specified hours, minutes, and seconds.
	//
	TimeSpan::TimeSpan( int hours, int minutes, int seconds )
	//-------------------------------------------------------------------------
	{
		mTicks = hours * TicksPerHour + minutes * TicksPerMinute + seconds * TicksPerSecond;
	}

	//-------------------------------------------------------------------------
	//
	// Returns a new TimeSpan from the specified days, hours, minutes, and seconds.
	//
	TimeSpan::TimeSpan( int days, int hours, int minutes, int seconds )
	//-------------------------------------------------------------------------
	{
		mTicks = days * TicksPerDay + hours * TicksPerHour + minutes * TicksPerMinute + seconds * TicksPerSecond;
	}

	//-------------------------------------------------------------------------
	//
	// Returns the largest value that can be represented by a TimeSpan.
	//
	TimeSpan TimeSpan::maxValue( )
	//-------------------------------------------------------------------------
	{
		return TimeSpan( 0x7fffffff );
	}

	//-------------------------------------------------------------------------
	//
	// Returns the smallest value that can be represented by a TimeSpan.
	//
	TimeSpan TimeSpan::minValue( )
	//-------------------------------------------------------------------------
	{
		return TimeSpan( 0x80000000 );
	}

	//-------------------------------------------------------------------------
	//
	// Returns zero as a TimeSpan.
	//
	TimeSpan TimeSpan::zero( )
	//-------------------------------------------------------------------------
	{
		return TimeSpan( 0 );
	}

	//-------------------------------------------------------------------------
	//
	// Returns number of ticks in time span.
	//
	time_t TimeSpan::getTicks( ) const
	//-------------------------------------------------------------------------
	{
		return mTicks;
	}

	//-------------------------------------------------------------------------
	//
	// Returns days component.
	//
	int TimeSpan::getDays( ) const
	//-------------------------------------------------------------------------
	{
		return (int)( mTicks / TicksPerDay );
	}

	//-------------------------------------------------------------------------
	//
	// Returns hours component.
	//
	int TimeSpan::getHours( ) const
	//-------------------------------------------------------------------------
	{
		return (int)( ( mTicks - getDays( ) * TicksPerDay ) / TicksPerHour );
	}

	//-------------------------------------------------------------------------
	//
	// Returns minutes component.
	//
	int TimeSpan::getMinutes( ) const
	//-------------------------------------------------------------------------
	{
		return (int)( ( mTicks - getHours( ) * TicksPerHour ) / TicksPerMinute );
	}

	//-------------------------------------------------------------------------
	//
	// Returns seconds component.
	//
	int TimeSpan::getSeconds( ) const
	//-------------------------------------------------------------------------
	{
		return (int)( mTicks - getMinutes( ) * TicksPerMinute / TicksPerSecond );
	}

	//-------------------------------------------------------------------------
	//
	// returns value in unit of days.
	//
	double TimeSpan::getTotalDays( ) const
	//-------------------------------------------------------------------------
	{
		return (double)mTicks / TicksPerDay;
	}

	//-------------------------------------------------------------------------
	//
	// Returns value in a unit of hours.
	//
	double TimeSpan::getTotalHours( ) const
	//-------------------------------------------------------------------------
	{
		return (double)mTicks / TicksPerHour;
	}

	//-------------------------------------------------------------------------
	//
	// Returns value in a unit of minutes.
	//
	double TimeSpan::getTotalMinutes( ) const
	//-------------------------------------------------------------------------
	{
		return (double)mTicks / TicksPerMinute;
	}

	//-------------------------------------------------------------------------
	//
	// Returns value in a unit of seconds.
	//
	double TimeSpan::getTotalSeconds( ) const
	//-------------------------------------------------------------------------
	{
		return (double)mTicks / TicksPerSecond;
	}

	//-------------------------------------------------------------------------
	//
	// Returns sum of two time spans, as a TimeSpan.
	//
	TimeSpan TimeSpan::add( TimeSpan ts ) const
	//-------------------------------------------------------------------------
	{
		return TimeSpan( (int)mTicks + (int)ts.mTicks );
	}

	//-------------------------------------------------------------------------
	//
	// Returns the absolute value of a TimeSpan, as a TimeSpan.
	//
	TimeSpan TimeSpan::duration( ) const
	//-------------------------------------------------------------------------
	{
		return TimeSpan( mTicks < 0 ? -(int)mTicks : (int)mTicks );
	}

	//-------------------------------------------------------------------------
	//
	// Returns negated value.
	//
	TimeSpan TimeSpan::negate( ) const
	//-------------------------------------------------------------------------
	{
		return TimeSpan( -(int)mTicks );
	}

	//-------------------------------------------------------------------------
	//
	// Returns difference between two time spans, as a TimeSpan.
	//
	TimeSpan TimeSpan::subtract( TimeSpan ts ) const
	//-------------------------------------------------------------------------
	{
		return TimeSpan( (int)mTicks - (int)ts.mTicks );
	}

	//-------------------------------------------------------------------------
	//
	// Returns a new TimeSpan from the specified days.
	//
	TimeSpan TimeSpan::fromDays( double days )
	//-------------------------------------------------------------------------
	{
		return TimeSpan( (int)( days * TicksPerDay ) );
	}

	//-------------------------------------------------------------------------
	//
	// Returns a new TimeSpan from the specified hours.
	//
	TimeSpan TimeSpan::fromHours( double hours )
	//-------------------------------------------------------------------------
	{
		return TimeSpan( (int)( hours * TicksPerHour ) );
	}

	//-------------------------------------------------------------------------
	//
	// Returns a new TimeSpan from the specified minutes.
	//
	TimeSpan TimeSpan::fromMinutes( double minutes )
	//-------------------------------------------------------------------------
	{
		return TimeSpan( (int)( minutes * TicksPerMinute ) );
	}

	//-------------------------------------------------------------------------
	//
	// Returns the specified seconds, as a TimeSpan.
	//
	TimeSpan TimeSpan::fromSeconds( double seconds )
	//-------------------------------------------------------------------------
	{
		return TimeSpan( (int)( seconds * TicksPerSecond ) );
	}
}
