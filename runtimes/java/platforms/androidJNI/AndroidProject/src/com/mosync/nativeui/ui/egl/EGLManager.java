package com.mosync.nativeui.ui.egl;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

/**
 * Singleton class that handles the display connection
 * for the default display.
 *
 * @author fmattias
 */
public class EGLManager
{
	private static EGL10 m_egl = null;
	private static EGLDisplay m_eglDisplay = null;
	private static int[] m_eglVersion = new int[2];

	/**
	 * Initializes the EGL connection to the default display.
	 */
	public static void initEgl()
	{
		if( m_egl != null)
		{
			return;
		}

		m_egl = (EGL10) EGLContext.getEGL( );
		m_eglDisplay = m_egl.eglGetDisplay( EGL10.EGL_DEFAULT_DISPLAY );
		m_egl.eglInitialize( m_eglDisplay, m_eglVersion );
	}

	/**
	 * Returns the default display that has been initialized
	 * or returns null if none exists.
	 *
	 * @return
	 */
	public static EGLDisplay getDisplay()
	{
		return m_eglDisplay;
	}

	/**
	 * Terminates the EGL connection to the default display.
	 */
	public static void terminateEgl()
	{
		if( m_egl == null || m_eglDisplay == null )
		{
			return;
		}

		m_egl.eglTerminate( m_eglDisplay );
		m_egl = null;
	}
}
