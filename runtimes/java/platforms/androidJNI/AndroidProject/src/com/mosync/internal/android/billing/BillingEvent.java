/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

package com.mosync.internal.android.billing;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_PURCHASE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_EVENT_PRODUCT_CREATE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_EVENT_REQUEST;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_EVENT_RESTORE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_EVENT_VERIFY_RECEIPT;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_RECEIPT_VALID;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_EVENT_REFUNDED;

/**
 * Utility class for creating MoSync events.
 * @author emma
 *
 */
public class BillingEvent
{

	public static int[] onProductCreate(int purchaseHandle, int state, int errorCode)
	{
		return setPurchaseEventData(
				MA_PURCHASE_EVENT_PRODUCT_CREATE, state, purchaseHandle, errorCode);
	}

	public static int[] onPurchaseStateChanged(int purchaseHandle, int state, int errorCode)
	{
		return setPurchaseEventData(
				MA_PURCHASE_EVENT_REQUEST, state, purchaseHandle, errorCode);
	}

	public static int[] onTransactionInfoParsed()
	{
		return setPurchaseEventData(
				MA_PURCHASE_EVENT_VERIFY_RECEIPT, MA_PURCHASE_STATE_RECEIPT_VALID, 0, 0);
	}

	public static int[] onRestoreTransaction(int state, int purchaseHandle, int errorCode)
	{
		return setPurchaseEventData(
				MA_PURCHASE_EVENT_RESTORE, state, purchaseHandle, errorCode);
	}

	public static int[] onProductRefund(int state, int purchaseHandle, int errorCode)
	{
		return setPurchaseEventData(
				MA_PURCHASE_EVENT_REFUNDED, state, purchaseHandle, errorCode);
	}

	public static int[] onVerifyReceipt(int handle, int state, int errorCode)
	{
		return setPurchaseEventData(
				MA_PURCHASE_EVENT_VERIFY_RECEIPT,
				state,
				handle,
				errorCode);
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