package com.mosync.nativeui.ui.widgets;

import com.mosync.nativeui.core.Types;

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
	 * The handle corresponding to this widget.
	 */
	private int m_handle;
	
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
	 * 
	 * @param handle Integer handle corresponding to this instance.
	 * @param view A view wrapped by this widget
	 */
	public Widget(int handle, View view)
	{
		m_handle = handle;
		m_view = view;
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
	public boolean setProperty(int property, String value)
	{		
		if( property == Types.WIDGET_PROPERTY_WIDTH )
		{
			getView( ).getLayoutParams( ).width = Integer.parseInt( value );
			return true;
		}
		else if( property == Types.WIDGET_PROPERTY_HEIGHT )
		{
			getView( ).getLayoutParams( ).height = Integer.parseInt( value );
			return true;
		}
		else
		{
			return false;
		}
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
		return m_handle;
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
}
