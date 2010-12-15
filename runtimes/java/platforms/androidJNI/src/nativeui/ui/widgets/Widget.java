package com.mosync.nativeui.ui.widgets;

import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.properties.HorizontalAlignment;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;
import com.mosync.nativeui.util.properties.VerticalAlignment;

import android.graphics.Color;
import android.view.View;

/**
 * A class that wraps a native view with functionality
 * used by the syscalls. The reason for wrapping them
 * is to contain the custom logic imposed by certain
 * views in a subclass of Widget, instead of having it
 * scattered in the syscalls.
 * 
 * @author fmattias
 */
public class Widget
{
	/**
	 * The view that is wrapped by this widget.
	 */
	private View m_view;
	
	/**
	 * The parent of this widget, is null when this widget
	 * has not parent.
	 */
	private Widget m_parent = null;
	
	/**
	 * Default layout params.
	 */
	private LayoutParams m_layoutParams = new LayoutParams();
	
	/**
	 * Constructor
	 * 
	 * Note: The id of the view is always equal to its handle.
	 * 
	 * @param handle Integer handle corresponding to this instance.
	 * @param view A view wrapped by this widget.
	 */
	public Widget(int handle, View view)
	{
		m_view = view;
		m_view.setId( handle );
	}
	
	/**
	 * Sets the property on the given widget.
	 * 
	 * TODO: More sensible implementation.
	 * 
	 * @param property The property of the widget that should be set,
	 *                 one of
	 * @param value The value of the property, this will be converted
	 *              to the appropriate type.
	 * @return true if the property was set, false otherwise.
	 */
	public boolean setProperty(String property, String value) throws PropertyConversionException
	{
		LayoutParams layoutParams = getLayoutParams( );
		if( property.equals( Types.WIDGET_PROPERTY_WIDTH ) )
		{
			layoutParams.width = IntConverter.convert( value );
		}
		else if( property.equals( Types.WIDGET_PROPERTY_HEIGHT ) )
		{
			layoutParams.height = IntConverter.convert( value );
		}
		else if( property.equals( Types.WIDGET_PROPERTY_HORIZONTAL_ALIGNMENT ) )
		{
			layoutParams.horizontalAlignment =  HorizontalAlignment.convert( value );
		}
		else if( property.equals( Types.WIDGET_PROPERTY_VERTICAL_ALIGNMENT ) )
		{
			layoutParams.verticalAlignment = VerticalAlignment.convert( value );
		}
		else if( property.equals( Types.WIDGET_PROPERTY_BACKGROUND_COLOR ) )
		{
			getView( ).setBackgroundColor( Color.parseColor( "#" + value ) );
		}
		else
		{
			return false;
		}
		
		return true;
	}
	
	/**
	 * Sets the parent of this widget.
	 * 
	 * @param parent The new parent of this widget.
	 */
	public void setParent(Widget parent)
	{
		m_parent = parent;
	}
	
	/**
	 * Returns the parent of this widget.
	 * 
	 * @return Returns the parent of this widget, or
	 *         null if it does not have a parent.
	 */
	public Widget getParent()
	{
		return m_parent;
	}
	
	/**
	 * Returns the view associated with this widget.
	 * 
	 * @return The view associated with this widget.
	 */
	public View getView()
	{
		return m_view;
	}
	
	/**
	 * Returns the handle of this widget.
	 * 
	 * @return The handle associated with this widget.
	 */
	public int getHandle()
	{
		return m_view.getId( );
	}
	
	/**
	 * Returns true if the given widget can layout other widgets.
	 * 
	 * @return
	 */
	public boolean isLayout()
	{
		return false;
	}
	
	/**
	 * Returns the layout parameters.
	 * 
	 * @return The layout parameters, these are never null.
	 */
	public LayoutParams getLayoutParams()
	{
		assert( m_layoutParams != null );
		return m_layoutParams;
	}
}
