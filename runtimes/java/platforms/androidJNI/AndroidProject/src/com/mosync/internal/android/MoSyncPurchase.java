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

import android.content.pm.PackageManager;
import android.os.Build;
import android.text.TextUtils;
import android.util.Log;

import com.mosync.internal.android.billing.BillingEvent;
import com.mosync.internal.android.billing.Consts;
import com.mosync.internal.android.billing.PurchaseManager;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RES_INVALID_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RES_UNAVAILABLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RES_BUFFER_TOO_SMALL;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_DISABLED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RES_RECEIPT_NOT_AVAILABLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RES_INVALID_FIELD_NAME;
import static com.mosync.internal.android.MoSyncHelpers.*;

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
		// Create the manager and bind service only if permission is set.
		if ( isBillingPermissionSet() )
		{
			// In-app purchase is supported only from Android 1.6 and higher.
			try{
				int target = IntConverter.convert( Build.VERSION.SDK );
				if ( target >= 4 )
				{
					SYSLOG("PurchaseManager is available, android sdk version > 4");
					mPurchaseManager = new PurchaseManager(thread);
				}
			}
			catch(PropertyConversionException pce )
			{
				return;
			}
		}
	}

	public void restoreService()
	{
		if (mPurchaseManager != null)
		{
			mPurchaseManager.restoreService();
		}
	}
	public void unbindService()
	{
		if (mPurchaseManager != null)
		{
			mPurchaseManager.unbindService();
		}
	}
	/**
	 * Check if Billing permission is set, and if not call maPanic().
	 */
	public boolean isBillingPermissionSet()
	{
		return
			PackageManager.PERMISSION_GRANTED ==
				mMoSyncThread.getActivity().checkCallingOrSelfPermission(
						"com.android.vending.BILLING");
	}

	/**
	 * Check if Billing permission is set, and if not call maPanic().
	 */
	void panicIfBillingPermissionIsNotSet()
	{
		if (!isBillingPermissionSet())
		{
			mMoSyncThread.maPanic(1,
				"Billing permission is not set in the MoSync project");
		}
	}
	/************************ Syscalls ************************/

	/**
	 * Internal function for the maPurchaseSupported system call.
	 * Synchronous request to check if in-app billing can be used.
	 */
	public int maPurchaseSupported()
	{
		panicIfBillingPermissionIsNotSet();
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
	public void maPurchaseCreate(final int productHandle, final String productID)
	{
		panicIfBillingPermissionIsNotSet();
		if (mPurchaseManager != null)
		{
			int createState = mPurchaseManager.createPurchase(productHandle, productID);
			Log.e("@@MoSync","maPurchaseCreate handle = " + productHandle + " , state = " + createState);
			mMoSyncThread.postEvent(BillingEvent.onProductCreate(
					productHandle,
					createState,
					0));

		}
		else
		{
			SYSLOG("maPurchaseCreate error: not available");
			mMoSyncThread.postEvent(BillingEvent.onProductCreate(
					productHandle,
					MA_PURCHASE_STATE_DISABLED,
					MA_PURCHASE_RES_UNAVAILABLE));
		}
	}

	/**
	 * Internal function for the maPurchaseSetPublicKey system call.
	 *
	 * @param developerPublicKey
	 * @return MA_PURCHASE_RES_MALFORMED_PUBLIC_KEY or MA_PURCHASE_RES_OK.
	 */
	public int maPurchaseSetPublicKey(String developerPublicKey)
	{
		panicIfBillingPermissionIsNotSet();
		if ( mPurchaseManager != null )
		{
			return mPurchaseManager.setKey(developerPublicKey);
		}
		Log.e("@@MoSync","maPurchaseSetPublicKey error: not available");
		return MA_PURCHASE_RES_UNAVAILABLE;
	}

	/**
	 * Internal function for the maPurchaseRequest system call.
	 * Asynchronous request for an in-app billing.
	 *
	 * @param handle
	 * @return
	 */
	public void maPurchaseRequest(int handle)
	{
		panicIfBillingPermissionIsNotSet();
		if ( mPurchaseManager != null )
		{
			mPurchaseManager.requestPurchase(handle);
		}
		else
		{
			Log.e("@@MoSync","maPurchaseRequest error: not available");
			PurchaseManager.onPurchaseStateChanged(
					MA_PURCHASE_RES_UNAVAILABLE,
					handle,
					0);
		}
	}

	/**
	 * Internal function for the maPurchaseGetName system call.
	 * Returns the product ID of an item, based on it's internal handle.
	 *
	 * @param productHandle
	 * @param memBuffer
	 * @param memBufSize
	 * @return The productID.
	 */
	public int maPurchaseGetName(int productHandle, int memBuffer, int memBufSize)
	{
		panicIfBillingPermissionIsNotSet();
		if ( mPurchaseManager != null )
		{
			String result = mPurchaseManager.getProductID(productHandle);

			if (result.length() == 0 )
			{
				Log.e("@@MoSync", "maPurchaseGetName: Invalid product handle " + productHandle);
				return MA_PURCHASE_RES_INVALID_HANDLE;
			}

			if( result.length( ) + 1 > memBufSize )
			{
				Log.e( "@@MoSync", "maPurchaseGetName: Buffer size " + memBufSize +
						" too short to hold buffer of size: " + result.length( ) + 1 );
				return MA_PURCHASE_RES_BUFFER_TOO_SMALL;
			}

			byte[] byteArray = result.getBytes();

			// Write string to MoSync memory.
			mMoSyncThread.mMemDataSection.position( memBuffer );
			mMoSyncThread.mMemDataSection.put( byteArray );
			mMoSyncThread.mMemDataSection.put( (byte)0 );

			return result.length( );
		}
		Log.e("@@MoSync","maPurchaseGetName error: not available");
		return MA_PURCHASE_RES_UNAVAILABLE;
	}

	/**
	 * Internal function for the maPurchaseGetField system call.
	 *
	 * @return The field value.
	 */
	public int maPurchaseGetField(
			final int productHandle,
			final String property,
			final int memBuffer,
			final int bufferSize)
	{
		panicIfBillingPermissionIsNotSet();
		if ( mPurchaseManager != null )
		{
			String result = mPurchaseManager.getField(productHandle, property);

			if ( TextUtils.isEmpty(result) ||
					result.equals(Consts.RECEIPT_FIELD_NOT_AVAILABLE) )
			{
				Log.e("@@MoSync", "maPurchaseGetField: The receipt field is not available " + property);
				return MA_PURCHASE_RES_INVALID_FIELD_NAME;
			}

			if (result.equals(Consts.RECEIPT_INVALID_HANDLE) )
			{
				Log.e("@@MoSync", "maPurchaseGetField: Invalid product handle " + productHandle);
				return MA_PURCHASE_RES_INVALID_HANDLE;
			}

			if ( result.equals(Consts.RECEIPT_NOT_AVAILABLE) )
			{
				Log.e("@@MoSync", "maPurchaseGetField: The product has not been purchased " + productHandle);
				return MA_PURCHASE_RES_RECEIPT_NOT_AVAILABLE;
			}

			if( result.length( ) + 1 > bufferSize )
			{
				Log.e( "@@MoSync", "maPurchaseGetField: Buffer size " + bufferSize +
						" too short to hold buffer of size: " + result.length( ) + 1 );
				return MA_PURCHASE_RES_BUFFER_TOO_SMALL;
			}

			byte[] ba = result.getBytes();

			// Write string to MoSync memory.
			mMoSyncThread.mMemDataSection.position( memBuffer );
			mMoSyncThread.mMemDataSection.put( ba );
			mMoSyncThread.mMemDataSection.put( (byte)0 );

			return result.length( );
		}
		Log.e("@@MoSync","maPurchaseGetField error: not available");
		return MA_PURCHASE_RES_UNAVAILABLE;
	}

	/**
	 * Internal function for the maPurchaseRestoreTransactions system call.
	 * Asynchronous request for restoring transations.
	 * Upon completion, the application will receive MAW_EVENT_PURCHASE_EVENT_RESTORED
	 * events.
	 */
	public void maPurchaseRestoreTransactions()
	{
		panicIfBillingPermissionIsNotSet();
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
	 * Internal fucntion for the maPurchaseVerifyReceipt system call.
	 * Asynchronous request for getting receipt state.
	 * @param handle
	 */
	public void maPurchaseVerifyReceipt(int handle)
	{
		panicIfBillingPermissionIsNotSet();
		if ( mPurchaseManager != null )
		{
			mPurchaseManager.verifyReceipt(handle);
		}
		else
		{
			Log.e("@@MoSync","maPurchaseVerifyReceipt error: not available");
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
		panicIfBillingPermissionIsNotSet();
		if (mPurchaseManager != null)
		{
			return mPurchaseManager.destroyPurchase(handle);
		}
		Log.e("@@MoSync","maPurchaseDestroy error: not available");
		return MA_PURCHASE_RES_UNAVAILABLE;
	}
	/************************ Class members ************************/

	/**
	 * The handle to the purchase manager.
	 */
	private PurchaseManager mPurchaseManager = null;
	private MoSyncThread mMoSyncThread;
}
