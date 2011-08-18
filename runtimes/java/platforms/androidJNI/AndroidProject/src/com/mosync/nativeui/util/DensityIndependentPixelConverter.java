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
