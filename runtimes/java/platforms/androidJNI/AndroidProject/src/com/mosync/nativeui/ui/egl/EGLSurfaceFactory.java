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
