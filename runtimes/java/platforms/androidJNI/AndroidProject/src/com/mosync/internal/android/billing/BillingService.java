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

import mosync.billing.example.BillingReceiver;
import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.util.Log;

import com.android.vending.billing.IMarketBillingService;

/**
 * This class sends messages to Google Play on behalf of the application by
 * connecting (binding) to the MarketBillingService. The application
 * creates an instance of this class and invokes billing requests through this service.
 *
 * The {@link BillingReceiver} class starts this service to process commands
 * that it receives from Google Play.
 *
 */
public class BillingService extends Service implements ServiceConnection{

	private static final String TAG = "BillingService";

	/** The service connection to the remote MarketBillingService. */
	private IMarketBillingService mService;

	@Override
	public void onCreate() {
		super.onCreate();
		//Log.i(TAG, "Service starting with onCreate");

		try {
			boolean bindResult = bindService(new Intent("com.android.vending.billing.MarketBillingService.BIND"), this, Context.BIND_AUTO_CREATE);
			if(bindResult){
				Log.i(TAG,"Market Billing Service Successfully Bound");
			} else {
				Log.e(TAG,"Market Billing Service could not be bound.");
				//TODO stop user continuing
			}
		} catch (SecurityException e){
			Log.e(TAG,"Market Billing Service could not be bound. SecurityException: "+e);
			//TODO stop user continuing
		}
	}

	public void setContext(Context context) {
        attachBaseContext(context);
    }

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}

	@Override
	public void onServiceConnected(ComponentName name, IBinder service) {
		Log.i(TAG, "Market Billing Service Connected.");
		mService = IMarketBillingService.Stub.asInterface(service);
		BillingHelper.instantiateHelper(getBaseContext(), mService);
	}

	@Override
	public void onServiceDisconnected(ComponentName name) {

	}

}
