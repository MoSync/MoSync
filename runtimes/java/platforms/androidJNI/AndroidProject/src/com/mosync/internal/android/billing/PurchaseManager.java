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

package com.mosync.internal.android.billing;

import java.util.Iterator;
import java.util.Map.Entry;

import android.util.Log;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.android.billing.request.BaseRequest;
import com.mosync.internal.android.billing.request.RestoreTransactions;
import com.mosync.nativeui.util.HandleTable;

import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_INVALID_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_PRODUCT_NOT_PURCHASED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_FAILED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_IN_PROGRESS;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_DUPLICATE_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_PRODUCT_VALID;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RECEIPT_PRODUCT_ID;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RECEIPT_PURCHASE_DATE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_COMPLETED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_RECEIPT_ERROR;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RECEIPT_PRICE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RECEIPT_APP_ITEM_ID;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_PUBLIC_KEY_NOT_SET;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_CONNECTION_FAILED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_UNKNOWN;
import static com.mosync.internal.android.MoSyncHelpers.*;

/**
 * The purchase manager holds all purchase info of both user
 * made requests, but also refunds.
 * The manager is responsible for handling requests.
 * @author emma
 *
 */
public class PurchaseManager extends IBillingObserver
{

	/**
	 * Constructor.
	 */
	public PurchaseManager(MoSyncThread thread)
	{
		mMoSyncThread = thread;
		mService = new BillingService();
		mService.setContext(mMoSyncThread.getActivity());
		mService.setThread(mMoSyncThread);
		mService.bindToMarketBillingService();
	}

	public int checkPurchaseSupported()
	{
		return mService.checkBillingSupported();
	}

	public int createPurchase(final int productHandle, final String productID)
	{
		PurchaseInformation newPurchase = new PurchaseInformation(productID);
		if ( m_PurchaseTable.add(productHandle, newPurchase) )
		{
			newPurchase.setHandle( productHandle );
			return MA_PURCHASE_STATE_PRODUCT_VALID;
		}

		return MA_PURCHASE_STATE_DUPLICATE_HANDLE;
	}

	/**
	 * Start a purchase request and set it to the current handle.
	 * @param handle
	 * @return
	 */
	public void requestPurchase(int handle)
	{
		if ( !mService.isPublicKeySet() )
		{
			Log.e("@@MoSync","maPurchaseRequest error: public key not set");
			mMoSyncThread.postEvent(BillingEvent.onPurchaseStateChanged(
					handle,
					MA_PURCHASE_STATE_FAILED,
					MA_PURCHASE_ERROR_PUBLIC_KEY_NOT_SET));
			return;
		}

		if ( !mService.isConnected() )
		{
			mMoSyncThread.postEvent(BillingEvent.onPurchaseStateChanged(
					handle,
					MA_PURCHASE_STATE_FAILED,
					MA_PURCHASE_ERROR_CONNECTION_FAILED));
			return;
		}

		PurchaseInformation purchase = m_PurchaseTable.get(handle);
		// Check if required: if BillingService.isPurchaseRequestInProgress(), place it on queue.
		if ( null != purchase )
		{
			// Remove pending receipt event if there is one.
			m_PendingEvents.remove(handle);

			// Send the request to the service.
			BaseRequest requestObj = mService.requestPurchase(purchase.getProductID(), handle);
			SYSLOG("@@MoSync maPurchaseRequest: requestID = " +  requestObj.getRequestID() );

			// If request was sent to the server send IN_PROGRESS event, but
			// keep in mind that RESPONSE_CODE intents can later signal errors on the server side.
			if ( requestObj.getRequestID() != Consts.BILLING_RESPONSE_INVALID_REQUEST_ID )
			{
				mCurrentPurchaseHandle = handle;

				// Send event! + message the request was sent successfully to Google Play.
				purchase.setState(MA_PURCHASE_STATE_IN_PROGRESS);
				purchase.setRequest(requestObj);

				mMoSyncThread.postEvent(BillingEvent.onPurchaseStateChanged(
						handle,
						MA_PURCHASE_STATE_IN_PROGRESS,
						0));
			}
			else
			{
				mMoSyncThread.postEvent(BillingEvent.onPurchaseStateChanged(
						handle,
						MA_PURCHASE_STATE_FAILED,
						MA_PURCHASE_ERROR_UNKNOWN)); // Though, it's more like DEVELOPER_ERROR.
			}
		}
		else
		{
			SYSLOG("@@MoSync maPurchaseRequest: Invalid product handle " + handle);
			mMoSyncThread.postEvent(BillingEvent.onPurchaseStateChanged(
					handle,
					MA_PURCHASE_STATE_FAILED,
					MA_PURCHASE_ERROR_INVALID_HANDLE));
		}
	}

	/**
	 * Set the notification id to the current purchase object.
	 * @param notificationID The notification ID received from Google Play
	 * after the purchase succeeded.
	 */
	public static void setPurchaseNotificationId(final String notificationID)
	{
		if ( mCurrentPurchaseHandle > 0 )
		{
			PurchaseInformation purchase = m_PurchaseTable.get(mCurrentPurchaseHandle);
			if ( purchase != null )
			{
				purchase.setNotificationID(notificationID);
			}
		}
	}

	public void setPurchaseState(int handle, boolean pending)
	{
		PurchaseInformation purchase = m_PurchaseTable.get(handle);
		if ( purchase != null )
		{
			purchase.getRequest().setPendingState(pending);
		}
	}

	public String getProductID(int handle)
	{
		PurchaseInformation purchase = m_PurchaseTable.get(handle);
		if ( null != purchase )
		{
			return purchase.getProductID();
		}

		return "";
	}

	public String getField(int handle, String field)
	{
		PurchaseInformation purchase = m_PurchaseTable.get(handle);
		if ( null != purchase )
		{
			if ( field.equals(MA_PURCHASE_RECEIPT_PRODUCT_ID) )
			{
				return purchase.getProductID();
			}
			if ( field.equals(MA_PURCHASE_RECEIPT_PURCHASE_DATE) )
			{
				return Long.toString(purchase.mTime);
			}
			if ( field.equals(MA_PURCHASE_RECEIPT_PRICE) )
			{
				return Integer.toString(purchase.mPrice);
			}
			if ( field.equals(MA_PURCHASE_RECEIPT_APP_ITEM_ID) )
			{
				return purchase.mPackageName;
			}
			return "Not available";
		}
		return "";
	}

	/**
	 * Send a restoreTransaction request to Google Play.
	 * After the request is sent to Google Play (i.e.
	 * request.getRequestID() != Consts.BILLING_RESPONSE_INVALID_REQUEST_ID )
	 * the app waits for PURCHASE_STATE_CHANGE messages.
	 */
	public void restoreTransactions()
	{
		RestoreTransactions request = mService.restoreTransactions();
		SYSLOG("@@MoSync maPurchaseRestoreTransactions requestID = " + request.getRequestID());
	}

	/**
	 * As the receipt is previously parsed and the events are constructed,
	 * just queue the events.
	 * First, check if a purchase request was initiated and completed for the
	 * given handle.
	 * If the product was restored/refunded, there is no point in verifying the
	 * receipt again.
	 * @param handle The product handle.
	 */
	public void verifyReceipt(int handle)
	{
		PurchaseInformation purchase = m_PurchaseTable.get(handle);
		if ( null != purchase )
		{
			// Check if a purchase was requested for the product.
			// If a product was not purchased, it means that it has no pending events.
			if ( purchase.getState() == MA_PURCHASE_STATE_COMPLETED )
			{
				int[] event = m_PendingEvents.get(handle);
				if ( null != event )
				{
					mMoSyncThread.postEvent(BillingEvent.onVerifyReceipt(
												event[2],event[3],event[4]));
				}
			}
			else if ( purchase.getState() == Consts.PURCHASE_STATE_REFUNDED )
			{
				Log.e("@@MoSync","maPurchaseVerifyReceipt cannot verify receipt for a restored product");
				// TODO send event with new error code. MA_PURCHASE_ERROR_NO_RECEIPT.
			}
			else
			{
				Log.e("@@MoSync","maPurchaseVerifyReceipt product was not purchased " + handle);
				mMoSyncThread.postEvent(BillingEvent.onVerifyReceipt(
						handle,
						MA_PURCHASE_STATE_RECEIPT_ERROR, // todo see if requires MA_PURCHASE_STATE_RECEIPT_INVALID
						MA_PURCHASE_ERROR_PRODUCT_NOT_PURCHASED));
			}
		}
		else
		{
			Log.e("@@MoSync", "maPurchaseVerifyReceipt: Invalid product handle " + handle);
			mMoSyncThread.postEvent(BillingEvent.onVerifyReceipt(
					handle,
					MA_PURCHASE_STATE_RECEIPT_ERROR,
					MA_PURCHASE_ERROR_INVALID_HANDLE));
		}
	}

	/**
	 * Generate a PublicKey instance from a string containing the
	 * Base64-encoded public key.
	 */
	public void setKey(final String developerPublicKey)
	{
		mService.setPublicKey(developerPublicKey);
	}

	public int destroyPurchase(int handle)
	{
		PurchaseInformation purchase = m_PurchaseTable.get(handle);
		if ( null != purchase )
		{
			// Remove the internal purchase object.
			m_PurchaseTable.remove(handle);
			// Remove pending receipt event if there is one.
			m_PendingEvents.remove(handle);

			return MA_PURCHASE_RES_OK;
		}

		Log.e("@@MoSync", "maPurchaseDestroy: Invalid product handle " + handle);
		return MA_PURCHASE_ERROR_INVALID_HANDLE;
	}

	public void unbindService()
	{
		if ( mService != null )
			mService.unbind();
	}

	@Override
	public void onTransactionInformationReceived(PurchaseInformation purchase)
	{
		// Get the productHandle, based on productID.
		mMoSyncThread.postEvent(BillingEvent.onPurchaseStateChanged(
				purchase.getHandle(),
				MA_PURCHASE_STATE_COMPLETED,
				0));
	}

	@Override
	public void onPurchaseRestored(PurchaseInformation purchase)
	{
		// Create new product handle.
		int handle = m_PurchaseTable.add(purchase);
		m_PurchaseTable.get(handle).setHandle(handle);
		// Post MoSync event (state is not required).
		mMoSyncThread.postEvent(BillingEvent.onRestoreTransaction(0, handle));
	}

	@Override
    public void onPurchaseStateChanged(int state, int handle, int error)
    {
		mMoSyncThread.postEvent(BillingEvent.onPurchaseStateChanged(handle, state, error));
    }

	@Override
    public void onReceiptEvent(int[] event, int handle)
    {
		m_PendingEvents.add(handle, event);
    }

	/**
	 * Search a product, based on productID.
	 * @param productID The product id.
	 * @return the product handle.
	 *
	public int searchProduct(String productID)
	{
		Iterator it = m_PurchaseTable.getIterator();
		while(it.hasNext())
		{
			Entry<Integer, PurchaseInformation> entry = (Entry) it.next();
			PurchaseInformation info = entry.getValue();
			if ( info.getProductID().equals(productID) )
			{
				return entry.getKey();
			}
		}
		return -1;
	}*/
	/************************ Class members ************************/

	/**
	 * The MoSync thread object.
	 */
	private MoSyncThread mMoSyncThread;
	private BillingService mService;

	/**
	 * -1 means no active purchase. todo add constant
	 */
	private static int mCurrentPurchaseHandle = -1;

	/**
	 * A table that contains a mapping between a handle and a purchase object.
	 */
	private static HandleTable<PurchaseInformation> m_PurchaseTable =
		new HandleTable<PurchaseInformation>();

    /**
     * Pending events for each product. The events are related to the receipt validation,
     * as they will be sent to the application only after maPurchaseVerifyReceipt is called.
     */
	private HandleTable<int[]> m_PendingEvents = new HandleTable<int[]>();
}