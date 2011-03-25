package com.mosync.nativeui.ui.widgets;

import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.ui.egl.EGLView;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * Wraps the behavior of a EGL view that displays hardware accelerated
 * graphics.
 * 
 * @author fmattias
 */
public class GLWidget extends FrameLayout
{
	private EGLView m_eglView = null;
	
	/**
	 * Constructor.
	 * 
	 * @param handle Integer handle corresponding to this instance.
	 * @param eglFrame A frame that frames the EGL View so that other
	 *                 widgets can lie on top of it.
	 * @param eglView An egl view wrapped by this widget.
	 */
	public GLWidget(int handle, android.widget.FrameLayout eglFrame, EGLView eglView)
	{
		super( handle, eglFrame );
		m_eglView = eglView;
	}
	
	@Override
	public boolean setProperty(String property, String value) throws PropertyConversionException, InvalidPropertyValueException
	{
		if( super.setProperty(property, value) )
		{
			return true;
		}
		
		if( property.equals( IX_WIDGET.MAW_GL_VIEW_BIND ) )
		{
			// Temporarily group these two together.
			m_eglView.bind( );
			m_eglView.enterRender( );
		}
		else if( property.equals( IX_WIDGET.MAW_GL_VIEW_INVALIDATE ) )
		{
			m_eglView.finishRender( );
		}
		else
		{
			return false;
		}
		
		return true;
	}
}
