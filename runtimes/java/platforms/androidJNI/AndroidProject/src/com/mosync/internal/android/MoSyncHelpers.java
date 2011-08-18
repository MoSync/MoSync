/* Copyright (C) 2010 MoSync AB

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

package com.mosync.internal.android;

import android.util.Log;
import com.mosync.internal.android.BigPhatError;

public class MoSyncHelpers
{
	private static boolean sLoggingIsOn = false;
	private static boolean sDebuggingIsOn = true;
	
	public static void SyslogOn(boolean on)
	{
		sLoggingIsOn = on;
	}

	public static void Syslog(String message)
	{
		if (sLoggingIsOn)
		{
			Log.i("@@@ MoSync SYSLOG", message);
		}
	}
	
	public static void SYSLOG(String message)
	{
		Syslog(message);
	}
	
	public static void DebugOn(boolean on)
	{
		sDebuggingIsOn = on;
	}

	public static boolean DebugIsOn()
	{
		return sDebuggingIsOn;
	}

	public static void DebugPrint(String message)
	{
		if (sDebuggingIsOn)
		{
			Log.i("@@@ MoSync DEBUG", message);
		}
	}
	
	//#define EXTENT_Y(e) ((short)(e))
	//#define EXTENT_X(e) ((short)((e) >> 16))
	//#define EXTENT(x, y) ((MAExtent)((((int)(x)) << 16) | ((y) & 0x0FFFF)))

	public static int EXTENT_X(int extent)
	{
		return ((short) extent) >> 16;
	}
	
	public static int EXTENT_Y(int extent)
	{
		return (short) extent;
	}
	
	public static int EXTENT(int x, int y)
	{
		return (x << 16) | (y & 0x0FFFF);
	}
	
	/**
	 * TODO: Make version of MYASSET that takes error message.
	 * @param state
	 */
	public static void MYASSERT(boolean state) 
	{
		if (!state) 
		{
			new Exception("MYASSERT failed").printStackTrace();
			throw new BigPhatError("Error thrown in MYASSERT");
			// TODO: Or panic.
		}
	}
}