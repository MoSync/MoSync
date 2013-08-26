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

import android.widget.ProgressBar;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * An activity indicator is a visual indicator of progress in some operation.
 * It shows a cyclic animation without an indication of progress.
 * It is used when the length of the task is unknown.
 *
 * @author emma
 */
public class ActivityIndicatorWidget extends Widget
{
	/**
	 * Constructor.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param progressbar A progress bar wrapped by this widget.
	 */
	public ActivityIndicatorWidget(int handle, ProgressBar progressBar)
	{
		super( handle, progressBar );
	}

	/**
	 * @see Widget.setProperty.
	 */
	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty(property, value) )
		{
			return true;
		}

		ProgressBar progressBar = (ProgressBar) getView( );

		if( property.equals( IX_WIDGET.MAW_ACTIVITY_INDICATOR_IN_PROGRESS ) )
		{
			if ( BooleanConverter.convert(value) )
			{
				progressBar.setVisibility(ProgressBar.VISIBLE);
			}
			else
			{
				progressBar.setVisibility(ProgressBar.INVISIBLE);
			}
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
		return super.getProperty( property );
	}
}
