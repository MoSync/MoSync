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

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_PURCHASE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_EVENT_PRODUCT_CREATE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_EVENT_REQUEST;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_EVENT_REFUNDED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_EVENT_RESTORED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_EVENT_VERIFY_RECEIPT;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_RECEIPT_INVALID;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_RECEIPT_VALID;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_RECEIPT_ERROR;

/**
 * Utility class for creating MoSync events.
 * @author emma
 *
 */
public class BillingEvent
{

	public static int[] onProductCreate(int purchaseHandle, int state)
	{
		return setPurchaseEventData(
				MA_PURCHASE_EVENT_PRODUCT_CREATE, state, purchaseHandle, 0);
	}

	public static int[] onPurchaseStateChanged(int purchaseHandle, int state, int errorCode)
	{
		return setPurchaseEventData(
				MA_PURCHASE_EVENT_REQUEST, state, purchaseHandle, errorCode);
	}

	public static int[] onTransactionInfo()
	{
		return setPurchaseEventData(
				MA_PURCHASE_EVENT_VERIFY_RECEIPT, MA_PURCHASE_STATE_RECEIPT_VALID, 0, 0);
	}

	public static int[] onRestoreTransaction(int state, int purchaseHandle)
	{
		return setPurchaseEventData(
				MA_PURCHASE_EVENT_RESTORED, state, purchaseHandle, 0);
	}

	private static int[] setPurchaseEventData(int type, int state, int purchaseHandle, int errorCode)
	{
		int[] event = new int[5];
		event[0] = EVENT_TYPE_PURCHASE;
		event[1] = type;
		event[2] = state;
		event[3] = purchaseHandle;
		event[4] = errorCode;

		return event;
	}

}