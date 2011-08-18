/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

package com.mosync.nativeui.ui.egl;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

/**
 * Factory for creating and destroying a context from a config and a display.
 *
 * @author fmattias
 */
public class EGLContextFactory
{
	/**
	 * Creates a context from the given EGL config and display.
	 *
	 * @param egl The EGL instance.
	 * @param config The EGL context configuration.
	 * @param display The display to which the context is bound to.
	 *
	 * @return The created context.
	 */
	public static EGLContext createContext(EGL10 egl, EGLConfig config, EGLDisplay display)
	{
		return egl.eglCreateContext( display, config, EGL10.EGL_NO_CONTEXT, null );
	}

	/**
	 * Destroys the given context and detaches it from the display.
	 *
	 * @param egl The EGL instance.
	 * @param display The display to which the context is bound to.
	 * @param context The context to destroy.
	 */
	public static void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context)
	{
		egl.eglDestroyContext( display, context );
	}
}
