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

/**
 *
 * @author emma
 *
 */
public class BillingConsts {



	/**
	 * The in-app billing broadcast intents that are sent by the Google Play application.
	 * They are received in the BillingReceiver.
	 * These broadcast intents inform your application about in-app billing actions that have occurred.
	 * These constants cannot be changed.
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
}
