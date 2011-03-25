package com.mosync.nativeui.ui.widgets;

import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.view.View;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.core.NativeUI;
import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.ColorConverter;
import com.mosync.nativeui.util.properties.FloatConverter;
import com.mosync.nativeui.util.properties.HorizontalAlignment;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;
import com.mosync.nativeui.util.properties.VerticalAlignment;

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
	private LayoutParams m_layoutParams = new LayoutParams( );
	
	/**
	 * The alpha is stored here to enable us to the alpha before
	 * a background drawable is set. This means that as soon as
	 * the background is changed, the alpha must also be set.
	 */
	private int m_alpha = 0xff;
	
	/**
	 * Constructor.
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
	 * @param property The property of the widget that should be set.
	 * @param value The value of the property, this will be converted
	 *              to the appropriate type.
	 * @return true if the property was set, false otherwise.
	 * 
	 * @throws PropertyConversionException If a value could not be converted
	 *                                     to a value suitable for the given property.
	 * @throws InvalidPropertyValueException If the converted value was out of range
	 *                                       or in any other sense invalid.
	 */
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		LayoutParams layoutParams = getLayoutParams( );
		if( property.equals( IX_WIDGET.MAW_WIDGET_WIDTH ) )
		{
			layoutParams.width = IntConverter.convert( value );
		}
		else if( property.equals( IX_WIDGET.MAW_WIDGET_HEIGHT ) )
		{
			layoutParams.height = IntConverter.convert( value );
		}
		else if( property.equals( Types.WIDGET_PROPERTY_MARGIN_LEFT ) ||
				 property.equals( IX_WIDGET.MAW_WIDGET_LEFT ) )
		{
			layoutParams.marginLeft = IntConverter.convert( value );
		}
		else if( property.equals( Types.WIDGET_PROPERTY_MARGIN_TOP ) || 
				 property.equals( IX_WIDGET.MAW_WIDGET_TOP ) )
		{
			layoutParams.marginTop = IntConverter.convert( value );
		}
		else if( property.equals( Types.WIDGET_PROPERTY_MARGIN_RIGHT ) )
		{
			layoutParams.marginRight = IntConverter.convert( value );
		}
		else if( property.equals( Types.WIDGET_PROPERTY_MARGIN_BOTTOM ) )
		{
			layoutParams.marginBottom = IntConverter.convert( value );
		}
		else if( property.equals( Types.WIDGET_PROPERTY_HORIZONTAL_ALIGNMENT ) )
		{
			layoutParams.horizontalAlignment =  HorizontalAlignment.convert( value );
		}
		else if( property.equals( Types.WIDGET_PROPERTY_VERTICAL_ALIGNMENT ) )
		{
			layoutParams.verticalAlignment = VerticalAlignment.convert( value );
		}
		else if( property.equals( IX_WIDGET.MAW_WIDGET_BACKGROUND_COLOR ) )
		{
			getView( ).setBackgroundColor( ColorConverter.convert( value ) );
			updateAlpha( m_alpha );
		}
		else if( property.equals( IX_WIDGET.MAW_BUTTON_BACKGROUND_IMAGE ) )
		{
			int imageHandle = IntConverter.convert( value );
			Bitmap background = NativeUI.getBitmap( imageHandle );
			if( background != null )
			{
				getView( ).setBackgroundDrawable( new BitmapDrawable( background ) );
				updateAlpha( m_alpha );
			}
			else
			{
				return false;
			}
		}
		else if( property.equals( IX_WIDGET.MAW_WIDGET_ALPHA ) )
		{
			float alpha = FloatConverter.convert( value );
			if( alpha > 1.0f || alpha < 0.0f )
			{
				throw new InvalidPropertyValueException( value, property );
			}
			
			int intAlpha = (int) (alpha * 255.0f);
			m_alpha = intAlpha;
			updateAlpha( m_alpha );
		}
		else if( property.equals( IX_WIDGET.MAW_WIDGET_VISIBLE ) )
		{
			boolean shouldBeVisible = BooleanConverter.convert( value );
			if( shouldBeVisible )
			{
				getView( ).setVisibility( View.VISIBLE );
			}
			else
			{
				getView( ).setVisibility( View.INVISIBLE );
			}
		}
		else
		{
			return false;
		}
		
		// Notify parent that the layout params has changed.
		Layout parent = (Layout) getParent( );
		if( getParent( ) != null )
		{
			parent.updateLayoutParamsForChild( this );
		}
		
		return true;
	}
	
	/**
	 * Returns the property of the wrapped widget.
	 * 
	 * @param property
	 * @return the property of the wrapped widget. If no property is found, an
	 *         empty string is returned.
	 */
	public String getProperty(String property)
	{
		if( property.equals( IX_WIDGET.MAW_WIDGET_WIDTH ) )
		{
			return Integer.toString( getView( ).getWidth( ) -
					getLayoutParams( ).marginLeft - getLayoutParams( ).marginRight );
		}
		else if( property.equals( IX_WIDGET.MAW_WIDGET_HEIGHT ) )
		{
			return Integer.toString( getView( ).getHeight( ) -
					getLayoutParams( ).marginTop - getLayoutParams( ).marginBottom );
		}
		else if( property.equals( IX_WIDGET.MAW_WIDGET_LEFT ) )
		{
			return Integer.toString( getView( ).getLeft( ) );
		}
		else if( property.equals( IX_WIDGET.MAW_WIDGET_TOP ) )
		{
			return Integer.toString( getView( ).getTop( ) );
		}
		else
		{
			return "";
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
	
	/**
	 * Updates the alpha of the background of the underlying view.
	 * 
	 * @param alpha The new alpha of the underlying view.
	 */
	public void updateAlpha(int alpha)
	{		
		Drawable background = getView( ).getBackground( );
		if( background == null )
		{
			return;
		}
		background.setAlpha( alpha & 0xff );
	}
}
