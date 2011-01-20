package com.mosync.nativeui.util;

import android.app.Activity;
import android.util.DisplayMetrics;

/**
 * Converts Density-independent pixels to actual pixels.
 * @author Mikael
 */
public class DensityIndependentPixelConverter 
{
	/**
	 * The activity for which we do the pixel conversion.
	 */
	Activity mActivity;
	
	/**
	 * Constructor.
	 * @param activity The activity for which we want 
	 * to do the pixel conversion.
	 */
	public DensityIndependentPixelConverter(Activity activity)
	{
		mActivity = activity;
	}
	
	/**
	 * Converts Density-independent pixels to actual pixels.
	 * @param densityIndependentPixels Density-independent pixel value.
	 * @return The corresponding display pixel value.
	 */
	public int toDisplayPixels(float densityIndependentPixels)
	{
		// Get display metrics.
		DisplayMetrics metrics = new DisplayMetrics();
		mActivity.getWindowManager().getDefaultDisplay().getMetrics(metrics);
		
		// Return the actual pixel value calculated from the density.
		return (int) (densityIndependentPixels * metrics.density);
	}
}
