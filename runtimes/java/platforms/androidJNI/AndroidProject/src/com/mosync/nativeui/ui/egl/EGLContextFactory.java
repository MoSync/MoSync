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
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;

import com.mosync.internal.generated.IX_OPENGL_ES_MA;
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
		if(glApi == IX_OPENGL_ES_MA.MA_GL_API_GL2) {
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
