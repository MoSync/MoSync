package com.mosync.nativeui.ui.ads;

import android.util.Log;
import android.view.View;

import com.google.ads.Ad;
import com.google.ads.AdRequest;
import com.google.ads.AdListener;
import com.google.ads.AdRequest.ErrorCode;
import com.google.ads.AdView;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.ColorConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_EVENT_FAILED;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_EVENT_LOADED;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_EVENT_ON_DISMISS;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_EVENT_ON_LEAVE_APPLICATION;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_ERROR_INTERNAL;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_ERROR_INVALID_REQUEST;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_ERROR_NETWORK;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_ERROR_NO_FILL;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_HEIGHT;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_WIDTH;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_IS_READY;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_COLOR_BG;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_COLOR_BG_TOP;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_COLOR_BORDER;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_COLOR_LINK;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_COLOR_TEXT;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_COLOR_URL;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_REQUEST_CONTENT;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_ENABLED;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_VISIBLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_ADS_TEST_DEVICE;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_ADS_BANNER;

/**
 * The Ad is like a widget, but it has it's specific properties only,
 * accessed via setAdBannerProperty/getAdBannerProperty calls.
 * @author emma tresanszki
 */
public class AdWidget extends Widget implements AdListener
{
	/**
	 * Constructor.
	 *
	 * Note: The id of the view is always equal to its handle.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param view A view wrapped by this widget.
	 */
	public AdWidget(int handle, AdView view, MoSyncThread mosyncThread)
	{
		// Add the AdView to the view hierarchy. The view will have no size
		// until the ad is loaded.
		super ( handle, view );
		mHandle = handle;
		mThread = mosyncThread;
		// Create an ad request.
		mAdRequest = new AdRequest();
	}

	/**
	 * Called when the full-screen Activity presented with onPresentScreen
	 * has been dismissed and control is returning to your app.
	 */
	@Override
	public void onDismissScreen(Ad arg0) {
		postAdsBannerEvent(new AdsEvent(MA_ADS_EVENT_ON_DISMISS, mHandle, 0));
	}

	/**
	 * Sent when loadAd has failed, typically because of network failure,
	 * an application configuration error, or a lack of ad inventory.
	 */
	@Override
	public void onFailedToReceiveAd(Ad arg0, ErrorCode err) {
		int errCode = 0;
		switch (err)
		{
		case INTERNAL_ERROR:
			errCode = MA_ADS_ERROR_INTERNAL;
			break;
		case INVALID_REQUEST:
			errCode = MA_ADS_ERROR_INVALID_REQUEST;
			break;
		case NETWORK_ERROR:
			errCode = MA_ADS_ERROR_NETWORK;
			break;
		case NO_FILL:
			errCode = MA_ADS_ERROR_NO_FILL;
			break;
		}
		postAdsBannerEvent(new AdsEvent(MA_ADS_EVENT_FAILED, mHandle, errCode));
	}

	/**
	 * Called when an Ad touch will launch a new application.
	 */
	@Override
	public void onLeaveApplication(Ad arg0) {
		postAdsBannerEvent(new AdsEvent(MA_ADS_EVENT_ON_LEAVE_APPLICATION, mHandle, 0));
	}

	@Override
	public void onPresentScreen(Ad arg0) {
	}

	/**
	 * Sent when AdView.loadAd has succeeded.
	 */
	@Override
	public void onReceiveAd(Ad arg0) {
		postAdsBannerEvent(new AdsEvent(MA_ADS_EVENT_LOADED, mHandle, 0));
	}

	/**
	 * Post a message to the MoSync event queue.
	 */
	private void postAdsBannerEvent(AdsEvent event)
	{
		int[] nativeEvent = new int[] {
				EVENT_TYPE_ADS_BANNER,
				event.mBannerEventType,
				event.mBannerHandle,
				event.mErrCode
			};
		mThread.postEvent(nativeEvent);
		Log.d("@@ MoSync ADS event ", event.toString());
	}

	/**
	 * Destroys the AdView.
	 * The AdView should no longer be used after this method is called.
	 */
	public void destroyAd()
	{
		AdView adView = (AdView) getView();
		adView.destroy();
	}

	/**
	 * Sets a specific ad banner property.
	 * @param property
	 * @param value
	 * @return
	 * @throws PropertyConversionException
	 * @throws InvalidPropertyValueException
	 */
	public boolean setAdBannerProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		AdView adView = (AdView) getView();
		if ( property.equals( MA_ADS_TEST_DEVICE ))
		{
			if ( value.equals("TEST_EMULATOR") )
			{
				mAdRequest.addTestDevice(AdRequest.TEST_EMULATOR);
			}
			else
			{
				mAdRequest.addTestDevice(value);
			}
		}
		else if ( property.equals( MA_ADS_REQUEST_CONTENT ) )
		{
			if ( BooleanConverter.convert(value) )
			{
				// Start loading the ad in the background.
				adView.loadAd(mAdRequest);
			}
			else
			{
				adView.stopLoading();
			}
		}
		else if ( property.equals( MA_ADS_ENABLED ) )
		{
			adView.setEnabled(BooleanConverter.convert(value));
		}
		else if ( property.equals( MA_ADS_VISIBLE ) )
		{
			boolean shouldBeVisible = BooleanConverter.convert( value );
			if( shouldBeVisible )
			{
				// No need to show it if it was visible before.
				if ( getView().getVisibility() != View.VISIBLE )
				{
					getView( ).setVisibility( View.VISIBLE );
				}
			}
			else
			{
				// Hide it, but still take up the space.
				getView( ).setVisibility( View.INVISIBLE );
			}
		}
		else if ( property.equals( MA_ADS_COLOR_BG ) )
		{
			// Throws PropertyConversionException.
			ColorConverter.convert(value);
			// Set the color in the format: RRGGBB.
			mAdRequest.addExtra("color_bg", value.substring(2));
		}
		else if ( property.equals( MA_ADS_COLOR_BG_TOP ) )
		{
			// Throws PropertyConversionException.
			ColorConverter.convert(value);
			mAdRequest.addExtra("color_bg_top", value.substring(2));
		}
		else if ( property.equals( MA_ADS_COLOR_BORDER ) )
		{
			// Throws PropertyConversionException.
			ColorConverter.convert(value);
			mAdRequest.addExtra("color_border", value.substring(2));
		}
		else if ( property.equals( MA_ADS_COLOR_LINK ) )
		{
			// Throws PropertyConversionException.
			ColorConverter.convert(value);
			mAdRequest.addExtra("color_link", value.substring(2));
		}
		else if ( property.equals( MA_ADS_COLOR_TEXT ) )
		{
			// Throws PropertyConversionException.
			ColorConverter.convert(value);
			mAdRequest.addExtra("color_text", value.substring(2));
		}
		else if ( property.equals( MA_ADS_COLOR_URL ) )
		{
			// Throws PropertyConversionException.
			ColorConverter.convert(value);
			mAdRequest.addExtra("color_url", value.substring(2));
		}
		else
		{
			return false;
		}
		return true;
	}

	/**
	 * Gets a specific ad property.
	 * @param property The specific property name.
	 */
	public String getAdBannerProperty(String property)
	{
		AdView adView = (AdView) getView();
		if( property.equals( MA_ADS_IS_READY ))
		{
			return Boolean.toString( adView.isReady() );
		}
		if( property.equals( MA_ADS_WIDTH ) )
		{
			return Integer.toString( adView.getWidth() );
		}
		else if ( property.equals( MA_ADS_HEIGHT ) )
		{
			return Integer.toString( adView.getHeight() );
		}
		else if ( property.equals( MA_ADS_ENABLED ) )
		{
			return Boolean.toString(adView.isEnabled());
		}
		else if ( property.equals( MA_ADS_VISIBLE ) )
		{
			if ( adView.getVisibility() == View.VISIBLE )
				return "true";
			else
				return "false";
		}
		return AD_INVALID_PROPERTY_NAME;
	}

	/**
	 * No widget specific properties.
	 */
	@Override
	public boolean setProperty(String property, String value)
		throws PropertyConversionException, InvalidPropertyValueException
	{
		return false;
	}

	@Override
	public String getProperty(String property)
	{
		return "";
	}

	/************************ Class members ************************/

	/**
	 * Error string for unavailable properties.
	 */
	public final static String AD_INVALID_PROPERTY_NAME = "AdInvalidPropertyName";

	/**
	 * Request for ads. Can be configured, stopped.
	 */
	private AdRequest mAdRequest;

	/**
	 * The handle of the ad view.
	 */
	private int mHandle;

	/**
	 * The MoSync thread object.
	 */
	private MoSyncThread mThread;
}