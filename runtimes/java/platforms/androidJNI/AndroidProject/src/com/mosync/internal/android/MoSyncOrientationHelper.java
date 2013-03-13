/* Copyright (C) 2013 MoSync AB

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

package com.mosync.internal.android;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.os.Build;

import com.mosync.java.android.MoSync;

import static com.mosync.internal.generated.MAAPI_consts.MA_SCREEN_ORIENTATION_LANDSCAPE;
import static com.mosync.internal.generated.MAAPI_consts.MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT;
import static com.mosync.internal.generated.MAAPI_consts.MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT;
import static com.mosync.internal.generated.MAAPI_consts.MA_SCREEN_ORIENTATION_PORTRAIT;
import static com.mosync.internal.generated.MAAPI_consts.MA_SCREEN_ORIENTATION_PORTRAIT_UP;
import static com.mosync.internal.generated.MAAPI_consts.MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN;
import static com.mosync.internal.generated.MAAPI_consts.MA_SCREEN_ORIENTATION_RES_INVALID_VALUE;
import static com.mosync.internal.generated.MAAPI_consts.MA_SCREEN_ORIENTATION_RES_NOT_SUPPORTED;
import static com.mosync.internal.generated.MAAPI_consts.MA_SCREEN_ORIENTATION_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_SCREEN_ORIENTATION_DYNAMIC;
import static com.mosync.internal.generated.MAAPI_consts.SCREEN_ORIENTATION_DYNAMIC;
import static com.mosync.internal.generated.MAAPI_consts.SCREEN_ORIENTATION_LANDSCAPE;
import static com.mosync.internal.generated.MAAPI_consts.SCREEN_ORIENTATION_PORTRAIT;

/**
 * Helper class for handling orientation related syscalls.
 * @author emma
 *
 */
public class MoSyncOrientationHelper {
	/**
	 * This is the MoSync Activity.
	 */
	private final MoSync mContext;

	private int mSupportedOrientations = 0x000000;

	/**
	 * Orientation helper constructor.
	 * @param context The MoSync activity.
	 */
	public MoSyncOrientationHelper(Context context)
	{
		mContext = (MoSync) context;
	}

	/**
	 * Helper for setting supported orientations.
	 * @param activityOrientation Constant from ActivityInfo.
	 * @return <0 in case of error.
	 */
	private int setOrientationIfSupported(int activityOrientation)
	{
		if ( Build.VERSION.SDK_INT >= Build.VERSION_CODES.GINGERBREAD )
		{
			maScreenSetOrientationHelper( activityOrientation );

			if (mSupportedOrientations != 0x000000)
				mSupportedOrientations |= activityOrientation;
			else
				mSupportedOrientations = activityOrientation;
			return MA_SCREEN_ORIENTATION_RES_OK;
		}
		return MA_SCREEN_ORIENTATION_RES_NOT_SUPPORTED;
	}

	private int setDynamicOrientation()
	{
		int fullSensorAvailable = setOrientationIfSupported(ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR);
		if ( fullSensorAvailable != MA_SCREEN_ORIENTATION_RES_OK )
			return setOrientationIfSupported(ActivityInfo.SCREEN_ORIENTATION_SENSOR);
		return fullSensorAvailable;
	}

	protected int setSupportedOrientation(int orientation)
	{
		// Initialize supported orientation flag.
		mSupportedOrientations = 0x000000;

		// Set to FULL_SENSOR or SENSOR.
		if ( ( (orientation & MA_SCREEN_ORIENTATION_PORTRAIT) == MA_SCREEN_ORIENTATION_PORTRAIT)
				&& ( (orientation & MA_SCREEN_ORIENTATION_LANDSCAPE) == MA_SCREEN_ORIENTATION_LANDSCAPE) )
		{
			return setDynamicOrientation();
		}
		if ( (orientation & MA_SCREEN_ORIENTATION_DYNAMIC) == MA_SCREEN_ORIENTATION_DYNAMIC)
		{
			return setDynamicOrientation();
		}

		// Set to SENSOR_PORTRAIT if supported, to normal portrait otherwise.
		if ( ( (orientation & MA_SCREEN_ORIENTATION_PORTRAIT_UP) == MA_SCREEN_ORIENTATION_PORTRAIT_UP)
			&& ( (orientation & MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN) == MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN) )
		{
			int orientationSupported = setOrientationIfSupported(ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT);
			if (orientationSupported != MA_SCREEN_ORIENTATION_RES_OK)
			{
				return setOrientationIfSupported(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
			}
			return orientationSupported;
		}

		// Set to SENSOR_LANDSCAPE is supported, to normal landscape otherwise.
		if ( ( (orientation & MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT) == MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT)
				&& ( (orientation & MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT) == MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT) )
		{
			int orientationSupported = setOrientationIfSupported(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
			if (orientationSupported != MA_SCREEN_ORIENTATION_RES_OK)
			{
				return setOrientationIfSupported(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
			}
			return orientationSupported;
		}

		// Set to SENSOR_PORTRAIT.
		if ( (orientation & MA_SCREEN_ORIENTATION_PORTRAIT)
					== MA_SCREEN_ORIENTATION_PORTRAIT)
		{
			return setOrientationIfSupported(ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT);
		}

		// Set to SENSOR_LANDSCAPE.
		if( (orientation & MA_SCREEN_ORIENTATION_LANDSCAPE)
					== MA_SCREEN_ORIENTATION_LANDSCAPE)
		{
			return setOrientationIfSupported(ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
		}

		// Set to PORTRAIT_UP.
		if ( (orientation & MA_SCREEN_ORIENTATION_PORTRAIT_UP)
					== MA_SCREEN_ORIENTATION_PORTRAIT_UP)
		{
			return setOrientationIfSupported(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		}

		// Set to PORTRAIT_UPSIDE_DOWN.
		if ( (orientation & MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN)
					== MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN)
		{
			return setOrientationIfSupported(ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT);
		}

		// Set to NORMAL_LANDSCAPE.
		if( (orientation & MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT)
				== MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT)
		{
			return setOrientationIfSupported(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		}

		// Set to REVERSE_LANDSCAPE.
		if( (orientation & MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT)
					== MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT)
		{
			return setOrientationIfSupported(ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE);
		}

		//setOrientationIfSupported(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

		return MA_SCREEN_ORIENTATION_RES_INVALID_VALUE;
	}

	private void maScreenSetOrientationHelper(final int androidScreenOrientation)
	{
		final Activity activity = mContext;

		activity.runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				activity.setRequestedOrientation(androidScreenOrientation);
			}
		});
	}

	protected int getSupportedOrientations()
	{
		if (mSupportedOrientations != 0x000000)
		{
			return mSupportedOrientations;
		}

		return MA_SCREEN_ORIENTATION_PORTRAIT_UP;
//		if ( Build.VERSION.SDK_INT >= Build.VERSION_CODES.GINGERBREAD )
//		{
//			return MA_SCREEN_ORIENTATION_LANDSCAPE |
//					MA_SCREEN_ORIENTATION_PORTRAIT |
//					MA_SCREEN_ORIENTATION_DYNAMIC;
//		}
//		else
//		{
//			return MA_SCREEN_ORIENTATION_PORTRAIT_UP |
//					MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT |
//					MA_SCREEN_ORIENTATION_DYNAMIC;
//		}
	}

	@Deprecated
	protected int setOrientation(int orientation)
	{
		if (SCREEN_ORIENTATION_LANDSCAPE == orientation)
		{
			maScreenSetOrientationHelper(
				ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		}
		else if (SCREEN_ORIENTATION_PORTRAIT == orientation)
		{
			maScreenSetOrientationHelper(
				ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		}
		else if (SCREEN_ORIENTATION_DYNAMIC == orientation)
		{
			if ( Build.VERSION.SDK_INT >= Build.VERSION_CODES.GINGERBREAD )
			{
				maScreenSetOrientationHelper(
						ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR);
			}
			else
			{
				maScreenSetOrientationHelper(
						ActivityInfo.SCREEN_ORIENTATION_SENSOR);
			}
		}
		else
		{
			return -1;
		}

		return 0;
	}
}
