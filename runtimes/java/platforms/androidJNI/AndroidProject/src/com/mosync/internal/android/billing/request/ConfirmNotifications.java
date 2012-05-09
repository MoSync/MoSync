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

import com.mosync.internal.android.billing.Consts;
import com.android.vending.billing.IMarketBillingService;
import com.mosync.internal.android.billing.BillingService;

/**
 * This request acknowledges that your application received the details of a
 * purchase state change. That is, this message confirms that you sent a
 * GET_PURCHASE_INFORMATION request for a given notification and that you
 * received the purchase information for the notification.
 * If you don't send a confirmation message, Google Play will continue sending
 * IN_APP_NOTIFY messages for the transactions you have not confirmed.
 * As a best practice, you should not send a CONFIRM_NOTIFICATIONS request for a
 * purchased item until you have delivered the item to the user. This way, if
 * your application crashes or something else prevents your application from
 * delivering the product, your application will still receive an IN_APP_NOTIFY
 * broadcast intent from Google Play indicating that you need to deliver the product.
 * Also, as a best practice, your application must be able to handle IN_APP_NOTIFY
 * messages that contain multiple orders.
 *
 * @author emma
 *
 */
public class ConfirmNotifications extends BaseRequest
{

	public ConfirmNotifications(int startId,
			IMarketBillingService service, String[] notifyIds)
	{
		super(-1, service);
		mNotifyIds = notifyIds;
	}

    @Override
    public void run() throws RemoteException {
        Bundle request = BillingService.createRequestBundle(Consts.METHOD_CONFIRM_NOTIFICATIONS);
        request.putStringArray(Consts.BILLING_REQUEST_NOTIFY_IDS, mNotifyIds);
        Bundle response = mService.sendBillingRequest(request);
        mRequestId = response.getLong(Consts.BILLING_RESPONSE_REQUEST_ID,
                Consts.BILLING_RESPONSE_INVALID_REQUEST_ID);
    }

    final String[] mNotifyIds;
}
