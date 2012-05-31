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

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;

/**
 * The broadcast receiver for all asynchronous messages received from Google Play.
 * @author emma
 */
public class BillingReceiver extends BroadcastReceiver
{
    /**
     * This is the entry point for all asynchronous messages sent from Google Play.
     * This method forwards the messages on to the {@link BillingService}, which
     * handles the communication back to Google Play.
     * The {@link BillingService} also reports state changes back to the application
     * by posting MoSync events.
     */
    @Override
    public void onReceive(Context context, Intent intent)
    {
        String action = intent.getAction();
        SYSLOG("MoSync BillingReceiver onReceive action: " + action);
        if (Consts.ACTION_STATE_CHANGED.equals(action))
        {
            String signedData = intent.getStringExtra(Consts.BILLING_RESPONSE_INAPP_SIGNED_DATA);
            String signature = intent.getStringExtra(Consts.BILLING_RESPONSE_INAPP_SIGNATURE);
            purchaseStateChanged(context, signedData, signature);
        }
        else if (Consts.ACTION_NOTIFY.equals(action))
        {
            String notifyId = intent.getStringExtra(Consts.BILLING_RESPONSE_NOTIFICATION_ID);
            notify(context, notifyId);
        }
        else if (Consts.ACTION_RESPONSE_CODE.equals(action))
        {
            long requestId = intent.getLongExtra(Consts.BILLING_RESPONSE_INAPP_REQUEST_ID, -1);
            int responseCodeIndex = intent.getIntExtra(Consts.BILLING_RESPONSE_INAPP_RESPONSE_CODE,
					Consts.BILLING_RESPONSE_INVALID_RESPONSE_CODE);
            checkResponseCode(context, requestId, responseCodeIndex);
        }
        else
        {
            SYSLOG("BillingReceiver onReceive unexpected action: " + action);
        }
    }

    /**
     * This response contains detailed information about one or more transactions.
     * The transaction information is contained in a JSON string. The JSON string
     * is signed and the signature is sent to your application along with the JSON
     * string (unencrypted).
     *
     * @param context the context
     * @param signedData the (unencrypted) JSON string that is signed by the server
     * with the developer's private key.
     * @param signature the signature for the signedData.
     */
    private void purchaseStateChanged(Context context, String signedData, String signature)
    {
        Intent intent = new Intent(Consts.ACTION_STATE_CHANGED);
        intent.setClass(context, BillingService.class);
        intent.putExtra(Consts.BILLING_RESPONSE_INAPP_SIGNED_DATA, signedData);
        intent.putExtra(Consts.BILLING_RESPONSE_INAPP_SIGNATURE, signature);
        context.startService(intent);
    }

    /**
     * This response indicates that a purchase has changed state, which means a purchase succeeded,
     * was canceled, or was refunded. This response contains one or more notification IDs.
     * Each notification ID corresponds to a specific server-side message, and each message
     * contains information about one or more transactions.
     * After your application receives an IN_APP_NOTIFY broadcast intent, you may send a
     * GET_PURCHASE_INFORMATION request with the notification IDs to retrieve transaction details.
     *
     * @param context the context
     * @param notifyId the notification ID
     */
    private void notify(Context context, String notifyId)
    {
		// Notify the service that the request has completed.
        Intent intent = new Intent(Consts.ACTION_GET_PURCHASE_INFORMATION);
        intent.setClass(context, BillingService.class);
        intent.putExtra(Consts.BILLING_RESPONSE_NOTIFICATION_ID, notifyId);
        context.startService(intent);
    }

    /**
     * This response contains a Google Play server response code, and is sent after you make an
     * in-app billing request. A server response code can indicate that a billing request was
     * successfully sent to Google Play or it can indicate that some error occurred during a
     * billing request.
     * This response is not used to report any purchase state changes (such as refund or purchase
     * information).
     *
     * @param context the context
     * @param requestId the request ID that corresponds to a previous request.
     * @param responseCodeIndex the ResponseCode ordinal value for the request.
     */
    private void checkResponseCode(Context context, long requestId, int responseCodeIndex)
    {
        Intent intent = new Intent(Consts.ACTION_RESPONSE_CODE);
        intent.setClass(context, BillingService.class);
        intent.putExtra(Consts.BILLING_RESPONSE_INAPP_REQUEST_ID, requestId);
        intent.putExtra(Consts.BILLING_RESPONSE_INAPP_RESPONSE_CODE, responseCodeIndex);
        context.startService(intent);
    }
}