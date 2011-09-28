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
		eglView.setLayoutParams( new LayoutParams( LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT ) );
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
