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

import java.security.PublicKey;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;

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
import com.mosync.internal.android.billing.request.RestoreTransactions;

import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RES_OUT_OF_DATE;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RES_CONNECTION_ERROR;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_FAILED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_COMPLETED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_STATE_PRODUCT_REFUNDED;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RES_MALFORMED_PUBLIC_KEY;
import static com.mosync.internal.generated.MAAPI_consts.MA_PURCHASE_RES_OK;
import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;

/**
 * This class sends messages to Google Play on behalf of the application by
 * connecting (binding) to the MarketBillingService. The application
 * creates an instance of this class and invokes billing requests through this service.
 *
 * The {@link BillingReceiver} class starts this service to process commands
 * that it receives from Google Play.
 * @author emma
 */
public class BillingService extends Service implements ServiceConnection
{

	/**
	 * Constructor.
	 */
    public BillingService()
    {
        super();
    }

	public void setContext(Context context)
	{
		if ( getBaseContext() == null )
		{
	        attachBaseContext(context);
	        mAppContext = context;
		}
    }

	public void setThread(MoSyncThread thread)
	{
		mThread = thread;
	}

	/**
	 * Compute your public key (that you got from the Android Market publisher site).
	 * @param developerPublicKey the Base64 encoded key.
	 * @return MA_PURCHASE_RES_MALFORMED_PUBLIC_KEY or MA_PURCHASE_RES_OK.
	 */
	public int setPublicKey(final String developerPublicKey)
	{
		try
		{
			mPublicKey = Security.generatePublicKey(developerPublicKey);
			mIsPublicKeySet = true;
			return MA_PURCHASE_RES_OK;
		} catch (Exception ex)
		{
			SYSLOG("maPurchaseSetPublicKey: malformed developerPublicKey");
			mPublicKey = null;
			return MA_PURCHASE_RES_MALFORMED_PUBLIC_KEY;
		}
	}

    /**
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
		SYSLOG("@@MoSync BillingService onDestroy");
    }

    /**
     * The {@link BillingReceiver} sends messages to this service using intents.
     * Each intent has an action and some extra arguments specific to that action.
     * @param intent the intent containing one of the supported actions
     * @param startId an identifier for the invocation instance of this service
     */
    public void handleCommand(Intent intent, int startId)
    {if (intent == null ) return;
        String action = intent.getAction();

        if (Consts.METHOD_CONFIRM_NOTIFICATIONS.equals(action))
        {
            String[] notifyIds = intent.getStringArrayExtra(Consts.BILLING_RESPONSE_NOTIFICATION_ID);
            confirmNotifications(startId, notifyIds);
        }
        else if (Consts.ACTION_GET_PURCHASE_INFORMATION.equals(action))
        {
            String notifyId = intent.getStringExtra(Consts.BILLING_RESPONSE_NOTIFICATION_ID);
            getPurchaseInformation(startId, new String[] { notifyId });
        }
        else if (Consts.ACTION_STATE_CHANGED.equals(action))
        {
			// State changed is received only after purchase information was retrieved.
            String signedData = intent.getStringExtra(Consts.BILLING_RESPONSE_INAPP_SIGNED_DATA);
            String signature = intent.getStringExtra(Consts.BILLING_RESPONSE_INAPP_SIGNATURE);
            onPurchaseStateChanged(startId, signedData, signature);
        }
        else if (Consts.ACTION_RESPONSE_CODE.equals(action))
        {
            long requestId = intent.getLongExtra(Consts.BILLING_RESPONSE_INAPP_REQUEST_ID, -1);
            int responseCode = intent.getIntExtra(Consts.BILLING_RESPONSE_INAPP_RESPONSE_CODE,
                    Consts.RESULT_ERROR);
            checkResponseCode(requestId, Consts.responseCodeValue(responseCode));
        }
    }

    /**
     * Check the response code received for a request and send events.
     * If the received response code is for a purchase request, then send
     * purchaseStateChanged events.
     * If the server sends invalid response code for ConfirmNotifications type
     * of request, ignore it for the moment, as the server will automatically
     * retry to communicate with the app by sending IN_APP_NOTIFY messages.
     * If the request type is RestoreTransactions, ignore the result code at this point,
     * as the user will later receive MA_PURCHASE_EVENT_RESTORE events for each product.
     * If the request type is getPurchaseInformation, the error code will be used later
     * when sending an RECEIPT_ERROR event, after maPurchaseVerifyReceipt is called.
     *
     * @param requestId The identifier of the request (as received from Google Play).
     * @param responseCode The request response code.
     */
    private void checkResponseCode(long requestId, int responseCode)
    {
		// Get the request object from the sent list.
		BaseRequest request = mSentRequests.get(requestId);

		if ( request != null && request instanceof Purchase )
		{
			if ( responseCode != Consts.RESULT_OK )
			{
				PurchaseManager.onPurchaseStateChanged(
						MA_PURCHASE_STATE_FAILED, request.getHandle(), responseCode);
			}
			mSentRequests.remove(requestId);
		}
    }

    /**
     * Binds to the MarketBillingService and returns true if the bind
     * succeeded.
     * Until onServiceConnected() gets called, mService is null.
     * @return true if the bind succeeded, false otherwise.
     */
    boolean bindToMarketBillingService()
    {
		if ( mService != null )
		{
			SYSLOG("@@MoSync BillingService: bindToMarketBillingService already bind");
			return true;
		}
        try {
            boolean bindResult = bindService(
                    new Intent(Consts.MARKET_BILLING_SERVICE_ACTION),
                    this,  // ServiceConnection.
                    Context.BIND_AUTO_CREATE);
            // Compatibility: prior to icecream sandwich flags BIND_WAIVE_PRIORITY and BIND_ADJUST_WITH_ACTIVITY

            if (bindResult)
            {
				SYSLOG("@@MoSync BillingService bindToMarketBillingService success");
                return true;
            }
            else
            {
                SYSLOG("@MoSync BillingService bindToMarketBillingService: Could not bind to service.");
            }
        } catch (SecurityException e)
        {
            SYSLOG("@@MoSync BillingService bindToMarketBillingService error: Security exception: " + e);
        }
        return false;
    }

    /**
     * Checks if in-app billing is supported.
     * It needs to be synchronous, therefore the service binding is done
     * before this call (in manager's constructor) so that we don't need
     * to wait for the onServiceConnected() callback.
     * @return MA_PURCHASE_RES_OK if supported, error code otherwise.
     */
    public int checkBillingSupported()
    {
		CheckBillingSupported request = new CheckBillingSupported(mService);
		if ( mService != null )
		{
			try{
				request.run();
				return request.getResponseCode();
			}catch(RemoteException e)
			{
				SYSLOG("@@MoSync maPurchaseSupported remote exception,out of date");
				return MA_PURCHASE_RES_OUT_OF_DATE;
//				mService = null;
			}
		}
		else
		{
			// No need to add to pending requests, service cannot bind.
			SYSLOG("@@MoSync maPurchaseSupported error: cannot bind to MarketBillingService");
			return MA_PURCHASE_RES_CONNECTION_ERROR;
		}
    }

    /**
     * Send a purchase request to Google Play.
     * Send the productHandle to GooglePlay as the developerPayload,
     * so that the transaction can be later identified.
     * There can only be one request at a time.
     * @param productID The product identifier.
     * @param handle The internal product handle.
     * @return the request.
     */
    public BaseRequest requestPurchase(final String productID, final int handle)
    {
		Purchase request = new Purchase(
				mService, productID, handle, mThread.getActivity());
		if ( mPurchaseRequestInProgress )
		{
			SYSLOG("MoSync BillingRequest placed on the queue.");
			request.setPendingState(true);
			mPendingRequests.add(request);
			return request;
		}
		request.runRequest();

		if ( request.getRequestID() == Consts.BILLING_RESPONSE_INVALID_REQUEST_ID )
		{
			return null;
		}

		mPurchaseRequestInProgress = true;
        request.setPendingState(false);
        mSentRequests.put(request.getRequestID(), request);
        return request;
    }

    /**
     * Check if the public key was set.
     * @return
     */
    public boolean isPublicKeySet()
    {
		return mIsPublicKeySet;
    }

    /**
     * Check if a connection to the billing server was made.
     */
    boolean isConnected()
    {
		if ( mService != null )
			return true;
		return false;
    }

    /**
     * Gets the purchase information. This message includes a list of
     * notification IDs sent to us by Android Market, which we include in
     * our request. The server responds with the purchase information,
     * encoded as a JSON string, and sends that to the {@link BillingReceiver}
     * in an intent with the action {@link Consts#ACTION_PURCHASE_STATE_CHANGED}.
     *
     * @param startId an identifier for the invocation instance of this service
     * @param notifyIds a list of opaque identifiers associated with purchase
     * state changes.
     */
	private void getPurchaseInformation(int startId, String[] notifyIds)
	{
		GetPurchaseInformation request = new GetPurchaseInformation(
				startId, mService, notifyIds);

		if ( bindToMarketBillingService() )
		{
			if ( request.runRequest() )
			{
				mSentRequests.put(request.getRequestID(), request);
				return;
			}
		}
		// Add to pending requests.
		mPendingRequests.add(request);
	}

    /**
     * Verifies that the data was signed with the given signature,
     * for each purchase.
     * @param startId an identifier for the invocation instance of this service.
     * @param signedData the signed JSON string (signed, not encrypted).
     * @param signature the signature for the data, signed with the private key.
     */
    private void onPurchaseStateChanged(int startId, String signedData, String signature)
    {
        ArrayList<PurchaseInformation> purchases;
        purchases = Security.verifyPurchase(mPublicKey, signedData, signature);
        if ( purchases != null )
        {
            ArrayList<String> notifyList = new ArrayList<String>();
            for (PurchaseInformation purchaseObj : purchases)
            {
                if (purchaseObj.mNotificationID != null)
                {
                    notifyList.add(purchaseObj.mNotificationID);
                }

                // Check if the product was restored, or refunded.
                if ( mRestoringTransactions )
                {
					PurchaseManager.onPurchaseRestored(purchaseObj);
					mRestoringTransactions = false;
                }
                else if ( purchaseObj.getState() == MA_PURCHASE_STATE_PRODUCT_REFUNDED )
                {
					PurchaseManager.onPurchaseRefunded(purchaseObj);
                }
                else if ( purchaseObj.getState() == MA_PURCHASE_STATE_COMPLETED )
                {
					PurchaseManager.onTransactionInformationReceived(purchaseObj);
                }
                // Notify the user with the transaction details, and only
				// after that confirmNotifications.
                if (!notifyList.isEmpty())
                {
                    String[] notifyIds = notifyList.toArray(new String[notifyList.size()]);
                    confirmNotifications(startId, notifyIds);
                }
            }
        }
    }

    /**
     * Confirms receipt of a purchase state change. Each {@code notifyId} is
     * an opaque identifier that came from the server. This method sends those
     * identifiers back to the MarketBillingService, which ACKs them to the
     * server.
     * At this moment, the request state is COMPLETED.
     * @param startId an identifier for the invocation instance of this service
     * @param notifyIds a list of opaque identifiers associated with purchase
     * state changes.
     */
    private void confirmNotifications(int startId, String[] notifyIds)
    {
		ConfirmNotifications request = new ConfirmNotifications(startId, mService, notifyIds);
		request.runRequest();

		if ( request.getRequestID() == Consts.BILLING_RESPONSE_INVALID_REQUEST_ID )
		{
			SYSLOG("@@MoSync maPurchaseRequest There was an error when confirming the transaction information");
		}
    }

    public RestoreTransactions restoreTransactions()
    {
		RestoreTransactions request = new RestoreTransactions(mService);
		mRestoringTransactions = true;
		request.runRequest();
		if ( request.getRequestID() == Consts.BILLING_RESPONSE_INVALID_REQUEST_ID )
		{
			mRestoringTransactions = false;
			return null;
		}

        mSentRequests.put(request.getRequestID(), request);
		return request;
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
		SYSLOG("@@MoSync BillingService - onServiceConnected !");
		// Get the service interface.
        mService = IMarketBillingService.Stub.asInterface(service);
        runPendingRequests();
	}

	@Override
	public void onServiceDisconnected(ComponentName name)
	{
        SYSLOG("@@MoSync BillingService - onServiceDisconnected !");
        mService = null;
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
			SYSLOG("@@MoSync BillingService - unbind exception");
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
     * Set the state of the current running request.
     * @param stateInProgress
     */
    public static void setPurchaseRequestInProgress(boolean stateInProgress)
    {
		mPurchaseRequestInProgress = stateInProgress;
    }

    /**
     * Runs any pending requests that are waiting for a connection to the
     * service to be established.
     */
    private void runPendingRequests()
    {
        int maxStartId = -1;
        BaseRequest request;
        while ((request = mPendingRequests.peek()) != null)
        {
			request.setService(mService);
			mCurrentPurchaseHandle = request.getHandle();
            if (request.runIfConnected())
            {
                // Remove the request.
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
				SYSLOG("@@MoSync BillingService runPendingRequests service crashed");
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
            SYSLOG("@@MoSync runPendingRequests stopping service startId = " + maxStartId);
            stopSelf(maxStartId);
        }
    }
	/************************ Class members ************************/

    /**
     * The service connection to the remote MarketBillingService.
     */
	private static IMarketBillingService mService = null;

	/**
	 * Application context.
	 */
	private static Context mAppContext;
	private static MoSyncThread mThread;

	/**
	 * The developer public key.
	 */
	private static PublicKey mPublicKey;
	private static boolean mIsPublicKeySet = false;

	private static boolean mPurchaseRequestInProgress = false;
	public static int mCurrentPurchaseHandle = -1;

    /**
     * The list of requests that are pending while waiting for the active
     * request to be completed.
     * CheckBillingSupported request cannot be pending, as it is synchronous.
     */
    private static LinkedList<BaseRequest> mPendingRequests = new LinkedList<BaseRequest>();

    /**
     * The list of requests that we have sent to Android Market but for which we have
     * not yet received a response code. The HashMap is indexed by the
     * request Id that each request receives when it executes.
     */
    private static HashMap<Long, BaseRequest> mSentRequests =
        new HashMap<Long, BaseRequest>();

    /**
     * Keeps track of restoreTransaction requests that are in progress.
     */
    private boolean mRestoringTransactions = false;
}
