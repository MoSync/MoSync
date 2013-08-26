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

	protected int setSupportedOrientation(int orientation)
	{
		// Store supported orientation flag.
		mSupportedOrientations = orientation;

		Boolean isLandscape = false;
		Boolean isPortrait = false;

		if (((orientation & MA_SCREEN_ORIENTATION_PORTRAIT_UP)
				== MA_SCREEN_ORIENTATION_PORTRAIT_UP) ||
			((orientation & MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN)
				== MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN))
		{
			isPortrait = true;
		}

		if (((orientation & MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT)
				== MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT) ||
			((orientation & MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT)
				== MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT))
		{
			isLandscape = true;
		}

		// Set to FULL_SENSOR or SENSOR.
		if ( (isLandscape && isPortrait) ||
				(orientation == MA_SCREEN_ORIENTATION_DYNAMIC) )
		{
			setOrientation(
					ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR,
					ActivityInfo.SCREEN_ORIENTATION_SENSOR);
		}
		// Set to SENSOR_PORTRAIT if supported, to normal portrait otherwise.
		else if ( (orientation & MA_SCREEN_ORIENTATION_PORTRAIT)
					== MA_SCREEN_ORIENTATION_PORTRAIT)
		{
			setOrientation(
					ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT,
					ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		}
		// Set to SENSOR_LANDSCAPE is supported, to normal landscape otherwise.
		else if( (orientation & MA_SCREEN_ORIENTATION_LANDSCAPE)
					== MA_SCREEN_ORIENTATION_LANDSCAPE)
		{
			setOrientation(
					ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE,
					ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		}
		// Set to PORTRAIT_UP.
		else if ( (orientation & MA_SCREEN_ORIENTATION_PORTRAIT_UP)
					== MA_SCREEN_ORIENTATION_PORTRAIT_UP)
		{
			maScreenSetOrientationHelper(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		}
		// Set to PORTRAIT_UPSIDE_DOWN.
		else if ( (orientation & MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN)
					== MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN)
		{
			setOrientation(
					ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT,
					ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		}
		// Set to NORMAL_LANDSCAPE.
		else if( (orientation & MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT)
					== MA_SCREEN_ORIENTATION_LANDSCAPE_LEFT)
		{
			maScreenSetOrientationHelper(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		}
		// Set to REVERSE_LANDSCAPE.
		else if( (orientation & MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT)
					== MA_SCREEN_ORIENTATION_LANDSCAPE_RIGHT)
		{
			setOrientation(
					ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE,
					ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		}
		else
		{
			return MA_SCREEN_ORIENTATION_RES_INVALID_VALUE;
		}

		return MA_SCREEN_ORIENTATION_RES_OK;
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
		if (mSupportedOrientations != 0)
		{
			return mSupportedOrientations;
		}

		return MA_SCREEN_ORIENTATION_DYNAMIC;
	}

	@Deprecated
	protected int maScreenSetOrientation(int orientation)
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
			setOrientation(
					ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR,
					ActivityInfo.SCREEN_ORIENTATION_SENSOR);
		}
		else
		{
			return -1;
		}

		return 0;
	}

	private void setOrientation(int orientation, int alternativeOrientation)
	{
		if ( Build.VERSION.SDK_INT >= Build.VERSION_CODES.GINGERBREAD )
		{
			maScreenSetOrientationHelper(orientation);
		}
		else
		{
			maScreenSetOrientationHelper(alternativeOrientation);
		}
	}
}
