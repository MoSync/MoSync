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

package com.mosync.internal.android;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_LOCATION;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_LOCATION_PROVIDER;
import static com.mosync.internal.generated.MAAPI_consts.MA_LOC_QUALIFIED;
import static com.mosync.internal.generated.MAAPI_consts.MA_LPS_AVAILABLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_LPS_OUT_OF_SERVICE;
import static com.mosync.internal.generated.MAAPI_consts.MA_LPS_TEMPORARILY_UNAVAILABLE;

import android.app.Activity;
import android.content.Context;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.location.LocationProvider;
import android.os.Bundle;
import android.os.Looper;
import android.util.Log;

public class MoSyncLocation
{
	/**
	 * The MoSync thread object.
	 */
	MoSyncThread mMoSyncThread;
	
	LocationEventThread mLocationEventThread = null;

	/**
	 * Constructor.
	 * @param thread The MoSync thread.
	 */
	public MoSyncLocation(MoSyncThread thread)
	{
		mMoSyncThread = thread;
	}

	/**
	 * @return The Activity object.
	 */
	private Activity getActivity()
	{
		return mMoSyncThread.getActivity();
	}
	
	public int maLocationStart()
	{
		if (null == mLocationEventThread)
		{
			mLocationEventThread = new LocationEventThread();
			mLocationEventThread.start();
		}
		
		return 1;
	}
	
	public int maLocationStop()
	{
		if (null != mLocationEventThread)
		{
			mLocationEventThread.kill();
			mLocationEventThread = null;
		}
		
		return 0;
	}
	
	class LocationEventThread extends Thread implements LocationListener
	{
		LocationManager mLocationManager;
		Looper mLooper;
	
		public LocationEventThread()
		{
		}
	
		public void kill()
		{
			mLocationManager.removeUpdates(this);
			if (null != mLooper)
			{
				mLooper.quit();
			}
		}
		
		void postOutOfServiceMessage()
		{
			int[] event = new int[2];
			event[0] = EVENT_TYPE_LOCATION_PROVIDER;
			event[1] = MA_LPS_OUT_OF_SERVICE;
			mMoSyncThread.postEvent(event);
		}
		
		public void run()
		{
			try
			{
				Looper.prepare();
	
				mLooper = Looper.myLooper();
			
				mLocationManager = (LocationManager) 
					getActivity().getSystemService(Context.LOCATION_SERVICE);
				if (mLocationManager == null) 
				{
					postOutOfServiceMessage();
					return;
				}
			
				if (!mLocationManager.isProviderEnabled(
						LocationManager.GPS_PROVIDER))
				{
					postOutOfServiceMessage();
					return;
				}
	
				mLocationManager.requestLocationUpdates(
					LocationManager.GPS_PROVIDER, 0, 0, this);
				mLocationManager.requestLocationUpdates(
					LocationManager.NETWORK_PROVIDER, 0, 0, this);
				
				Looper.loop();
			}
			catch (Throwable e)
			{
				Log.i("MoSyncLocation.LocationEventThread", "Error: " + e);
				postOutOfServiceMessage();
			}
		}
	
		public void onLocationChanged(Location location)
		{
			int[] event = new int[11];
			event[0] = EVENT_TYPE_LOCATION;
			event[1] = MA_LOC_QUALIFIED;
	
			// LATITUDE
			long lat = Double.doubleToLongBits(location.getLatitude());
			event[2] = (int)lat; 
			event[3] = ((int)(lat >> 32));
			
			// LONGITUDE
			long lon = Double.doubleToLongBits(location.getLongitude());
			event[4] = (int)lon; 
			event[5] = ((int)(lon >> 32));
			
			// ACCURACCY
			long acc = Double.doubleToLongBits(location.getAccuracy());
			// Latitude accuraccy
			event[6] = (int)acc; 
			event[7] = ((int)(acc >> 32));
			// Longitude accuraccy (same value)
			event[8] = (int)acc; 
			event[9] = ((int)(acc >> 32));
			
			// ALTITUDE
			event[10] = (int) Float.floatToIntBits((float)location.getAltitude());
	
			mMoSyncThread.postEvent(event);
		}
		
		public void onProviderDisabled(String provider)
		{
			if (provider.equals(LocationManager.GPS_PROVIDER))
			{
				postOutOfServiceMessage();
			}
		}
		
		public void onProviderEnabled(String provider)
		{
			if (provider.equals(LocationManager.GPS_PROVIDER))
			{
				int[] event = new int[2];
				event[0] = EVENT_TYPE_LOCATION_PROVIDER;
				event[1] = MA_LPS_AVAILABLE;
				mMoSyncThread.postEvent(event);
			}
		}
	
		public void onStatusChanged(String provider, int status, Bundle extras)
		{
			if (provider.equals(LocationManager.GPS_PROVIDER))
			{
				int[] event = new int[2];
				
				event[0] = EVENT_TYPE_LOCATION_PROVIDER;
				
				switch (status)
				{
					case LocationProvider.OUT_OF_SERVICE: 
						event[1] = MA_LPS_OUT_OF_SERVICE; 
						break;
					case LocationProvider.TEMPORARILY_UNAVAILABLE: 
						event[1] = MA_LPS_TEMPORARILY_UNAVAILABLE; 
						break;
					case LocationProvider.AVAILABLE: 
						event[1] = MA_LPS_AVAILABLE; 
						break;
					default: 
						event[1] = MA_LPS_TEMPORARILY_UNAVAILABLE; 
				}
				
				mMoSyncThread.postEvent(event);
			}
		}
	}
}
