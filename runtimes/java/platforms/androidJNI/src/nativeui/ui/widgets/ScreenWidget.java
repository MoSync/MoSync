package com.mosync.nativeui.ui.widgets;

import com.mosync.nativeui.core.NativeUI;
import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.util.LayoutParamsSetter;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.view.ViewGroup;

/**
 * A screen widget that represents the root of a widget
 * tree.
 * 
 * Note: For now a screen widget is just a wrapper to a view
 * like all other widgets, but by default it fills the whole
 * screen.
 * 
 * @author fmattias
 */
public class ScreenWidget extends Layout
{
	/**
	 * Title of this screen.
	 */
	private String m_title = "";
	
	/**
	 * Icon of this screen.
	 */
	private Drawable m_icon = null;
	
	/**
	 * Constructor
	 * 
	 * @param handle handle Integer handle corresponding to this instance.
	 * @param view A screen wrapped by this widget.
	 */
	public ScreenWidget(int handle, ViewGroup view)
	{
		super( handle, view );
		
		// Screen fills the whole screen.
		getLayoutParams( ).width = ViewGroup.LayoutParams.FILL_PARENT;
		getLayoutParams( ).height = ViewGroup.LayoutParams.FILL_PARENT;
		
		ViewGroup.LayoutParams nativeLayoutParams = createNativeLayoutParams( getLayoutParams( ) );
		LayoutParamsSetter.setPossibleParams( getLayoutParams( ), nativeLayoutParams );
		getView( ).setLayoutParams( nativeLayoutParams );
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException
	{
		if( super.setProperty( property, value ) )
		{
			return true;
		}
		
		if( property.equals( Types.WIDGET_PROPERTY_TITLE ) )
		{
			m_title = value;
		}
		else if( property.equals( Types.WIDGET_PROPERTY_ICON ) )
		{
			int imageHandle = IntConverter.convert( value );
			Bitmap icon = NativeUI.getBitmap( imageHandle );
			if( icon != null )
			{
				m_icon = new BitmapDrawable( NativeUI.getBitmap( imageHandle ) );
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
		
		return true;
	}
	
	/**
	 * Returns the title of this screen.
	 * 
	 * @return the title of this screen.
	 */
	public String getTitle()
	{
		return m_title;
	}
	
	/**
	 * Returns the icon of this screen.
	 * 
	 * @return the icon of this screen.
	 */
	public Drawable getIcon()
	{
		return m_icon;
	}
	
	
}