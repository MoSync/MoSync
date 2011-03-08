package com.mosync.nativeui.ui.widgets;

import android.widget.TextView;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.properties.ColorConverter;
import com.mosync.nativeui.util.properties.FloatConverter;
import com.mosync.nativeui.util.properties.HorizontalAlignment;
import com.mosync.nativeui.util.properties.PropertyConversionException;
import com.mosync.nativeui.util.properties.VerticalAlignment;

/**
 * This class represents a static text label, i.e.
 * not interactive.
 * 
 * @author fmattias
 */
public class LabelWidget extends Widget
{
	/**
	 * Constructor.
	 * 
	 * @param handle handle Integer handle corresponding to this instance.
	 * @param view A text view wrapped by this widget.
	 */
	public LabelWidget(int handle, TextView view)
	{
		super( handle, view );
		view.setGravity( 0 );
	}

	/**
	 * @see Widget.setProperty.
	 */
	@Override
	public boolean setProperty(String property, String value) throws PropertyConversionException
	{
		if( super.setProperty(property, value) )
		{
			return true;
		}
		
		TextView textView = (TextView) getView( );
		if( property.equals( IX_WIDGET.MAW_LABEL_TEXT ) )
		{
			textView.setText( value );
		}
		else if( property.equals( IX_WIDGET.MAW_EDIT_BOX_PLACEHOLDER ) )
		{
			textView.setHint( value );
		}
		else if( property.equals( IX_WIDGET.MAW_LABEL_FONT_COLOR ) )
		{
			textView.setTextColor( ColorConverter.convert( value ) );
		}
		else if( property.equals( IX_WIDGET.MAW_LABEL_FONT_SIZE ) )
		{
			textView.setTextSize( FloatConverter.convert( value ) );
		}
		else if( property.equals( Types.WIDGET_PROPERTY_TEXT_HORIZONTAL_ALIGNMENT ) )
		{
			int currentGravity = HorizontalAlignment.clearHorizontalAlignment( textView.getGravity( ) );
			textView.setGravity( currentGravity | HorizontalAlignment.convert( value ) );
		}
		else if( property.equals( Types.WIDGET_PROPERTY_TEXT_VERTICAL_ALIGNMENT ) )
		{
			int currentGravity = VerticalAlignment.clearVerticalAlignment( textView.getGravity( ) );
			textView.setGravity( currentGravity | VerticalAlignment.convert( value ) );
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
		TextView textView = (TextView) getView( );
		if( property.equals( IX_WIDGET.MAW_LABEL_TEXT ) )
		{
			if( textView.getText( ) != null && textView.getText( ).length( ) > 0 )
			{
				return textView.getText( ).toString( );
			}
			else if( textView.getHint( ) != null && textView.getText( ).length( ) > 0 )
			{
				return textView.getHint( ).toString( );
			}
			else
			{
				return "";
			}
		}
		else
		{
			return super.getProperty( property );
		}
	}
}
