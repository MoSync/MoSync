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

package com.mosync.nativeui.ui.widgets;

import android.widget.RatingBar;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.FloatConverter;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A RatingBar is an extension of SeekBar and ProgressBar that shows a rating in stars.
 * @author emma
 */
public class RatingBarWidget extends Widget
{
	/**
	 * Constructor.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param ratingBar A rating bar wrapped by this widget.
	 */
	public RatingBarWidget(int handle, RatingBar ratingBar)
	{
		super( handle, ratingBar );
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty(property, value) )
		{
			return true;
		}

		RatingBar ratingBar = (RatingBar) getView( );

		if( property.equals( IX_WIDGET.MAW_RATING_BAR_NUM_STAR ) )
		{
			int stars = IntConverter.convert(value);
			if ( stars < 0 )
			{
				throw new InvalidPropertyValueException(property, value);
			}
			ratingBar.setNumStars(stars);
		}
		else if( property.equals( IX_WIDGET.MAW_RATING_BAR_RATING ) )
		{
			Float rating = FloatConverter.convert(value);
			if ( rating < 0 )
			{
				throw new InvalidPropertyValueException(property, value);
			}
			if ( rating % ratingBar.getStepSize() != 0 )
			{
				throw new InvalidPropertyValueException(property, value);
			}
			ratingBar.setRating(rating);
		}
		else if( property.equals( IX_WIDGET.MAW_RATING_BAR_GRANULARITY ) )
		{
			Float granularity = FloatConverter.convert(value);
			if ( granularity < 0 )
			{
				throw new InvalidPropertyValueException(property, value);
			}
			ratingBar.setStepSize(granularity);
		}
		else
		{
			return false;
		}
		return true;
	}

	/**
	 * @see Widget.getProperty.
	 */
	@Override
	public String getProperty(String property)
	{
		RatingBar ratingBar = (RatingBar) getView( );
		if( property.equals( IX_WIDGET.MAW_RATING_BAR_NUM_STAR ) )
		{
			return Integer.toString( ratingBar.getNumStars() );
		}
		else if( property.equals( IX_WIDGET.MAW_RATING_BAR_RATING ) )
		{
			return Float.toString( ratingBar.getRating() );
		}
		else if( property.equals( IX_WIDGET.MAW_RATING_BAR_GRANULARITY ) )
		{
			return Float.toString( ratingBar.getStepSize() );
		}
		else
		{
			return super.getProperty( property );
		}
	}
}
