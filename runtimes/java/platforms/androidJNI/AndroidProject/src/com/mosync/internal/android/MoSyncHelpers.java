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

package com.mosync.internal.android;

import android.util.Log;
import com.mosync.internal.android.BigPhatError;

public class MoSyncHelpers
{
	private static boolean sLoggingIsOn = false;
	private static boolean sDebuggingIsOn = false;

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
        return extent >>> 16;
    }

    public static int EXTENT_Y(int extent)
    {
        return extent & 0xffff;
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