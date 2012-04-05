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

package com.mosync.internal.android;

import android.app.Activity;
import android.os.Build;

import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RES_UNAVAILABLE;

/**
 * Wrapper for In-app Purchase Syscalls to avoid cluttering the MoSyncSyscalls file.
 * @author emma tresanszki
 */
public class MoSyncPurchase
{
	/**
	 * Constructor Purchase API.
	 * @param thread The underlying MoSync thread.
	 */
	public MoSyncPurchase(MoSyncThread thread)
	{
		mMoSyncThread = thread;
		//mAds = new Ads(mMoSyncThread, getActivity());
		try{
			int target = IntConverter.convert( Build.VERSION.SDK );
			if ( target >= 4 )
			{
				//mPushNotificationManager = new PushNotificationsManager(thread, getActivity());
				// purchaseManager
			}
		}
		catch(PropertyConversionException pce )
		{
			return;
		}
	}

	/**
	 * @return The Activity object.
	 */
	private Activity getActivity() {
		return mMoSyncThread.getActivity();
	}

	/************************ Syscalls ************************/

	/**
	 * Internal function for the maPurchaseSupported system call.
	 * Synchronous request to check if in-app billing can be used.
	 */
	public int maPurchaseSupported()
	{
		// In-app purchase is supported only from Android 1.6 and higher.



		return MA_PURCHASE_RES_UNAVAILABLE;
	}

	/************************ Class members ************************/

	/**
	 * The MoSync thread object.
	 */
	private MoSyncThread mMoSyncThread;

	//private Ads mAds;
}
