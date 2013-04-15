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

import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.content.Intent;
import android.util.Log;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.nativeui.util.HandleTable;
import com.mosync.internal.android.billing.util.*;

import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_INVALID_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_NO_RECEIPT;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RES_MALFORMED_PUBLIC_KEY;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_FAILED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_IN_PROGRESS;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_DUPLICATE_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_PRODUCT_VALID;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RECEIPT_PRODUCT_ID;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RECEIPT_PURCHASE_DATE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RECEIPT_TRANSACTION_ID;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RECEIPT_TITLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RECEIPT_DESCRIPTION;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RECEIPT_PRICE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_COMPLETED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_RECEIPT_ERROR;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_RECEIPT_VALID;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RECEIPT_APP_ITEM_ID;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_PUBLIC_KEY_NOT_SET;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_CONNECTION_FAILED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_PRODUCT_RESTORED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_PRODUCT_REFUNDED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_RESTORE_ERROR;
import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;

/**
 * The purchase manager holds all purchase info of both user
 * made requests, but also refunds.
 * The manager is responsible for handling requests.
 * Note: since 26th February 2013, migrated to use In-AppBiling version 3.
 * @author emma tresanszki
 *
 */
public class PurchaseManager extends BillingListener
{
	/************************ Class members ************************/

    /**
     * The helper object from InAppPurchase.
     */
    private static IabHelper mHelper;

	/**
	 * The MoSync thread object.
	 */
	private static MoSyncThread mMoSyncThread;

	/**
	 * -1 means no active purchase.
	 */
	private static int mCurrentPurchaseHandle = -1;

	/**
	 * A table that contains a mapping between a handle and a purchase object.
	 */
	private static HandleTable<Purchase> m_PurchaseTable =
		new HandleTable<Purchase>();

	private static String key;
	private boolean mRestoringTransactions = false;

	/**
	 * Constructor.
	 */
	public PurchaseManager(MoSyncThread thread)
	{
		mMoSyncThread = thread;
	}

	/**
	 * Bind the service if the IAP permission is set.
	 */
	public void bindService()
	{
		SYSLOG("@@MoSync Purchase: bindService ");
		// IAP permission is set, bind the service.
		mHelper = new IabHelper(mMoSyncThread.getActivity());

		if (key != null)
			mHelper.setSignature(key);

        // Start setup. This is asynchronous and the specified listener
        // will be called once setup completes.
		mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
            public void onIabSetupFinished(IabResult result) {
                if (!result.isSuccess()) {
                    return ; //cannot connect.
                }
            }
        });
	}

	public void restoreService()
	{
		if (mHelper == null)
			bindService();
	}

	public int checkPurchaseSupported()
	{
		if (mHelper == null)
		{
			// Something went wrong with the service connection.
			return MA_PURCHASE_ERROR_CONNECTION_FAILED;
		}
		return Consts.responseCodeValue( mHelper.getBillingSupportedResponse() );
	}

	/**
	 * Create an internal purchase object that will hold all the info needed.
	 * @param productHandle The internal product handle.
	 * @param productID The product id, known as the SKU.
	 * @return MA_PURCHASE_STATE_PRODUCT_VALID or an error code.
	 */
	public int createPurchase(final int productHandle, final String productID)
	{
		Purchase newPurchase = new Purchase(productID);
		if ( m_PurchaseTable.add(productHandle, newPurchase) )
		{
			SYSLOG("@@MoSync maPurchaseCreate " + productHandle + " for id = " + productID);
			newPurchase.setHandle( productHandle );

			SYSLOG("MoSync maPurchaseCreate: Valid product " + productID);
			return MA_PURCHASE_STATE_PRODUCT_VALID;
		}

		SYSLOG("@@MoSync maPurchaseCreate: Duplicate handle " + productHandle);
		return MA_PURCHASE_STATE_DUPLICATE_HANDLE;
	}

    // Listener that's called when we finish querying the items we own.
    IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
        public void onQueryInventoryFinished(IabResult result, Inventory inventory) {
            Log.d("@@MoSync", "Billing Query inventory finished.");
            if (result.isFailure())
            {
                SYSLOG("@@MoSynv maPurchaseRestoreTransactions failed: " + result);
                mRestoringTransactions = false;
                return;
            }

            SYSLOG("@@MoSync maPurchaseRestoreTransactions: Query inventory was successful.");

            List<Purchase> ownedItems = inventory.getAllPurchases();
            for (int i=0; i<ownedItems.size(); i++)
            {
				mHelper.consumeAsync(ownedItems.get(i), mConsumeFinishedListener);
            }
            mRestoringTransactions = false;
        }
    };

    // Called when consumption is complete.
    IabHelper.OnConsumeFinishedListener mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener() {
        public void onConsumeFinished(Purchase purchase, IabResult result) {
            SYSLOG("@@MoSync Consumption finished. Purchase: " + purchase + ", result: " + result);

            if (result.isSuccess())
            {
                // Successfully consumed
                SYSLOG("@@MoSync Consumption successful. Provisioning." + purchase.getSku());
                if (mRestoringTransactions)
                {
					SYSLOG("@@MoSync maPurchaseRestoreTransactions received purchase: " + purchase.getSku());
					onPurchaseRestored(purchase);
					return;
                }
            }
        }
    };

    // Callback for when a purchase is finished.
    IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
        public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
            SYSLOG("@@MoSync Purchase finished: result = " + result +
					", purchase: " + purchase + " .For handle = " + result.getPurchaseHandle());
            if (result.isFailure())
            {
				m_PurchaseTable.get(mCurrentPurchaseHandle).setState(MA_PURCHASE_STATE_FAILED);
				onPurchaseStateChanged(
						MA_PURCHASE_STATE_FAILED,
						result.getPurchaseHandle(),
						Consts.responseCodeValue( result.getResponse()) );
                return;
            }

            SYSLOG("@@MoSync Purchase successful.");
			onTransactionInformationReceived(purchase);

			// Consume all managed in-app products.
            mHelper.consumeAsync(purchase, mConsumeFinishedListener);
        }
    };

	/**
	 * Start a purchase request and set it to the current handle.
	 * @param handle
	 */
	public void requestPurchase(int handle)
	{
		SYSLOG("@@MoSync maPurchseRequest");

		if (!mHelper.isPublicKeySet())
		{
			onPurchaseStateChanged(
					MA_PURCHASE_STATE_FAILED,
					handle,
					MA_PURCHASE_ERROR_PUBLIC_KEY_NOT_SET);
			return;
		}

		Purchase purchase = m_PurchaseTable.get(handle);
		if ( null != purchase )
		{
			// Send the request to the service.
			mHelper.launchPurchaseFlow(
					mMoSyncThread.getActivity(), purchase.getSku(), handle, mPurchaseFinishedListener);

			mCurrentPurchaseHandle = handle;
			purchase.setState(MA_PURCHASE_STATE_IN_PROGRESS);
		}
		else
		{
			SYSLOG("@@MoSync maPurchaseRequest: Invalid product handle " + handle);
			Log.e("MoSync","@@MoSync maPurchaseRequest: Invalid product handle " + handle);
			mMoSyncThread.postEvent(BillingEvent.onPurchaseStateChanged(
					handle,
					MA_PURCHASE_STATE_FAILED,
					MA_PURCHASE_ERROR_INVALID_HANDLE));
		}
	}

	/**
	 * Activity receives control again after the checkout UI finished the transaction.
	 * @param requestCode Unique request code set when purchasing the item.
	 * @param resultCode The result code of the request.
	 * @param data
	 * @return
	 */
	public static boolean handleActivityResult(final int requestCode, final int resultCode, final Intent data)
	{
		if (mHelper != null)
		{
			return mHelper.handleActivityResult(requestCode, resultCode, data);
		}
		else
		{
			SYSLOG("@@MoSync Purchase: handleActivityResult setup again the helper");
			mHelper = new IabHelper(mMoSyncThread.getActivity(), key);
			mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
	            public void onIabSetupFinished(IabResult result) {
	                if (!result.isSuccess()) {
	                    return ; //cannot connect.
	                }
	                mHelper.handleActivityResult(requestCode, resultCode, data);
	            }
	        });

		}
		return true;
	}

	/**
	 * Get the product ID for the given handle.
	 * @param handle The product handle.
	 * @return The product ID.
	 */
	public String getProductID(int handle)
	{
		Purchase purchase = m_PurchaseTable.get(handle);
		if ( null != purchase )
		{
			return purchase.getSku();
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
		Purchase purchase = m_PurchaseTable.get(handle);
		if ( null != purchase )
		{
			if ( purchase.getState() != MA_PURCHASE_STATE_COMPLETED &&
					purchase.getState() != MA_PURCHASE_STATE_PRODUCT_REFUNDED )
			{
				return Consts.RECEIPT_NOT_AVAILABLE;
			}
			if ( field.equals(MA_PURCHASE_RECEIPT_PRODUCT_ID) )
			{
				return purchase.getSku();
			}
			if ( field.equals(MA_PURCHASE_RECEIPT_PURCHASE_DATE) )
			{
				return String.valueOf(purchase.getPurchaseTime());
			}
			if ( field.equals(MA_PURCHASE_RECEIPT_APP_ITEM_ID) )
			{
				return purchase.getPackageName();
			}
			if ( field.equals(MA_PURCHASE_RECEIPT_TRANSACTION_ID) )
			{
				return purchase.getOrderId();
			}
			if ( field.equals(MA_PURCHASE_RECEIPT_PRICE) )
			{
				String price = getDouble(purchase.getPrice());
				return price.replace(",",".");
			}
			if ( field.equals(MA_PURCHASE_RECEIPT_TITLE) )
			{
				return purchase.getTitle();
			}
			if ( field.equals(MA_PURCHASE_RECEIPT_DESCRIPTION) )
			{
				return purchase.getDescription();
			}
			return Consts.RECEIPT_FIELD_NOT_AVAILABLE;
		}
		return Consts.RECEIPT_INVALID_HANDLE;
	}

	private String getDouble(String doubleAsString)
	{
		StringBuffer sBuffer = new StringBuffer();
		Pattern p = Pattern.compile("[0-9]+,[0-9]+");
		Matcher m = p.matcher(doubleAsString);
		while (m.find())
		{
			sBuffer.append(m.group());
		}
		return sBuffer.toString();
	}

	/**
	 * Send a restoreTransaction request to Google Play.
	 * After the request is sent to Google Play (i.e.
	 * request.getRequestID() != Consts.BILLING_RESPONSE_INVALID_REQUEST_ID )
	 * the app waits for PURCHASE_STATE_CHANGE messages.
	 */
	public void restoreTransactions()
	{
		SYSLOG("@@MoSync maPurchaseRestoreTransactions");
		if ( key.isEmpty() )
		{
			mMoSyncThread.postEvent(BillingEvent.onRestoreTransaction(
					MA_PURCHASE_STATE_RESTORE_ERROR,
					-1,
					MA_PURCHASE_ERROR_PUBLIC_KEY_NOT_SET));
			return;
		}

		mRestoringTransactions = true;
		mHelper.queryInventoryAsync(mGotInventoryListener);
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
		Purchase purchase = m_PurchaseTable.get(handle);
		if ( null != purchase )
		{
			// Check if a purchase was requested for the product.
			// If a product was not purchased, it means that it has no pending events.
			if ( purchase.getState() == MA_PURCHASE_STATE_COMPLETED ||
					purchase.getState() == MA_PURCHASE_STATE_PRODUCT_REFUNDED ||
					purchase.getState() == MA_PURCHASE_STATE_PRODUCT_RESTORED )
			{
				SkuDetails details = null;
				try {
					details = mHelper.getSkuDetails(purchase.getSku());
				} catch (IabException e) {
					mMoSyncThread.postEvent(
							BillingEvent.onVerifyReceipt(
												handle,
												MA_PURCHASE_STATE_RECEIPT_ERROR,
												MA_PURCHASE_ERROR_NO_RECEIPT));
					return;
				}
				if (details != null)
				{
					purchase.setPrice(details.getPrice());
					purchase.setTitle(details.getTitle());
					purchase.setDescription(details.getDescription());

					mMoSyncThread.postEvent(
							BillingEvent.onVerifyReceipt(
												handle,
												MA_PURCHASE_STATE_RECEIPT_VALID,
												0));
					return;
				}
				else
				{
					SYSLOG("@@MoSync maPurchaseVerifyReceipt: the item does not have a receipt " + handle);
				}
			}
			else
			{
				SYSLOG("@@MoSync maPurchaseVerifyReceipt product was not purchased " + handle);
			}
			mMoSyncThread.postEvent(BillingEvent.onVerifyReceipt(handle,
					MA_PURCHASE_STATE_RECEIPT_ERROR,
					MA_PURCHASE_ERROR_NO_RECEIPT));
		}
		else
		{
			SYSLOG("@@MoSync maPurchaseVerifyReceipt: Invalid product handle " + handle);
			mMoSyncThread.postEvent(BillingEvent.onVerifyReceipt(
					handle,
					MA_PURCHASE_STATE_RECEIPT_ERROR,
					MA_PURCHASE_ERROR_INVALID_HANDLE));
		}
	}

	public static int getCurrentRequestCode()
	{
		if (mHelper != null)
			return mHelper.getRequestCode();
		return -1;
	}

	/**
	 * Generate a PublicKey instance from a string containing the
	 * Base64-encoded public key.
	 */
	public int setKey(final String developerPublicKey)
	{
		try
		{
			Security.generatePublicKey(developerPublicKey);
		} catch (Exception ex)
		{
			SYSLOG("maPurchaseSetPublicKey: malformed developerPublicKey");
			return MA_PURCHASE_RES_MALFORMED_PUBLIC_KEY;
		}

		if (mHelper == null)
		{
			mHelper = new IabHelper(mMoSyncThread.getActivity(), developerPublicKey);
		}
		else
		{
			mHelper.setSignature(developerPublicKey);
		}

		key = developerPublicKey;
		return MA_PURCHASE_RES_OK;
	}

	public int destroyPurchase(int handle)
	{
		Purchase purchase = m_PurchaseTable.get(handle);
		if ( null != purchase )
		{
			// Remove the internal purchase object.
			m_PurchaseTable.remove(handle);
			// Remove pending receipt event if there is one.
			//m_PendingEvents.remove(handle);

			return MA_PURCHASE_RES_OK;
		}

		SYSLOG("@@MoSync maPurchaseDestroy: Invalid product handle " + handle);
		return MA_PURCHASE_ERROR_INVALID_HANDLE;
	}

	public void unbindService()
	{
		if (mHelper != null)
			mHelper.dispose();
		mHelper = null;
	}

	/**
	 * Manager is notified through this method when a purchase was completed.
	 * Set all the purchase information to the current purchase object.
	 * At this point transaction details(the receipt) are available to the
	 * user, after he calls verifyReceipt.
	 * @param purchase The purchase.
	 */
	public static void onTransactionInformationReceived(Purchase purchase)
	{
		purchase.setState(MA_PURCHASE_STATE_COMPLETED);
		setCurrentPurchaseInformation(purchase);

		// Notify app that request is now completed.
		onPurchaseStateChanged(
				MA_PURCHASE_STATE_COMPLETED, mCurrentPurchaseHandle, 0);
	}

	/**
	 * Set all the purchase information to the current purchase object.
	 * @param purchase The purchase object that was created while parsing the
	 * received JSON.
	 */
	public static void setCurrentPurchaseInformation(Purchase purchase)
	{
		Purchase currentPurchase = m_PurchaseTable.get(mCurrentPurchaseHandle);
		currentPurchase.setState(purchase.getState());
		currentPurchase.setOrderID(purchase.getOrderId());
		currentPurchase.setOrderTime(purchase.getPurchaseTime());
		currentPurchase.setPackageName(purchase.getPackageName());
	}

	/**
	 * Manager is notified through this method when a purchase is restored
	 * @param purchase The new purchase object.
	 */
	public static void onPurchaseRestored(Purchase purchase)
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
	public static void onPurchaseRefunded(Purchase purchase)
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
	public static void onReceivedPurchase(Purchase purchase, int purchaseState)
	{
		// Create new product handle.
		int handle = mMoSyncThread.nativeCreatePlaceholder();
		m_PurchaseTable.add(handle, purchase);
		m_PurchaseTable.get(handle).setHandle(handle);

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
			// Store the receipt for the current purchase.
			purchase.setState(MA_PURCHASE_STATE_PRODUCT_RESTORED);
			setCurrentPurchaseInformation(purchase);
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
		mMoSyncThread.postEvent(BillingEvent.onPurchaseStateChanged(handle, state, error));
	}
}