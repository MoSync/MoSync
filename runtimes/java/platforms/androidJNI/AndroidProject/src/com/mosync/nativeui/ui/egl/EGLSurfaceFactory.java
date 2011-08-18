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
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;

import android.view.SurfaceHolder;

/**
 * Wrapper class for EGL to create surfaces with a given config
 * without attributes.
 *
 * @author fmattias
 */
public class EGLSurfaceFactory
{
	/**
	 * Creates a surface with the given config bound to the area given
	 * to the surface holder.
	 *
	 * @param egl The EGL instance.
	 * @param config The EGL context configuration.
	 * @param display The display which EGL has a connection to.
	 * @param holder The area in the screen which the EGL surface will be bound to.
	 *
	 * @return The created surface.
	 */
	public static EGLSurface createSurface(EGL10 egl, EGLConfig config, EGLDisplay display, SurfaceHolder holder)
	{
		return egl.eglCreateWindowSurface( display, config, holder, null );
	}

	/**
	 * Destroys the EGL surface.
	 *
	 * @param egl The EGL instance.
	 * @param display The display to which the surface is bound to.
	 * @param surface The surface to destroy.
	 */
	public static void destroySurface(EGL10 egl, EGLDisplay display, EGLSurface surface)
	{
		egl.eglDestroySurface( display, surface );
	}
}
