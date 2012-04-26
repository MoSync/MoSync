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


import com.mosync.internal.android.MoSyncThread;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_PURCHASE;


/**
 *
 * @author emma
 *
 */
public class BillingEventHandler
{

	/**
	 * Constructor.
	 */
	public BillingEventHandler(MoSyncThread thread)
	{
		mMoSyncThread = thread;
	}

	/**
	 *
	 * @param purchaseHandle
	 * @param state
	 * @return
	 */
	public static int[] onPurchaseStateChanged(int purchaseHandle, int state)
	{
		int[] event = new int[3];
		event[0] = EVENT_TYPE_PURCHASE;
		event[1] = purchaseHandle;
		event[2] = state;

		return event;
	}

	private static MoSyncThread mMoSyncThread;
}
