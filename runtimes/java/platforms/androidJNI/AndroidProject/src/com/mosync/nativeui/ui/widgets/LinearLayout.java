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

import android.view.ViewGroup;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.HorizontalAlignment;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;
import com.mosync.nativeui.util.properties.VerticalAlignment;

/**
 * A class for a linear layout that is either horizontal
 * or vertical.
 *
 * @author fmattias
 */
public class LinearLayout extends Layout
{

	/**
	 * Constructor.
	 *
	 * @param handle Integer handle corresponding to this instance.
	 * @param linearLayout A linear layout wrapped by this widget.
	 */
	public LinearLayout(int handle, android.widget.LinearLayout linearLayout)
	{
		super( handle, linearLayout );
	}

	/**
	 * @see Layout.updateLayoutParams.
	 */
	@Override
	public ViewGroup.LayoutParams createNativeLayoutParams(LayoutParams mosyncLayoutParams)
	{
		return new android.widget.LinearLayout.LayoutParams( mosyncLayoutParams.getWidth( ) , mosyncLayoutParams.getHeight( ) );
	}

	/**
	 * @see updateLayoutParamsForChild.
	 */
	@Override
	public void updateLayoutParamsForChild(Widget child)
	{
		android.widget.LinearLayout linearLayout = (android.widget.LinearLayout) getView( );

		/**
		 * Widgets automatically fill the remaining space when set to FILL_PARENT(-1),
		 * or share equally the parent space.
		 */
		LayoutParams childLayoutParams = child.getLayoutParams( );
		if( linearLayout.getOrientation( ) == android.widget.LinearLayout.VERTICAL )
		{
			if( childLayoutParams.getHeight( ) == -1 )
			{
				childLayoutParams.height = 0;
				childLayoutParams.weight = 1.0f;
			}
			else if ( childLayoutParams.getHeight( ) != 0 )
			{
				childLayoutParams.weight = 0.0f;
			}
		}
		else
		{
			if( childLayoutParams.getWidth( ) == -1 )
			{
				childLayoutParams.width = 0;
				childLayoutParams.weight = 1.0f;
			}
			else if ( childLayoutParams.getWidth( ) != 0 )
			{
				childLayoutParams.weight = 0.0f;
			}
		}

		super.updateLayoutParamsForChild( child );
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		android.widget.LinearLayout layout = (android.widget.LinearLayout) getView( );

		if( property.equals( IX_WIDGET.MAW_HORIZONTAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT ) ||
				property.equals( IX_WIDGET.MAW_VERTICAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT ) )
		{
			layout.setHorizontalGravity( HorizontalAlignment.convert( value ) );
		}
		else if( property.equals( IX_WIDGET.MAW_HORIZONTAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT) ||
				 property.equals( IX_WIDGET.MAW_VERTICAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT ) )
		{
			layout.setVerticalGravity( VerticalAlignment.convert( value ) );
		}
		else if ( property.equals( IX_WIDGET.MAW_HORIZONTAL_LAYOUT_PADDING_LEFT ) )
		{
			layout.setPadding(IntConverter.convert(value), layout.getPaddingTop(), layout.getPaddingRight(), layout.getPaddingBottom());
		}
		else if (property.equals( IX_WIDGET.MAW_HORIZONTAL_LAYOUT_PADDING_TOP ))
		{
			layout.setPadding( layout.getPaddingLeft(), IntConverter.convert(value), layout.getPaddingRight(), layout.getPaddingBottom());
		}
		else if (property.equals( IX_WIDGET.MAW_HORIZONTAL_LAYOUT_PADDING_RIGHT ))
		{
			layout.setPadding( layout.getPaddingLeft(), layout.getPaddingTop(), IntConverter.convert(value), layout.getPaddingBottom());
		}
		else if (property.equals( IX_WIDGET.MAW_HORIZONTAL_LAYOUT_PADDING_BOTTOM))
		{
			layout.setPadding( layout.getPaddingLeft(), layout.getPaddingTop(), layout.getPaddingRight(), IntConverter.convert(value));
		}
		else if( super.setProperty( property, value ) )
		{
			return true;
		}
		else
		{
			return false;
		}
		return true;
	}
}
