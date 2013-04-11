/* Copyright (C) 2011 MoSync AB

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

package com.mosync.nativeui.ui.ads;

import java.nio.ByteBuffer;

import android.app.Activity;
import android.util.Log;

import com.google.ads.AdSize;
import com.google.ads.AdView;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.java.android.MoSync;
import com.mosync.nativeui.ui.widgets.Layout;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.util.HandleTable;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_RES_INVALID_BANNER_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_RES_INVALID_LAYOUT_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_RES_INVALID_PROPERTY_NAME;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_RES_INVALID_PROPERTY_VALUE;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_RES_INVALID_STRING_BUFFER_SIZE;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_RES_ERROR;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_SIZE_BANNER;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_SIZE_IAB_BANNER;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_SIZE_LEADERBOARD;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_SIZE_RECT;

/**
 * Class for handling ads.
 * @author emma tresanszki
 */
public class Ads
{

	/**
	 * Constructor.
	 * @param thread The MoSync thread.
	 * @param activity The Activity in which the ads should be created.
	 */
	public Ads(MoSyncThread thread, Activity activity)
	{
		mMoSyncThread = thread;
		m_activity = activity;
	}

	/**
	 * Internal function for the maAdsBannerCreate system call.
	 * Creates a banner, but does not request for ads yet.
	 *
	 * Note: Should only be called on the UI thread.
	 */
	public int maAdsBannerCreate(int bannerSize, String publisherID)
	{
		int nextHandle = m_AdsTable.getNextHandle( );
		AdWidget ad = createAd(m_activity, nextHandle, bannerSize, publisherID);
		if( null != ad )
		{
			m_AdsTable.add( nextHandle, ad );
			return nextHandle;
		}

		Log.e("MoSync", "maAdsBannerCreate: Error while creating ad banner" );
		return MA_ADS_RES_ERROR;
	}

	/**
	 * Create an ad with the specified type and publisher id.
	 * @param activity The app activity.
	 * @param handle The ad internal handle.
	 * @param bannerSize The banner size.
	 * @param publisherID
	 * @return the result code.
	 */
	public AdWidget createAd(Activity activity, int handle, int bannerSize, String publisherID)
	{
		AdSize adSize;
		switch(bannerSize)
		{
		case MA_ADS_SIZE_BANNER:
			adSize = AdSize.BANNER;
			break;
		case MA_ADS_SIZE_IAB_BANNER:
			adSize = AdSize.IAB_BANNER;
			break;
		case MA_ADS_SIZE_LEADERBOARD:
			adSize = AdSize.IAB_LEADERBOARD;
			break;
		case MA_ADS_SIZE_RECT:
			adSize = AdSize.IAB_MRECT;
			break;
		default:
			adSize = AdSize.BANNER;
			break;
		}
		AdView adView = new AdView(activity, adSize, publisherID);

		return new AdWidget(handle, adView, mMoSyncThread);
	}

	/**
	 * Internal function for the maAdsAddBannerToLayout system call.
	 * Adds a banner to a given parent, the parent must be of type Layout.
	 *
	 * Note: Should only be called on the UI thread.
	 */
	public int maAdsAddBannerToLayout(int childHandle, int parentHandle, Widget parent)
	{
		if( childHandle == parentHandle )
		{
			Log.e( "MoSync", "maAdsAddBannerToLayout: Child and parent are the same." );
			return MA_ADS_RES_ERROR;
		}

		AdWidget child = m_AdsTable.get( childHandle );

		if( null == child )
		{
			Log.e( "MoSync", "maAdsAddBannerToLayout: Invalid ad banner handle: " + childHandle );
			return MA_ADS_RES_INVALID_BANNER_HANDLE;
		}
		if( null == parent )
		{
			Log.e( "MoSync", "maAdsAddBannerToLayout: Invalid parent widget handle: " + parentHandle );
			return MA_ADS_RES_INVALID_LAYOUT_HANDLE;
		}
		if ( null != child.getParent() )
		{
			Log.e( "MoSync", "maAdsAddBannerToLayout: Child already has a parent." );
			return MA_ADS_RES_ERROR;
		}
		if ( parent.isLayout() )
		{
			Layout parentAsLayout = (Layout) parent;
			parentAsLayout.addChildAt( child, -1 );
		}
		else
		{
			Log.e( "MoSync", "maAdsAddBannerToLayout: Parent " + parentHandle + " is not a layout." );
			return MA_ADS_RES_INVALID_LAYOUT_HANDLE;
		}
		return MA_ADS_RES_OK;
	}

	/**
	 * Internal function for the maAdsRemoveBannerFromLayout system call
	 * that runs on the UI thread.
	 * Removes an ad widget from its parent, but keeps a reference to it.
	 */
	public int maAdsRemoveBannerFromLayout(final int bannerHandle, final int layoutHandle, final Widget layoutWidget)
	{
		AdWidget child = m_AdsTable.get( bannerHandle );
		if( null == child )
		{
			Log.e( "MoSync", "maAdsRemoveBannerFromLayout: Invalid ad banner handle: " + bannerHandle );
			return MA_ADS_RES_INVALID_BANNER_HANDLE;
		}

		Widget parent = child.getParent( );
		if( null == parent )
		{
			Log.e( "MoSync", "maAdsRemoveBannerFromLayout: AdWidget " + bannerHandle + " has no parent." );
			return MA_ADS_RES_INVALID_BANNER_HANDLE;
		}

		if ( parent.isLayout() && parent == layoutWidget )
		{
			Layout parentAsLayout = (Layout) parent;
			parentAsLayout.removeChild( child );
		}
		else
		{
			Log.e( "MoSync", "maAdsRemoveBannerFromLayout: Parent for " + bannerHandle + " is not a layout." );
			return MA_ADS_RES_INVALID_LAYOUT_HANDLE;
		}

		return MA_ADS_RES_OK;
	}

	/**
	 * Internal function for the maAdsBannerDestroy system call
	 * that runs on the UI thread.
	 * Destroys the given ad widget handle.
	 */
	public int maAdsBannerDestroy(int bannerHandle)
	{
		AdWidget ad = m_AdsTable.get( bannerHandle );
		if( null == ad )
		{
			Log.e( "MoSync", "maAdsBannerDestroy: Invalid ad banner handle: " + bannerHandle );
			return MA_ADS_RES_INVALID_BANNER_HANDLE;
		}

		// Destroy internally the ad view.
		ad.destroyAd();

		// Disconnect widget from widget tree.
		Layout parent = (Layout) ad.getParent( );
		if( null != parent )
		{
			parent.removeChild( ad );
		}

		// Destroy widget.
		m_AdsTable.remove( ad.getHandle() );

		return MA_ADS_RES_OK;
	}

	/**
	 * Internal function for the maAdsBannerSetProperty system call.
	 * Sets a property on the given ad banner, by accessing it from
	 * the ads table and calling its setProperty method.
	 *
	 * Note: Should only be called on the UI thread.
	 */
	public int maAdsBannerSetProperty(
		final int adHandle,
		final String key,
		final String value)
	{
		AdWidget adBanner = m_AdsTable.get( adHandle );
		if( null == adBanner )
		{
			Log.e( "MoSync", "maAdsBannerSetProperty: Invalid ad banner handle: " + adHandle );
			return MA_ADS_RES_INVALID_BANNER_HANDLE;
		}

		boolean result;
		try
		{
			result =  adBanner.setAdBannerProperty( key, value );
		}
		catch(PropertyConversionException pce)
		{
			Log.e( "MoSync", "maAdsBannerSetProperty Error while converting property value '" + value + "': " + pce.getMessage( ) );
			return MA_ADS_RES_INVALID_PROPERTY_VALUE;
		}
		catch(InvalidPropertyValueException ipve)
		{
			Log.e( "MoSync", "Error while setting ad banner property: " + ipve.getMessage( ) );
			return MA_ADS_RES_INVALID_PROPERTY_VALUE;
		}

		if( result )
		{
			return MA_ADS_RES_OK;
		}

		Log.e( "MoSync", "maAdsBannerSetProperty: Invalid property '" + key + "' on ad widget: " + adHandle );
		return MA_ADS_RES_INVALID_PROPERTY_NAME;
	}

	/**
	 * Internal function for the maAdsBannerGetProperty system call.
	 * Gets a property on the given ad banner, by accessing it from
	 * the ads table and calling its getProperty method.
	 *
	 * Note: Should only be called on the UI thread.
	 */
	public int maAdsBannerGetProperty(
		final int adHandle,
		final String key,
		final int memBuffer,
		final int memBufferSize)
	{
		AdWidget adBanner = m_AdsTable.get( adHandle );
		if( null == adBanner )
		{
			Log.e( "MoSync", "maAdsBannerGetProperty: Invalid ad banner handle: " + adHandle );
			return MA_ADS_RES_INVALID_BANNER_HANDLE;
		}

		String result = adBanner.getAdBannerProperty( key );
		if( result.equals(AdWidget.AD_INVALID_PROPERTY_NAME) )
		{
			Log.e( "MoSync", "maAdsBannerGetProperty: Invalid or empty property '" +
					key + "' on ad banner: " + adHandle );
			return MA_ADS_RES_INVALID_PROPERTY_NAME;
		}

		if( result.length( ) + 1 > memBufferSize )
		{
			Log.e( "MoSync", "maAdsBannerGetProperty: Buffer size " + memBufferSize +
					" too short to hold buffer of size: " + result.length( ) + 1 );
			return MA_ADS_RES_INVALID_STRING_BUFFER_SIZE;
		}

		byte[] byteArray = result.getBytes();

		// Write string to MoSync memory.
		MoSyncThread mosyncThread = ((MoSync) m_activity).getMoSyncThread( );
		ByteBuffer buffer = mosyncThread.getMemorySlice(memBuffer, byteArray.length + 1);
		buffer.put( byteArray );
		buffer.put( (byte)0 );

		return result.length( );
	}

	/************************ Class members ************************/

	/**
	 * The MoSync thread object.
	 */
	private MoSyncThread mMoSyncThread;

	/**
	 * Context of the main activity.
	 */
	private Activity m_activity;

	/**
	 * A table that contains a mapping between a handle and an ad, in a
	 * mosync program a handle is the only reference to a widget.
	 */
	private HandleTable<AdWidget> m_AdsTable = new HandleTable<AdWidget>();
}