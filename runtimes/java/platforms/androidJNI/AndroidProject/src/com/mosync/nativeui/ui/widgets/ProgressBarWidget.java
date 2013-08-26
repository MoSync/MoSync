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
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * A progress bar is a visual indicator of progress in some operation.
 * Displays a bar to the user representing how far the operation has progressed.
 *
 * @author emma
 */
public class ProgressBarWidget extends Widget
{
	/**
	 * Constructor.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param progressbar A progressbar wrapped by this widget.
	 */
	public ProgressBarWidget(int handle, ProgressBar progressBar)
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

		if ( IntConverter.convert( value ) < 0 )
		{
			throw new InvalidPropertyValueException(property, value);
		}

		if( property.equals( IX_WIDGET.MAW_PROGRESS_BAR_MAX ) )
		{
			progressBar.setMax(IntConverter.convert( value ));
		}
		else if( property.equals( IX_WIDGET.MAW_PROGRESS_BAR_PROGRESS ))
		{
			progressBar.setProgress(IntConverter.convert(value));
		}
		else if( property.equals(IX_WIDGET.MAW_PROGRESS_BAR_INCREMENT_PROGRESS ))
		{
			progressBar.incrementProgressBy(IntConverter.convert( value ));
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
		ProgressBar progressBar = (ProgressBar) getView( );

		if( property.equals( IX_WIDGET.MAW_PROGRESS_BAR_MAX ) )
		{
			return Integer.toString( progressBar.getMax() );
		}
		else if( property.equals( IX_WIDGET.MAW_PROGRESS_BAR_PROGRESS ))
		{
			return Integer.toString( progressBar.getProgress() );
		}
		else
		{
			return super.getProperty( property );
		}
	}
}
