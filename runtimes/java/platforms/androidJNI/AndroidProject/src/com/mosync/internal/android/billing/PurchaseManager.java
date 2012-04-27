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

import java.security.KeyFactory;
import java.security.NoSuchAlgorithmException;
import java.security.PublicKey;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.X509EncodedKeySpec;

import android.util.Log;

import com.mosync.internal.android.EventQueue;
import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.android.billing.request.BaseRequest;
import com.mosync.nativeui.util.HandleTable;

import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_INVALID_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_FAILED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_IN_PROGRESS;

/**
 * The purchase manager holds all purchase info of both user
 * made requests, but also refunds.
 * The manager is responsible for handling requests.
 * @author emma
 *
 */
public class PurchaseManager
{

	/**
	 * Constructor.
	 */
	public PurchaseManager(MoSyncThread thread)
	{
		mMoSyncThread = thread;
		mService = new BillingService();
		mService.setContext(mMoSyncThread.getActivity());
		mService.setManager(this);
	}

	public int checkPurchaseSupported()
	{
		int result = mService.checkBillingSupported();
		Log.e("@@MoSync","Billing maPurchaseSupported result = " + result);
		return result;
//		return mService.checkBillingSupported();
	}

	public int createPurchase(final String productID)
	{
		PurchaseInformation newPurchase = new PurchaseInformation(productID);
		newPurchase.setHandle( m_PurchaseTable.add(newPurchase) );

		return newPurchase.getHandle();
	}

	/**
	 * Start a purchase request and set it to the current handle.
	 * @param handle
	 * @return
	 */
	public int requestPurchase(int handle)
	{
		PurchaseInformation purchase = m_PurchaseTable.get(handle);
		// and BillingService.isPurchaseRequestInProgress() return new err code.
		if ( null != purchase )
		{
			// If product is managed, cannot purchase it again.TODO err code.
			// Send the request to the service.
			BaseRequest requestObj = mService.requestPurchase(purchase.getProductID());
			//if ( requestObj != null )
			// or requestObj.getRequestID() > 0 0
			if ( requestObj.getRequestID() != Consts.BILLING_RESPONSE_INVALID_REQUEST_ID )
			{
				mCurrentPurchaseHandle = handle;

				// Send event! + message the request was sent successfully to Google Play.
				purchase.setState(MA_PURCHASE_STATE_IN_PROGRESS);

				purchase.setRequest(requestObj);
				return MA_PURCHASE_RES_OK;
			}
			else
			{
				purchase.setState(MA_PURCHASE_STATE_FAILED);
				// Post the event to the MoSync queue.
				EventQueue.getDefault().postPurchaseStateChanged(handle, MA_PURCHASE_STATE_FAILED);
				return -1; //todo Service not connected! = no internet connection
			}
		}

		Log.e("@@MoSync", "maPurchaseRequest: Invalid product handle " + handle);
		return MA_PURCHASE_ERROR_INVALID_HANDLE;
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

	/**
	 *
	 * @return
	 */
	public int getPurchaseInfo()
	{

		return -1;
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

	public void restoreTransactions()
	{
//		mService.
	}

	/**
	 * Generate a PublicKey instance from a string containing the
	 * Base64-encoded public key.
	 */
	public int setKey(final String developerPublicKey)
	{
		try
		{
			byte[] decodedKey = Base64.decode(developerPublicKey);
			KeyFactory keyFactory = KeyFactory.getInstance(Security.KEY_FACTORY_ALGORITHM);
			mPublicKey = keyFactory.generatePublic( new X509EncodedKeySpec(decodedKey) );
		} catch (NoSuchAlgorithmException e) {
//			return MA_PURCHASE_RES_DEVELOPER_ERROR;
			return -1;
		} catch (InvalidKeySpecException e) {
//			Log.e(TAG, "Invalid key specification.");
//			throw new IllegalArgumentException(e);
			return -1;
		} catch (Base64DecoderException e) {
//			Log.e(TAG, "Base64DecoderException.", e);
			return -1;
		}
//		BillingService.setPublicKey(mDeveloperKey);
		return MA_PURCHASE_RES_OK;
	}

	public int destroyPurchase(int handle)
	{
		PurchaseInformation purchase = m_PurchaseTable.get(handle);
		if ( null != purchase )
		{
			// Remove the internal purchase object.
			m_PurchaseTable.remove(handle);

			return MA_PURCHASE_RES_OK;
		}

		Log.e("@@MoSync", "maPurchaseDestroy: Invalid product handle " + handle);
		return MA_PURCHASE_ERROR_INVALID_HANDLE;
	}

	public void unbindService()
	{
		mService.unbind();
	}

	/************************ Class members ************************/

	/**
	 * The MoSync thread object.
	 */
	private MoSyncThread mMoSyncThread;

	/**
	 *
	 */
	private BillingService mService;

	/**
	 * -1 means no active purchase. todo add constant
	 */
	private static int mCurrentPurchaseHandle = -1;
	/**
	 * The developer public key.
	 */
	private PublicKey mPublicKey = null;

	/**
	 * A table that contains a mapping between a handle and a purchase object.
	 */
	private static HandleTable<PurchaseInformation> m_PurchaseTable = new HandleTable<PurchaseInformation>();
//	private static HandleTable<Request> m_PurchaseTable = new HandleTable<Request>();
}
