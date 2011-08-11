package com.mosync.nativeui.ui.egl;

import com.mosync.internal.generated.IX_OPENGL_ES;
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
	private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;

	/**
	 * Creates a context from the given EGL config and display.
	 *
	 * @param egl The EGL instance.
	 * @param config The EGL context configuration.
	 * @param display The display to which the context is bound to.
	 *
	 * @return The created context.
	 */
	public static EGLContext createContext(EGL10 egl, EGLConfig config, EGLDisplay display, int glApi)
	{
		if(glApi == IX_OPENGL_ES.MA_GL_API_GL2) {
			int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
			return egl.eglCreateContext( display, config, EGL10.EGL_NO_CONTEXT, attrib_list );
		} else {
			return egl.eglCreateContext( display, config, EGL10.EGL_NO_CONTEXT, null );
		}
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
