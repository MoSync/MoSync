package com.mosync.nativeui.ui.widgets;

import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.ui.egl.EGLView;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * Wraps the behavior of a EGL view that displays hardware accelerated
 * graphics.
 * 
 * @author fmattias
 */
public class GLWidget extends Widget
{
	/**
	 * Constructor.
	 * 
	 * @param handle Integer handle corresponding to this instance.
	 * @param button An egl view wrapped by this widget.
	 */
	public GLWidget(int handle, EGLView eglView)
	{
		super( handle, eglView );
	}
	
	@Override
	public boolean setProperty(String property, String value) throws PropertyConversionException
	{
		if( super.setProperty(property, value) )
		{
			return true;
		}
		
		EGLView eglView = (EGLView) getView( );
		if( property.equals( Types.WIDGET_PROPERTY_BIND ) )
		{
			// Temporarily group these two together.
			eglView.bind( );
			eglView.enterRender( );
		}
		else if( property.equals( Types.WIDGET_PROPERTY_INVALIDATE ) )
		{
			eglView.finishRender( );
		}
		else
		{
			return false;
		}
		
		return true;
	}
}
