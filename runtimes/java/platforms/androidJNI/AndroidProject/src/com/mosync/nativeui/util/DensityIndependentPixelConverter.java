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
