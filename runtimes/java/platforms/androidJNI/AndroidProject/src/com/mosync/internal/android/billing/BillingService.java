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

import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Queue;

import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;

import com.android.vending.billing.IMarketBillingService;
import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.android.billing.request.BaseRequest;
import com.mosync.internal.android.billing.request.GetPurchaseInformation;
import com.mosync.internal.android.billing.request.CheckBillingSupported;
import com.mosync.internal.android.billing.request.ConfirmNotifications;
import com.mosync.internal.android.billing.request.Purchase;
import com.mosync.internal.android.billing.Consts.PurchaseState;
import com.mosync.internal.android.billing.Security.VerifiedPurchase;

/**
 * This class sends messages to Google Play on behalf of the application by
 * connecting (binding) to the MarketBillingService. The application
 * creates an instance of this class and invokes billing requests through this service.
 *
 * The {@link BillingReceiver} class starts this service to process commands
 * that it receives from Google Play.
 *
 */
public class BillingService extends Service implements ServiceConnection
{

	/**
	 * Constructor.
	 */
    public BillingService()
    {
        super();
        //mThread = thread;
    }

	@Override
	public void onCreate()
	{
		//todo delete this method.
		super.onCreate();

		try {
			boolean bindResult = bindService(
								new Intent(Consts.MARKET_BILLING_SERVICE_ACTION),
								this,
								Context.BIND_AUTO_CREATE);
			if(bindResult)
			{
				Log.i("@@MoSync", "Market Billing Service Successfully bound");
			}
			else
			{
				Log.e("@@MoSync", "Market Billing Service could not be bound.");
				//TODO stop user continuing
			}
		} catch (SecurityException e){
			Log.e("@@MoSync", "Market Billing Service could not be bound. SecurityException: " + e);
			//TODO stop user continuing
		}
	}

	public void setContext(Context context)
	{
        attachBaseContext(context);
        mAppContext = context;
    }

	public void setManager(PurchaseManager manager)
	{
		mPurchaseManager = manager;
	}

    /**
     * We don't support binding to this service, only starting the service.
     * Once the service is bond, it will create a tunnel to talk with its
     * clients which is the IBinder Interface.
     * This is used by your AIDL Interface implementation and returns the IBinder.
     */
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onStart(Intent intent, int startId) {
        handleCommand(intent, startId);
    }

    @Override
    public void onDestroy()
    {
		super.onDestroy();
		Log.e("@@MoSync", "onDestroy is called!!!!!!!!!!!!!!!!");
		// todo unregister receiver.
		//todo what happens on MoSync.onDestroy().
    }

    /**
     * The {@link BillingReceiver} sends messages to this service using intents.
     * Each intent has an action and some extra arguments specific to that action.
     * @param intent the intent containing one of the supported actions
     * @param startId an identifier for the invocation instance of this service
     */
    public void handleCommand(Intent intent, int startId)
    {
        String action = intent.getAction();
        // The event that will be sent to the MoSync queue.
        int[] event = null;
        if (Consts.METHOD_CONFIRM_NOTIFICATIONS.equals(action))
        {
            String[] notifyIds = intent.getStringArrayExtra(Consts.BILLING_RESPONSE_NOTIFICATION_ID);
//            confirmNotifications(startId, notifyIds);
//            event = BillingEventHandler.onConfirmNotifications(1);

        }
        else if (Consts.ACTION_GET_PURCHASE_INFORMATION.equals(action))
        {
            String notifyId = intent.getStringExtra(Consts.BILLING_RESPONSE_NOTIFICATION_ID);
            getPurchaseInformation(startId, new String[] { notifyId });
        }
        else if (Consts.ACTION_STATE_CHANGED.equals(action))
        {
            String signedData = intent.getStringExtra(Consts.BILLING_RESPONSE_INAPP_SIGNED_DATA);
            String signature = intent.getStringExtra(Consts.BILLING_RESPONSE_INAPP_SIGNATURE);
//            purchaseStateChanged(startId, signedData, signature);

            onPurchaseStateChanged(startId, signedData, signature);
        }
        else if (Consts.ACTION_RESPONSE_CODE.equals(action))
        {/*
            long requestId = intent.getLongExtra(Consts.BILLING_RESPONSE_INAPP_REQUEST_ID, -1);
            int responseCodeIndex = intent.getIntExtra(Consts.BILLING_RESPONSE_INAPP_RESPONSE_CODE,
                    Consts.ResponseCode.RESULT_ERROR.ordinal());
            Consts.ResponseCode responseCode = Consts.ResponseCode.valueOf(responseCodeIndex);
            checkResponseCode(requestId, responseCode);*/
        }
//        if (event != null)
//        	mMoSyncThread.postEvent(event);
        else if(Consts.ACTION_NOTIFY.equals(action))
        {
			String notifyId = intent.getStringExtra(Consts.BILLING_RESPONSE_NOTIFICATION_ID);
			onPurchaseRequestCompeted(notifyId);
        }
    }

    /**
     * Binds to the MarketBillingService and returns true if the bind
     * succeeded.
     * Until onServiceConnected() gets called, mService is null.
     * @return true if the bind succeeded, false otherwise.
     */
    private boolean bindToMarketBillingService()
    {
		Log.e("@@MoSync","bindToMarketBillingService");
		if ( mService != null )
		{
			Log.e("@@MoSync","bindToMarketBillingService - already bind");
			return true;
		}
        try {
            boolean bindResult = bindService(
                    new Intent(Consts.MARKET_BILLING_SERVICE_ACTION),
                    this,  // ServiceConnection.
                    Context.BIND_AUTO_CREATE);
            // compatibility:prior to icecream sandwich flags BIND_WAIVE_PRIORITY and BIND_ADJUST_WITH_ACTIVITY

            if (bindResult)
            {
				Log.e("@@MoSync","bindToMarketBillingService TRUE");
                return true;
            }
            else
            {
                Log.e("@MoSync", "BillingService error: Could not bind to service.");
            }
        } catch (SecurityException e)
        {
            Log.e("@@MoSync", "BillingService error: Security exception: " + e);
        }
        return false;
    }

    /**
     * Checks if in-app billing is supported.
     * @return MA_PURCHASE_RES_OK if supported, error code otherwise.
     */
    public int checkBillingSupported()
    {
		CheckBillingSupported request = new CheckBillingSupported(mService);
		if ( bindToMarketBillingService() )
		{
			if ( request.runRequest() )
			{
				return request.getResponseCode();
			}
		}
		else
		{
			// Add to pending requests.
			mPendingRequests.add(request);
		}
		return -1;//false; dev_error
    }

    /**
     *
     */
    public BaseRequest requestPurchase(final String productID)
    {
		Purchase request = new Purchase(
				mService, productID, mThread.getActivity());
		if ( bindToMarketBillingService() && !mPurchaseRequestInProgress )
		{
			mPurchaseRequestInProgress = true;
			//return request.runRequest();
			if ( request.runRequest() )
			{
//    			// There is no active purchase request in progress.
//    			mPurchaseRequestInProgress = false;
				return request;
			}
		}

        request.setPendingState(true);
        // Add to pending requests.
        mPendingRequests.add(request);
        return request;
    }

    /**
     * Gets the purchase information. This message includes a list of
     * notification IDs sent to us by Android Market, which we include in
     * our request. The server responds with the purchase information,
     * encoded as a JSON string, and sends that to the {@link BillingReceiver}
     * in an intent with the action {@link Consts#ACTION_PURCHASE_STATE_CHANGED}.
     * Returns false if there was an error trying to connect to the MarketBillingService.
     *
     * @param startId an identifier for the invocation instance of this service
     * @param notifyIds a list of opaque identifiers associated with purchase
     * state changes
     * @return false if there was an error connecting to Google Play.
     */
	private int getPurchaseInformation(int startId, String[] notifyIds)
	{
		GetPurchaseInformation request = new GetPurchaseInformation(
				startId, mService, notifyIds);
		if ( bindToMarketBillingService() )
		{
			if ( request.runRequest() )
			{
				return 1;

			}
		}
		// Add to pending requests.
		mPendingRequests.add(request);
		return -1; // or request after else.
//    		return false;
	}

    /**
     * Verifies that the data was signed with the given signature, and calls
     * {@link ResponseHandler#purchaseResponse(Context, PurchaseState, String, String, long)}
     * for each verified purchase.
     * @param startId an identifier for the invocation instance of this service
     * @param signedData the signed JSON string (signed, not encrypted)
     * @param signature the signature for the data, signed with the private key
     */
    private void onPurchaseStateChanged(int startId, String signedData, String signature)
    {
        ArrayList<Security.VerifiedPurchase> purchases;
        purchases = Security.verifyPurchase(signedData, signature);
        if (purchases == null)
        {
            return;
        }

        ArrayList<String> notifyList = new ArrayList<String>();
        for (VerifiedPurchase vp : purchases)
        {
            if (vp.notificationId != null)
            {
                notifyList.add(vp.notificationId);
            }
//            ResponseHandler.purchaseResponse(this, vp.purchaseState, vp.productId,
//                    vp.orderId, vp.purchaseTime, vp.developerPayload);
        }
        if (!notifyList.isEmpty()) {
            String[] notifyIds = notifyList.toArray(new String[notifyList.size()]);
//            confirmNotifications(startId, notifyIds);
        }
    }

    /**
     *
     * @param notifyId
     */
    private void onPurchaseRequestCompeted(String notifyId)
    {
		// There is no active purchase request in progress.
		mPurchaseRequestInProgress = false;
		mPurchaseManager.setPurchaseState(mCurrentPurchaseHandle, mPurchaseRequestInProgress);
    }

    /**
     * Create the bundle for the request.
     * @param method The request method. One of the following:
     * 	- METHOD_CHECK_BILLING_SUPOPRTED
     *  - METHOD_CONFIRM_NOTIFICATIONS
     *  - METHOD_GET_PURCHASE_INFO
     *  - METHOD_REQUEST_PURCHASE
     *  - METHOD_RESTORE_TRANSACTIONS.
     * @return The created bundle.
     */
    public static Bundle createRequestBundle(final String method)
    {
		Bundle request = new Bundle();
		request.putString(Consts.BILLING_REQUEST_METHOD, method);
		// 	The version of Google Play's in-app billing service.The current version is 1.
		request.putInt(Consts.BILLING_REQUEST_API_VERSION, 1);
		request.putString(Consts.BILLING_REQUEST_PACKAGE_NAME,
				mAppContext.getPackageName());
		return request;
    }

	@Override
	public void onServiceConnected(ComponentName name, IBinder service)
	{
		Log.e("@@MoSync", "BillingService - onServiceConnected ");
		// Get the service interface.
        mService = IMarketBillingService.Stub.asInterface(service);
        runPendingRequests();
	}

	@Override
	public void onServiceDisconnected(ComponentName name)
	{
        Log.e("@@MoSync", "BillingService - onServiceDisconnected");
        mService = null;
		// Post some MoSync error event to the queue.

	}

    /**
     * Unbinds from the MarketBillingService. Call this when the application
     * terminates to avoid leaking a ServiceConnection.
     * TODO see if we need to unbind after each request, so we don't keep the connection open.
     */
    public void unbind()
    {
        try
        {
            unbindService(this);
        } catch (IllegalArgumentException e)
        {
            // This might happen if the service was disconnected.
			Log.e("@@MoSync","BillingService - unbind exception");
        }
    }

    /**
     * Checks if there is a purchase request pending or in progress, and
     * the user has completed the checkout UI process.
     * Returns false if there is no purchase request in progress.
     * @return -true if there is a Purchase request issued, and the user
     * hasn't yet finished completed the checkout UI
     * 		   -false if there is no
     *
     */
    public static boolean isPurchaseRequestInProgress()
    {
		return mPurchaseRequestInProgress;
    }

    /**
     * Runs any pending requests that are waiting for a connection to the
     * service to be established.  This runs in the main UI thread.
     */
    private void runPendingRequests()
    {
        int maxStartId = -1;
        BaseRequest request;
        while ((request = mPendingRequests.peek()) != null)
        {
            if (request.runIfConnected())
            {
                // Remove the request
                mPendingRequests.remove();

                // Remember the largest startId, which is the most recent
                // request to start this service.
                if (maxStartId < request.getStartId())
                {
                    maxStartId = request.getStartId();
                }
            }
            else
            {
			//TODO send event.
				Log.e("@@MoSync","runPendingRequests service crashed");
                // The service crashed, so restart it. Note that this leaves
                // the current request on the queue.
                bindToMarketBillingService();
                return;
            }
        }

        // If we get here then all the requests ran successfully.  If maxStartId
        // is not -1, then one of the requests started the service, so we can
        // stop it now.
        if (maxStartId >= 0)
        {
            Log.e("@@MoSync","runPendingRequests stopping service startId = " + maxStartId);
            stopSelf(maxStartId);
        }
    }
	/************************ Class members ************************/

    /**
     * The service connection to the remote MarketBillingService.
     */
	private IMarketBillingService mService = null;

	/**
	 * Application context.
	 */
	private static Context mAppContext;

	private static MoSyncThread mThread;

	/**
	 * Reference to the manager, so that the requests handle table can be accesed.
	 */
	private PurchaseManager mPurchaseManager;

	private static boolean mPurchaseRequestInProgress = false;
	public static int mCurrentPurchaseHandle = -1;

    /**
     * The list of requests that are pending while waiting for the active
     * request to be completed.
     */
    private static LinkedList<BaseRequest> mPendingRequests = new LinkedList<BaseRequest>();
//    private static Queue<BaseRequest> mR = new Queue<BaseRequest>();

    /**
     * The list of requests that we have sent to Android Market but for which we have
     * not yet received a response code. The HashMap is indexed by the
     * request Id that each request receives when it executes.
     */
//    private static HashMap<Long, BaseRequest> mSentRequests =
//        new HashMap<Long, BaseRequest>();
}
