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

import android.util.Log;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.android.billing.request.BaseRequest;
import com.mosync.internal.android.billing.request.RestoreTransactions;
import com.mosync.nativeui.util.HandleTable;

import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_INVALID_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_NO_RECEIPT;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_FAILED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_IN_PROGRESS;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_DUPLICATE_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_PRODUCT_VALID;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RECEIPT_PRODUCT_ID;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RECEIPT_PURCHASE_DATE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RECEIPT_TRANSACTION_ID;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_COMPLETED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_RECEIPT_ERROR;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_RECEIPT_VALID;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RECEIPT_APP_ITEM_ID;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_PUBLIC_KEY_NOT_SET;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_CONNECTION_FAILED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_UNKNOWN;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_RESTORE_ERROR;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_PRODUCT_RESTORED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_PRODUCT_REFUNDED;
import static com.mosync.internal.android.MoSyncHelpers.*;

/**
 * The purchase manager holds all purchase info of both user
 * made requests, but also refunds.
 * The manager is responsible for handling requests.
 * @author emma
 *
 */
public class PurchaseManager extends BillingListener
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

	public void restoreService()
	{
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
		if ( !mService.isConnected() )
		{
			onPurchaseStateChanged(
					MA_PURCHASE_STATE_FAILED,
					handle,
					MA_PURCHASE_ERROR_CONNECTION_FAILED);
			return;
		}

		if ( !mService.isPublicKeySet() )
		{
			onPurchaseStateChanged(
					MA_PURCHASE_STATE_FAILED,
					handle,
					MA_PURCHASE_ERROR_PUBLIC_KEY_NOT_SET);
			return;
		}

		PurchaseInformation purchase = m_PurchaseTable.get(handle);
		if ( null != purchase )
		{
			// Remove pending receipt event if there is one.
			m_PendingEvents.remove(handle);

			// Send the request to the service.
			// If there is another request in progress, this one will be placed
			// on the queue.
			BaseRequest requestObj = mService.requestPurchase(purchase.getProductID(), handle);
			if ( requestObj.isPending() )
			{
				SYSLOG("@@MoSync Request was placed on a queue");
				return;
			}

			// If request was sent to the server send IN_PROGRESS event, but
			// keep in mind that RESPONSE_CODE intents can later signal errors on the server side.
			if ( requestObj != null &&
					requestObj.getRequestID() != Consts.BILLING_RESPONSE_INVALID_REQUEST_ID )
			{
				SYSLOG("@@MoSync maPurchaseRequest: requestID = " +  requestObj.getRequestID() );
				mCurrentPurchaseHandle = handle;

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
			SYSLOG("@@MoSync maPurchaseRequest: requestID = " +  requestObj.getRequestID() );
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

	/**
	 * Get the product ID for the given handle.
	 * @param handle The product handle.
	 * @return The product ID.
	 */
	public String getProductID(int handle)
	{
		PurchaseInformation purchase = m_PurchaseTable.get(handle);
		if ( null != purchase )
		{
			return purchase.getProductID();
		}

		return "";
	}

	/**
	 * Get the receipt field for a given purchase.
	 * @param handle The product handle.
	 * @param field The receipt field.
	 * @return The receipt field value.
	 */
	public String getField(int handle, String field)
	{
		PurchaseInformation purchase = m_PurchaseTable.get(handle);
		if ( null != purchase )
		{
			if ( purchase.getState() != MA_PURCHASE_STATE_COMPLETED &&
					purchase.getState() != MA_PURCHASE_STATE_PRODUCT_REFUNDED )
			{
				return Consts.RECEIPT_NOT_AVAILABLE;
			}
			if ( field.equals(MA_PURCHASE_RECEIPT_PRODUCT_ID) )
			{
				return purchase.getProductID();
			}
			if ( field.equals(MA_PURCHASE_RECEIPT_PURCHASE_DATE) )
			{
				return String.valueOf(purchase.mTime);
			}
			if ( field.equals(MA_PURCHASE_RECEIPT_APP_ITEM_ID) )
			{
				return purchase.getPackageName();
			}
			if ( field.equals(MA_PURCHASE_RECEIPT_TRANSACTION_ID) )
			{
				return purchase.getOrderId();
			}
			return Consts.RECEIPT_FIELD_NOT_AVAILABLE;
		}
		return Consts.RECEIPT_INVALID_HANDLE;
	}

	/**
	 * Send a restoreTransaction request to Google Play.
	 * After the request is sent to Google Play (i.e.
	 * request.getRequestID() != Consts.BILLING_RESPONSE_INVALID_REQUEST_ID )
	 * the app waits for PURCHASE_STATE_CHANGE messages.
	 */
	public void restoreTransactions()
	{
		if ( !mService.isConnected() )
		{
			mMoSyncThread.postEvent(BillingEvent.onRestoreTransaction(
					MA_PURCHASE_STATE_RESTORE_ERROR,
					-1,
					MA_PURCHASE_ERROR_CONNECTION_FAILED));
			return;
		}

		if ( !mService.isPublicKeySet() )
		{
			mMoSyncThread.postEvent(BillingEvent.onRestoreTransaction(
					MA_PURCHASE_STATE_RESTORE_ERROR,
					-1,
					MA_PURCHASE_ERROR_PUBLIC_KEY_NOT_SET));
			return;
		}

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
			if ( purchase.getState() == MA_PURCHASE_STATE_COMPLETED ||
					purchase.getState() == MA_PURCHASE_STATE_PRODUCT_REFUNDED )
			{
				int[] event = m_PendingEvents.get(handle);
				if ( null != event )
				{
					mMoSyncThread.postEvent(
							BillingEvent.onVerifyReceipt(
												event[3],event[2],event[4]));
				}
				// Remove pending receipt event if there is one.
				m_PendingEvents.remove(handle);
			}
			else
			{
				Log.e("@@MoSync","maPurchaseVerifyReceipt product was not purchased " + handle);
				mMoSyncThread.postEvent(BillingEvent.onVerifyReceipt(
						handle,
						MA_PURCHASE_STATE_RECEIPT_ERROR,
						MA_PURCHASE_ERROR_NO_RECEIPT));
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
	public int setKey(final String developerPublicKey)
	{
		return mService.setPublicKey(developerPublicKey);
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

	/**
	 * Manager is notified through this method when a purchase was completed.
	 * Set all the purchase information to the current purchase object.
	 * At this point transaction details(the receipt) are available to the
	 * user, after he calls verifyReceipt.
	 * @param purchase The purchase.
	 */
	public static void onTransactionInformationReceived(PurchaseInformation purchase)
	{
		// Keep MA_PURCHASE_EVENT_RECEIPT_VALID event for this purchase.
		// The event will be sent after verifyreceipt is called.
		onReceiptEvent(BillingEvent.onVerifyReceipt(
				mCurrentPurchaseHandle, MA_PURCHASE_STATE_RECEIPT_VALID, 0), mCurrentPurchaseHandle);

		setCurrentPurchaseInformation(purchase);

		// Notify app that request is now completed.
		onPurchaseStateChanged(MA_PURCHASE_STATE_COMPLETED, mCurrentPurchaseHandle, 0);
	}

	/**
	 * Set all the purchase information to the current purchase object.
	 * @param purchase The purchase object that was created while parsing the
	 * received JSON.
	 */
	public static void setCurrentPurchaseInformation(PurchaseInformation purchase)
	{
		PurchaseInformation currentPurchase = m_PurchaseTable.get(mCurrentPurchaseHandle);
		currentPurchase.setNotificationID(purchase.getNotificationId());
		currentPurchase.setState(purchase.getState());
		currentPurchase.setOrderId(purchase.getOrderId());
		currentPurchase.mTime = purchase.mTime;
		currentPurchase.setPackageName(purchase.getPackageName());
	}

	/**
	 * Manager is notified through this method when a purchase is restored
	 * @param purchase The new purchase object.
	 */
	public static void onPurchaseRestored(PurchaseInformation purchase)
	{
		onReceivedPurchase(purchase, MA_PURCHASE_STATE_PRODUCT_RESTORED);
	}

	/**
	 * Manager is notified through this method when a purchase is refunded.
	 * Refunds cannot be initiated by the user( only by the merchant). There is
	 * an exception, by testing refund process using android.test.refunded
	 * reserved productId.
	 *
	 * @param purchase The new purchase object.
	 */
	public static void onPurchaseRefunded(PurchaseInformation purchase)
	{
		onReceivedPurchase(purchase, MA_PURCHASE_STATE_PRODUCT_REFUNDED);
	}

	/**
	 * On incoming purchase: store it's details, keep the verifyReceipt
	 * event and post refunded or restored event.
	 *
	 * @param purchase The new purchase object.
	 * @param purchaseState One of the constants:
	 * 	- MA_PURCHASE_STATE_PRODUCT_REFUNDED,
	 * 	- MA_PURCHASE_STATE_PRODUCT_RESTORED
	 */
	public static void onReceivedPurchase(PurchaseInformation purchase, int purchaseState)
	{
		// Create new product handle.
		int handle = mMoSyncThread.nativeCreatePlaceholder();
		m_PurchaseTable.add(handle, purchase);
		m_PurchaseTable.get(handle).setHandle(handle);

		// Keep MA_PURCHASE_EVENT_RECEIPT_VALID event for this purchase.
		// The event will be sent after verifyReceipt is called.
		onReceiptEvent(BillingEvent.onVerifyReceipt(
				handle, MA_PURCHASE_STATE_RECEIPT_VALID, 0), handle);

		// Post product refunded or restored event to MoSync queue.
		if ( purchaseState == MA_PURCHASE_STATE_PRODUCT_REFUNDED )
		{
			mMoSyncThread.postEvent(BillingEvent.onProductRefund(
					MA_PURCHASE_STATE_PRODUCT_REFUNDED, handle, 0));
		}
		else
		{
			mMoSyncThread.postEvent(BillingEvent.onRestoreTransaction(
					MA_PURCHASE_STATE_PRODUCT_RESTORED, handle, 0));
		}
	}

	/**
	 * Manager is notified through this method when a purchase changes state.
	 * @param state The new state.
	 * @param handle The purchase handle.
	 * @param error ErrorCode if necessary.
	 */
	public static void onPurchaseStateChanged(int state, int handle, int error)
	{
		BillingService.setPurchaseRequestInProgress(false);
		mMoSyncThread.postEvent(BillingEvent.onPurchaseStateChanged(handle, state, error));
	}

	/**
	 * Manager is notified through this method for receipt events.
	 * Receipt events will be kept in memory until an explicit verifyReceipt
	 * call.
	 * @param event The receipt event.
	 * @param handle Teh purchase handle.
	 */
    public static void onReceiptEvent(int[] event, int handle)
    {
		m_PendingEvents.add(handle,event);
    }
	/************************ Class members ************************/

	/**
	 * The MoSync thread object.
	 */
	private static MoSyncThread mMoSyncThread;
	private BillingService mService;

	/**
	 * -1 means no active purchase.
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
	private static HandleTable<int[]> m_PendingEvents = new HandleTable<int[]>();
}