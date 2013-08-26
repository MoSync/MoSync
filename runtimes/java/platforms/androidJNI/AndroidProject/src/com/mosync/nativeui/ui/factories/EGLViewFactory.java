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

package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.view.ViewGroup.LayoutParams;
import android.widget.FrameLayout;

import com.mosync.internal.android.EventQueue;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.ui.egl.EGLView;
import com.mosync.nativeui.ui.egl.EGLView.EGLViewReadyListener;
import com.mosync.nativeui.ui.widgets.GLWidget;
import com.mosync.nativeui.ui.widgets.Widget;

/**
 * A factory that creates button and sets up the default
 * behavior for sending events when clicked.
 *
 * @author fmattias
 */
public class EGLViewFactory implements AbstractViewFactory
{
	private int m_glApi;

	public EGLViewFactory(int glApi) {
		super( );
		m_glApi = glApi;
	}

	/**
	 * @see AbstractViewFactory.create.
	 */
	@Override
	public Widget create(Activity activity, final int handle)
	{
		FrameLayout eglFrame = new FrameLayout( activity );
		EGLView eglView = null;
		try
		{
			eglView = new EGLView( activity, m_glApi );
		}
		catch(Exception e)
		{
			return null;
		}
		eglView.setLayoutParams( new LayoutParams( LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT ) );
		eglView.setEglViewReadyListener( new EGLViewReadyListener( ) {

			@Override
			public void eglViewReady()
			{
				EventQueue.getDefault( ).postWidgetEvent( IX_WIDGET.MAW_EVENT_GL_VIEW_READY, handle );
			}
		});
		eglFrame.addView( eglView );

		return new GLWidget( handle, eglFrame, eglView );
	}
}
