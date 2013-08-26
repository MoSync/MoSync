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
