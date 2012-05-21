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

import com.android.vending.billing.IMarketBillingService;
import com.mosync.internal.android.billing.BillingService;
import com.mosync.internal.android.billing.Consts;
import com.mosync.internal.android.billing.Security;

/**
 * Wrapper class that sends a RESTORE_TRANSACTIONS message to the server.
 *
 * @author emma
 */
public class RestoreTransactions extends BaseRequest
{
	long mNonce;

	public RestoreTransactions(IMarketBillingService service)
	{
		// This object is never created as a side effect of starting this
		// service so we pass -1 as the startId to indicate that we should
		// not stop this service after executing this request.
		super(-1, service);
	}

	@Override
	public void run() throws RemoteException
	{
		mNonce = Security.generateNonce();

		Bundle request = BillingService.createRequestBundle(Consts.METHOD_RESTORE_TRANSACTIONS);
		request.putLong(Consts.BILLING_REQUEST_NONCE, mNonce);
		Bundle response = mService.sendBillingRequest(request);

		mRequestId = response.getLong(Consts.BILLING_RESPONSE_REQUEST_ID,
				Consts.BILLING_RESPONSE_INVALID_REQUEST_ID);
	}

	@Override
	protected void onRemoteException(RemoteException e)
	{
		super.onRemoteException(e);
		Security.removeNonce(mNonce);
	}
}