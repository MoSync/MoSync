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

import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RES_UNAVAILABLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_UNKNOWN;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_INVALID_PRODUCT;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_CONNECTION_FAILED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_ERROR_CANCELLED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_COMPLETED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_FAILED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_PRODUCT_REFUNDED;

/**
 * Utility class that holds constants required in billing requests.
 * Values are received from Google Play,so do NOT attempt to change them.
 * @author emma
 */
public class Consts
{

    /** This is the action we use to bind to the MarketBillingService. */
    public static final String MARKET_BILLING_SERVICE_ACTION =
        "com.android.vending.billing.MarketBillingService.BIND";

    /**
     * The possible states of an in-app purchase, as defined by Android Market.
     * The state change events are sent for requestPurchase or restoreTransactions
     * request types.
     */
    // User was charged for the order.
    private static final int PURCHASE_STATE_PURCHASED = 0;
    // The charge failed on the server.
    private static final int PURCHASE_STATE_CANCELED = 1;
    // User received a refund for the order.
    static final int PURCHASE_STATE_REFUNDED = 2;

	// Match a PurchaseState to a MoSync constant.
	public static int purchaseStateValue(int purchaseState)
	{
		switch (purchaseState)
		{
		case PURCHASE_STATE_PURCHASED:
			return MA_PURCHASE_STATE_COMPLETED;
		case PURCHASE_STATE_CANCELED:
			return MA_PURCHASE_STATE_FAILED;
		case PURCHASE_STATE_REFUNDED:
			return MA_PURCHASE_STATE_PRODUCT_REFUNDED;
		default:
			return MA_PURCHASE_STATE_FAILED;
		}
	}

    // The response codes for a request, defined by Android Market.
    public static final int RESULT_OK = 0;
    public static final int RESULT_USER_CANCELED = 1;
    public static final int RESULT_SERVICE_UNAVAILABLE = 2;
    public static final int RESULT_BILLING_UNAVAILABLE = 3;
    public static final int RESULT_ITEM_UNAVAILABLE = 4;
    public static final int RESULT_DEVELOPER_ERROR = 5;
    public static final int RESULT_ERROR = 6;

	// Match a response code to a MoSync constant.
	public static int responseCodeValue(int responseCode)
	{
		switch (responseCode)
		{
		case RESULT_OK:
			return MA_PURCHASE_RES_OK;
		case RESULT_USER_CANCELED:
			return MA_PURCHASE_ERROR_CANCELLED;
		case RESULT_SERVICE_UNAVAILABLE:
			return MA_PURCHASE_ERROR_CONNECTION_FAILED;
		case RESULT_BILLING_UNAVAILABLE:
			return MA_PURCHASE_RES_UNAVAILABLE;
		case RESULT_ITEM_UNAVAILABLE:
			return MA_PURCHASE_ERROR_INVALID_PRODUCT;
		case RESULT_DEVELOPER_ERROR:
		case RESULT_ERROR:
		default:
			return MA_PURCHASE_ERROR_UNKNOWN;
		}
	}

	public final static int PURCHASE_STATE_ON_HOLD = -1;
	public final static String RECEIPT_NOT_AVAILABLE = "Receipt not available";
	public final static String RECEIPT_FIELD_NOT_AVAILABLE = "Receipt Field not available";
	public final static String RECEIPT_INVALID_HANDLE = "Invalid handle";

	/**
	 * The request methods.
	 */
	public static final String METHOD_CHECK_BILLING_SUPOPRTED = "CHECK_BILLING_SUPPORTED";
	public static final String METHOD_CONFIRM_NOTIFICATIONS = "CONFIRM_NOTIFICATIONS";
	public static final String METHOD_GET_PURCHASE_INFO = "GET_PURCHASE_INFORMATION";
	public static final String METHOD_REQUEST_PURCHASE = "REQUEST_PURCHASE";
	public static final String METHOD_RESTORE_TRANSACTIONS = "RESTORE_TRANSACTIONS";

	/**
	 * The names of the fields in the request bundle.
	 */
    public static final String BILLING_REQUEST_METHOD = "BILLING_REQUEST";
    public static final String BILLING_REQUEST_API_VERSION = "API_VERSION";
    public static final String BILLING_REQUEST_PACKAGE_NAME = "PACKAGE_NAME";
    public static final String BILLING_REQUEST_ITEM_ID = "ITEM_ID";
    public static final String BILLING_REQUEST_DEVELOPER_PAYLOAD = "DEVELOPER_PAYLOAD";
    public static final String BILLING_REQUEST_NOTIFY_IDS = "NOTIFY_IDS";
    public static final String BILLING_REQUEST_NONCE = "NONCE";

    public static final String BILLING_RESPONSE_RESPONSE_CODE = "RESPONSE_CODE";
    public static final String BILLING_RESPONSE_PURCHASE_INTENT = "PURCHASE_INTENT";
    public static final String BILLING_RESPONSE_REQUEST_ID = "REQUEST_ID";
    public static long BILLING_RESPONSE_INVALID_REQUEST_ID = -1;
    public static int BILLING_RESPONSE_INVALID_RESPONSE_CODE = -1;

    /**
     * These are the names of the extras that are passed in an intent from
     * Google Play to this application and cannot be changed.
     */
    public static final String BILLING_RESPONSE_NOTIFICATION_ID = "notification_id";
    public static final String BILLING_RESPONSE_INAPP_SIGNED_DATA = "inapp_signed_data";
    public static final String BILLING_RESPONSE_INAPP_SIGNATURE = "inapp_signature";
    public static final String BILLING_RESPONSE_INAPP_REQUEST_ID = "request_id";
    public static final String BILLING_RESPONSE_INAPP_RESPONSE_CODE = "response_code";

	/**
	 * The in-app billing broadcast intents that are sent by the Google Play application.
	 * They are received in the BillingReceiver.
	 * These broadcast intents inform your application about in-app billing actions that have occurred.
	 */
	/**
	 * This broadcast intent contains a Google Play response code, and is sent after you make an in-app
	 * billing request. A server response code can indicate that a billing request was successfully sent
	 * to Google Play or it can indicate that some error occurred during a billing request.
	 */
	public final static String ACTION_RESPONSE_CODE = "com.android.vending.billing.RESPONSE_CODE";

	/**
	 * This response indicates that a purchase has changed state, which means a purchase succeeded,
	 * was canceled, or was refunded.
	 */
	public final static String ACTION_NOTIFY = "com.android.vending.billing.IN_APP_NOTIFY";

	/**
	 * This broadcast intent contains detailed information about one or more transactions.
	 */
	public final static String ACTION_STATE_CHANGED = "com.android.vending.billing.PURCHASE_STATE_CHANGED";

	/**
	 * Intent actions that we send from the BillingReceiver to the BillingService.
	 */
	public final static String ACTION_GET_PURCHASE_INFORMATION = "com.mosync.java.android.GET_PURCHASE_INFORMATION";
	public final static String ACTION_CONFIRM_NOTIFICATION = "com.mosync.java.android.CONFIRM_NOTIFICATION";
	public final static String ACTION_RESTORE_TRANSACTIONS = "com.mosync.java.android.RESTORE_TRANSACTIONS";

	/**
	 * String constants used in verifying purchase signature.
	 */
	public final static String TRANSACTION_PURCHASE_STATE = "purchaseState";
	public final static String TRANSACTION_PRODUCT_ID = "productId";
	public final static String TRANSACTION_PACKAGE_NAME = "packageName";
	public final static String TRANSACTION_PURCHASE_TIME = "purchaseTime";
	public final static String TRANSACTION_ORDER_ID = "orderId";
	public final static String TRANSACTION_NOTIFICATION_ID = "notificationId";
	public final static String TRANSACTION_DEVELOPER_PAYLOAD = "developerPayload";
}