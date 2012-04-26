/* Copyright (C) 2012 MoSync AB

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

import android.os.Build;
import android.util.Log;

import com.mosync.internal.android.billing.PurchaseManager;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.java.android.MoSync;

import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_INVALID_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RES_UNAVAILABLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_INVALID_STRING_BUFFER_SIZE;

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
		// In-app purchase is supported only from Android 1.6 and higher.
		try{
			int target = IntConverter.convert( Build.VERSION.SDK );
			if ( target >= 4 )
			{
				mPurchaseManager = new PurchaseManager(thread);
			}
		}
		catch(PropertyConversionException pce )
		{
			return;
		}
	}

	public void unbindService()
	{
		if (mPurchaseManager != null)
		{
			mPurchaseManager.unbindService();
		}
	}
	/************************ Syscalls ************************/

	/**
	 * Internal function for the maPurchaseSupported system call.
	 * Synchronous request to check if in-app billing can be used.
	 */
	public int maPurchaseSupported()
	{
		if ( mPurchaseManager != null)
		{
			return mPurchaseManager.checkPurchaseSupported();
		}

		return MA_PURCHASE_RES_UNAVAILABLE;
	}

	/**
	 * Internal function for the maPurchaseCreate system call.
	 * Creates an entry in the purchase table.
	 * @param productID
	 * @return The purchase handle.
	 */
	public int maPurchaseCreate(String productID)
	{
		if (mPurchaseManager != null)
		{
			return mPurchaseManager.createPurchase(productID);
		}
		else
		{
			Log.e("@@MoSync","maPurchaseCreate error: not available");
			return MA_PURCHASE_RES_UNAVAILABLE;
		}
	}

	/**
	 * Internal function for the maPurchaseSetPublicKey system call.
	 *
	 * @param developerPublicKey
	 * @return
	 */
	public int maPurchaseSetPublicKey(String developerPublicKey)
	{
		if ( mPurchaseManager != null )
		{
//			if ( developerPublicKey.length() > 50 )
//			{
//				return -1;
//			}
			return mPurchaseManager.setKey(developerPublicKey);
		}
		else
		{
			Log.e("@@MoSync","maPurchaseSetPublicKey error: not available");
			return MA_PURCHASE_RES_UNAVAILABLE;
		}
	}

	/**
	 * Internal function for the maPurchaseRequest system call.
	 * Asynchronous request for an in-app billing.
	 *
	 * @param handle
	 * @return
	 */
	public int maPurchaseRequest(int handle)
	{
		if ( mPurchaseManager != null )
		{
			return mPurchaseManager.requestPurchase(handle);
		}
		else
		{
			Log.e("@@MoSync","maPurchaseRequest error: not available");
			return MA_PURCHASE_RES_UNAVAILABLE;
		}
	}

	/**
	 * Internal function for the maPurchaseGetName system call.
	 * Returns the product ID of an item, based on it's internal handle.
	 *
	 * @param productHandle
	 * @param memBuffer
	 * @param memBufSize
	 * @return
	 */
	public int maPurchaseGetName(int productHandle, int memBuffer, int memBufSize)
	{
		if ( mPurchaseManager != null )
		{
			String result = mPurchaseManager.getProductID(productHandle);

			if (result.isEmpty() )
			{
				Log.e("@@MoSync", "maPurchaseGetName: Invalid product handle " + productHandle);
				return MA_PURCHASE_ERROR_INVALID_HANDLE;
			}

			if( result.length( ) + 1 > memBufSize )
			{
				Log.e( "@@MoSync", "maPurchaseGetName: Buffer size " + memBufSize +
						" too short to hold buffer of size: " + result.length( ) + 1 );
//				return -2;
				return MA_PURCHASE_ERROR_INVALID_STRING_BUFFER_SIZE;
			}

			byte[] ba = result.getBytes();

			// Write string to MoSync memory.
			mMoSyncThread.mMemDataSection.position( memBuffer );
			mMoSyncThread.mMemDataSection.put( ba );
			mMoSyncThread.mMemDataSection.put( (byte)0 );

//			return result.length( );
			return MA_PURCHASE_RES_OK;
		}
		else
		{
			Log.e("@@MoSync","maPurchaseGetName error: not available");
			return MA_PURCHASE_RES_UNAVAILABLE;
		}
	}

	/**
	 * Internal function for the maPurchaseRestoreTransactions system call.
	 * Asynchronous request for restoring transations.
	 * Upon completion, the application will receive MAW_EVENT_PURCHASE_EVENT_RESTORED
	 * events.
	 */
	public void maPurchaseRestoreTransactions()
	{
		if ( mPurchaseManager != null )
		{
			mPurchaseManager.restoreTransactions();
		}
		else
		{
			Log.e("@@MoSync","maPurchaseRequest error: not available");
		}
	}

	/**
	 * Internal function for the maPurchaseDestroy system call.
	 *
	 * @param handle
	 * @return
	 */
	public int maPurchaseDestroy(int handle)
	{
		if (mPurchaseManager != null)
		{
			return mPurchaseManager.destroyPurchase(handle);

		}
		else
		{
			Log.e("@@MoSync","maPurchaseDestroy error: not available");
			return MA_PURCHASE_RES_UNAVAILABLE;
		}
	}
	/************************ Class members ************************/

	/**
	 * The handle to the purchase manager.
	 */
	private PurchaseManager mPurchaseManager;
	private MoSyncThread mMoSyncThread;
}
