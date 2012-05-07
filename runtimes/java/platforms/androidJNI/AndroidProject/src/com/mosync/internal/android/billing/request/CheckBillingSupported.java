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

/**
 * Wrapper class that checks if in-app billing is supported.
 * This request verifies that the Google Play application supports in-app billing.
 * Usually this request is sent when an application first starts up. This request
 * is useful if you want to enable or disable certain UI features that are relevant
 * only to in-app billing.
 * @author emma
 */
public class CheckBillingSupported extends BaseRequest
{
    public CheckBillingSupported(IMarketBillingService service)
    {
        // This object is never created as a side effect of starting this
        // service so we pass -1 as the startId to indicate that we should
        // not stop this service after executing this request.
        super(-1, service);
    }

    /**
     * No need to send an event, as the method is synchronous.
     */
    @Override
    public void run() throws RemoteException
    {
        Bundle response = mService.sendBillingRequest(
				BillingService.createRequestBundle(Consts.METHOD_CHECK_BILLING_SUPOPRTED));

        // Get only the response code, requestId is not neccessary since it's synchronous.
        int code = response.getInt(Consts.BILLING_RESPONSE_RESPONSE_CODE);
        mResponseCode = Consts.responseCodeValue(code);
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
        Log.e("@@MoSync", "CheckBillingSupported remote billing service crashed");
        super.onRemoteException(e);
    }
}
