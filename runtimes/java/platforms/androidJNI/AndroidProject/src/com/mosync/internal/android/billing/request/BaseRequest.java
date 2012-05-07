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

package com.mosync.internal.android.billing.request;

import android.os.RemoteException;
import android.util.Log;

import com.android.vending.billing.IMarketBillingService;


/**
 * The base class for all requests that use the MarketBillingService.
 * Each derived class overrides the run() method to call the appropriate
 * service interface.  If we are already connected to the MarketBillingService,
 * then we call the run() method directly. Otherwise, we bind
 * to the service and save the request on a queue to be run later when
 * the service is connected.
 * Explicit requests:
 * 	- CheckBillingSupported synchronous request.
 *  - Purchase that sends a purchase message to Google Play.
 *  - RestoreTransactions that retrieves a user's transaction status for managed purchases.
 * Implicit requests:
 * 	- GetPurchaseInformation that retrieves the details of a purchase state changed, either
 * failed or a purchase was billed successfully.
 *  - ConfirmNotifications confirms that a GetPurchaseInformation request was sent
 * and the app received the purchase info for the notification.
 */
public abstract class BaseRequest
{
	/**
	 * Constructor.
	 * @param startId
	 * @param service instance of MarketBillingService interface.
	 */
    public BaseRequest(int startId, IMarketBillingService service)
    {
        mStartId = startId;
        mService = service;
    }

    public int getStartId()
    {
        return mStartId;
    }

    public long getRequestID()
    {
		return mRequestId;
    }

    public boolean isPending()
    {
		return mPending;
    }

    public void setPendingState(boolean pending)
    {
		mPending = pending;
    }

    public int getResponseCode()
    {
		return mResponseCode;
    }

    /**
     * Run the request, starting the connection if necessary.
     * @return true if the request was executed or queued; false if there
     * was an error starting the connection
     */
    public boolean runRequest()
    {
		Log.e("@@MoSync","IN runRequest");
        if (runIfConnected())
        {
            return true;
        }
        if ( mService != null )
        {
            // Add a pending request to run when the service is connected.
//            mPendingRequests.add(this);
            return true;
        }

        return false;
    }

    /**
     * Try running the request directly if the service is already connected.
     * @return true if the request ran successfully; false if the service
     * is not connected or there was an error when trying to use it.
     */
    public boolean runIfConnected()
    {
		Log.e("@@MoSync","IN runIfConnected");
        if (mService != null)
        {
            try {
                //mRequestId = run();
				run();
                Log.e("emma", " runIfConnected() !!!!!!!!!!!!!!!!!!!!!request id: " + mRequestId);

                if (mRequestId >= 0)
                {
//                    mSentRequests.put(mRequestId, this);
                }
                return true;
            } catch (RemoteException e) {
                onRemoteException(e);
            }
        }
        Log.e("@@MoSync","runIfConnected mService is NULL");
//        return Consts.SERVICE_CANNOT_CONNECT;
        return false;
    }

    /**
     * Called when a remote exception occurs while trying to execute the
     * {@link #run()} method.  The derived class can override this to
     * execute exception-handling code.
     * @param e the exception
     */
    protected void onRemoteException(RemoteException e) {
        Log.e("@@MoSync", "Purchase remote billing service crashed!");
        mService = null;
    }

    /**
     * The derived class must implement this method.
     * @throws RemoteException
     */
    abstract public void run() throws RemoteException;

    /**
     * This is called when Android Market sends a response code for a Purchase
     * or RestoreTransactions request.
     * @param responseCode the response code
     */
//    public void responseCodeReceived(int responseCode)
//    {
////    	mResponseCode = (int)responseCode;
//    }
    protected void stateChanged(int state)
    {

    }
	/************************ Class members ************************/

    /**
     *
     */
    private final int mStartId;

    /**
     * Used to identify requests while handling messages.
     */
    //private String mDeveloperPayload;

    private boolean mPending = false;
    /**
     * ReqID received from Google Play service.
     */
    protected long mRequestId;

    protected int mResponseCode;
    protected IMarketBillingService mService = null;
}
