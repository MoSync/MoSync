/* Copyright (c) 2012 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.mosync.internal.android.billing.util;

import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_IN_PROGRESS;

import org.json.JSONException;
import org.json.JSONObject;

import com.mosync.internal.android.billing.Consts;

/**
 * Represents an in-app billing purchase.
 */
public class Purchase {

	public static final int PURCHASE_HANDLE_UNKNOWN = -1;

    String mOrderId;
    String mPackageName;
    String mSku;
    long mPurchaseTime;
    int mPurchaseState;
    String mDeveloperPayload;
    String mToken;
    String mOriginalJson;
    String mSignature;
    /**
     * MoSync internal handle for the purchase.
     */
    int mProductHandle = PURCHASE_HANDLE_UNKNOWN;
	/**
	 * Purchase state is at first unknown.
	 * After the purchase request is received by Google Play, the state becomes
	 * MA_PURCHASE_STATE_IN_PROGRESS.
	 * After the purchase was made, the state becomes MA_PURCHASE_STATE_COMPLETED.
	 * If the purchase request fails at any moment, the state becomes MA_PURCHASE_STATE_FAILED.
	 * If the product is refundend, the state becomes MA_PURCHASE_STATE_REFUNDED.
	 */
	public int mState = MA_PURCHASE_STATE_IN_PROGRESS;

    public Purchase(String productID){
		mSku = productID;
		mState = Consts.PURCHASE_STATE_ON_HOLD;
    }

    public Purchase(int handle)
    {
		mProductHandle = handle;
    }

    public Purchase(String jsonPurchaseInfo, String signature) throws JSONException {
        mOriginalJson = jsonPurchaseInfo;
        JSONObject o = new JSONObject(mOriginalJson);
        mOrderId = o.optString("orderId");
        mPackageName = o.optString("packageName");
        mSku = o.optString("productId");
        mPurchaseTime = o.optLong("purchaseTime");
        mPurchaseState = o.optInt("purchaseState");
        mDeveloperPayload = o.optString("developerPayload");
        mToken = o.optString("token", o.optString("purchaseToken"));
        mSignature = signature;
    }

    public void setHandle(int handle){
		mProductHandle = handle;
    }
	public void setState(final int state)
	{
		mState = state;
	}
	public int getState()
	{
		return mState;
	}
	public void setOrderID(String orderID)
	{
		mOrderId = orderID;
	}
	public void setOrderTime(long purchaseTime)
	{
		mPurchaseTime = purchaseTime;
	}
	public void setPackageName(String name)
	{
		mPackageName = name;
	}
    public String getOrderId() { return mOrderId; }
    public String getPackageName() { return mPackageName; }
    public String getSku() { return mSku; }
    public long getPurchaseTime() { return mPurchaseTime; }
    public int getPurchaseState() { return mPurchaseState; }
    public String getDeveloperPayload() { return mDeveloperPayload; }
    public String getToken() { return mToken; }
    public String getOriginalJson() { return mOriginalJson; }
    public String getSignature() { return mSignature; }
    public int getProductHandle() { return mProductHandle; };

    @Override
    public String toString() { return "PurchaseInfo:" + mOriginalJson; }
}
