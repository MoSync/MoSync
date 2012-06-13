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

import android.text.TextUtils;

import com.mosync.internal.android.billing.request.BaseRequest;
import com.mosync.nativeui.util.properties.IntConverter;

import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_IN_PROGRESS;

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

    public PurchaseInformation(int purchaseState, String notificationId,
            String productId, String orderId, long purchaseTime,
            String packageName, String payload)
    {
        mState = purchaseState;
        mNotificationID = notificationId;
        mProductID = productId;
        mOrderID = orderId;
        mTime = purchaseTime/1000;
        mPackageName = packageName;
        mPayload = payload;
        if ( !TextUtils.isEmpty(payload) )
			mHandle = IntConverter.convert(payload);
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

	public String getPackageName()
	{
		return mPackageName;
	}

	public void setPackageName(final String pck)
	{
		mPackageName = pck;
	}

	public void setDeveloperPayload(final String devPayload)
	{
		mPayload = devPayload;
	}

	public void setOrderId(final String id)
	{
		mOrderID = id;
	}

	public void setState(final int state)
	{
		mState = state;
	}

	public int getState()
	{
		return mState;
	}

	public String getOrderId()
	{
		return mOrderID;
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
	/************************ Class members ************************/

	/**
	 * The internal handle of the purchase.
	 */
	private int mHandle = -1;
	private BaseRequest mRequest;

	/**
	 * The product id.
	 */
	private String mProductID;

	/**
	 * The notification Id that Google Play is sending when a purchase
	 * request was successfull. Used in GetPurchaseInformation requests.
	 */
	public String mNotificationID;
	public String mOrderID;
	public String mPackageName;
	/**
	 * The time the product was purchased, in milliseconds since the epoch (Jan 1, 1970).
	 * Send it to MoSync in seconds.
	 */
	public long mTime;
	public String mPayload;

	/**
	 * Purchase state is at first unknown.
	 * After the purchase request is received by Google Play, the state becomes
	 * MA_PURCHASE_STATE_IN_PROGRESS.
	 * After the purchase was made, the state becomes MA_PURCHASE_STATE_COMPLETED.
	 * If the purchase request fails at any moment, the state becomes MA_PURCHASE_STATE_FAILED.
	 * If the product is refundend, the state becomes MA_PURCHASE_STATE_REFUNDED.
	 */
	public int mState = MA_PURCHASE_STATE_IN_PROGRESS;
}