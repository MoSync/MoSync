package com.mosync.nativeui.ui.factories;

import static com.mosync.internal.generated.IX_WIDGET.WIDGET_EVENT_GL_VIEW_READY;
import android.app.Activity;
import android.view.ViewGroup.LayoutParams;
import android.widget.FrameLayout;

import com.mosync.internal.android.EventQueue;
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
	/**
	 * @see AbstractViewFactory.create.
	 */
	@Override
	public Widget create(Activity activity, final int handle)
	{
		FrameLayout eglFrame = new FrameLayout( activity );
		EGLView eglView = new EGLView( activity );
		eglView.setLayoutParams( new LayoutParams( LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT ) );
		eglView.setEglViewReadyListener( new EGLViewReadyListener( ) {
			
			@Override
			public void eglViewReady()
			{
				EventQueue.getDefault( ).postWidgetEvent( WIDGET_EVENT_GL_VIEW_READY, handle );
			}
		});
		eglFrame.addView( eglView );
		
		return new GLWidget( handle, eglFrame, eglView );
	}
}
