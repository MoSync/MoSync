package com.mosync.nativeui.ui.widgets;

import android.view.ViewGroup;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.properties.HorizontalAlignment;
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
	
	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException
	{
		if( super.setProperty( property, value ) )
		{
			return true;
		}
		
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
		else
		{
			return false;
		}
		
		return true;
	}
}
