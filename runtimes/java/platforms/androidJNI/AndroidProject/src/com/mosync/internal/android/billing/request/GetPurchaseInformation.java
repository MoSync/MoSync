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

package com.mosync.internal.android.billing.request;

import android.os.Bundle;
import android.os.RemoteException;
import android.util.Log;

import com.android.vending.billing.IMarketBillingService;
import com.mosync.internal.android.billing.BillingService;
import com.mosync.internal.android.billing.Consts;
import com.mosync.internal.android.billing.Security;

/**
 * This request retrieves the details of a purchase state change.
 * A purchase state change can occur when a purchase request is billed
 * successfully or when a user cancels a transaction during checkout.
 * It can also occur when a previous purchase is refunded.
 * Google Play notifies your application when a purchase changes state,
 * so you only need to send this request when there is transaction information to retrieve.
 *
 * @author emma
 */
public class GetPurchaseInformation extends BaseRequest
{
	public GetPurchaseInformation
		(int startID, IMarketBillingService service, String[] notifyIds)
	{
	    // This object is never created as a side effect of starting this
	    // service so we pass -1 as the startId to indicate that we should
	    // not stop this service after executing this request.
	    super(startID, service);
	    mNotifyIds = notifyIds;
	}

	@Override
	public void run() throws RemoteException
	{
		/**
		 * The REQUEST_NONCE key contains a cryptographically secure nonce
		 * (number used once) that you must generate.
		 * Google Play returns this nonce with the PURCHASE_STATE_CHANGED broadcast
		 * intent so you can verify the integrity of the transaction information.
		 */
		mNonce = Security.generateNonce();

	    Bundle request = BillingService.createRequestBundle(Consts.METHOD_GET_PURCHASE_INFO);
	    request.putLong(Consts.BILLING_REQUEST_NONCE, mNonce);
	    request.putStringArray(Consts.BILLING_REQUEST_NOTIFY_IDS, mNotifyIds);
	    Bundle response = mService.sendBillingRequest(request);

	    mRequestId = response.getLong(Consts.BILLING_RESPONSE_REQUEST_ID,
				Consts.BILLING_RESPONSE_INVALID_REQUEST_ID);

	    int responseCode = response.getInt(Consts.BILLING_RESPONSE_INAPP_RESPONSE_CODE,
				Consts.BILLING_RESPONSE_INVALID_RESPONSE_CODE);
	    mResponseCode = Consts.responseCodeValue(responseCode);

	    //Todo delete log.
	    Log.e("@@MoSync", "GetPurchaseInformation response code: " + mResponseCode );
	    Log.e("@@MoSync", "GetPurchaseInformation requestID: " + mRequestId );
	}

	/**
	 * Called when a remote exception occurs while trying to execute the
	 * {@link #run()} method.  The derived class can override this to
	 * execute exception-handling code.
	 * @param e the exception
	 */
	@Override
	protected void onRemoteException(RemoteException e)
	{
	    Log.e("@@MoSync", "Remote billing service crashed");
	    super.onRemoteException(e);
	    Security.removeNonce(mNonce);
	}

	/************************ Class members ************************/
    private long mNonce;
    private final String[] mNotifyIds;
}