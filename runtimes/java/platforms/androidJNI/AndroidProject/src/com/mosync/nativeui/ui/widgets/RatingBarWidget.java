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
