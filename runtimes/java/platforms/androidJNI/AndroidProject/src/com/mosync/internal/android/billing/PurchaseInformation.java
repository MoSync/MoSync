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

import java.util.Date;

import com.mosync.internal.android.billing.request.BaseRequest;

/**
 * Purchase class holds information about a purchase.
 * Available fields: prodID, quantity, time, price, state.
 * @author emma
 *
 */
public class PurchaseInformation
{
	/**
	 * Constructor
	 */
	public PurchaseInformation(final String productID)
	{
		mProductID = productID;
		mState = Consts.PURCHASE_STATE_ON_HOLD;
	}

	public void setHandle(int handle)
	{
		mHandle = handle;
	}

	public int getHandle()
	{
		return mHandle;
	}

	public String getProductID()
	{
		return mProductID;
	}

	public void setState(final int state)
	{
		mState = state;
	}

	public int getState()
	{
		return mState;
	}

	public void setRequest(final BaseRequest reqObject)
	{
		mRequest = reqObject;
	}

	public BaseRequest getRequest()
	{
		return mRequest;
	}

	public void setNotificationID(final String notifId)
	{
		mNotificationID = notifId;
	}

	public String getNotificationId()
	{
		return mNotificationID;
	}

//	+ add member Request !!!!!!!!!!!!!!!

	/************************ Class members ************************/
	/**
	 * The MoSync thread object.
	 */
	//private MoSyncThread mMoSyncThread;

	/**
	 * The internal handle of the purchase.
	 */
	private int mHandle = -1;

	/**
	 *
	 */
	private BaseRequest mRequest = null;

	/**
	 * The product id.
	 */
	private String mProductID = null;

	/**
	 * The notification Id that Google Play is sending when a purchase
	 * request was successfull. Used in GetPurchaseInformation requests.
	 */
	private String mNotificationID = null;

	private int mPrice = 0;
	private int mQuantity = 0;
	private Date mTime;

	/**
	 * Purchase state is at first unknown.
	 * After the purchase request is received by Google Play, the state becomes
	 * MA_PURCHASE_STATE_IN_PROGRESS.
	 * After the purchase was made, the state becomes MA_PURCHASE_STATE_COMPLETED.
	 * If the purchase request fails at any moment, the state becomes MA_PURCHASE_STATE_FAILED.
	 */
	private int mState;//= Consts.PurchaseState.MA_PURCHASE_STATE_UNKNOWN;
}
