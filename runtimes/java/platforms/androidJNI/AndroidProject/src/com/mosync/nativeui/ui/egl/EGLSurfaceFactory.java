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
