package com.mosync.nativeui.ui.factories;

import static com.mosync.internal.generated.IX_WIDGET.WIDGET_EVENT_CLICKED;

import com.mosync.nativeui.ui.widgets.ButtonWidget;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.internal.android.EventQueue;

import android.app.Activity;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

/**
 * A factory that creates button and sets up the default
 * behavior for sending events when clicked.
 * 
 * @author fmattias
 */
public class ButtonFactory implements AbstractViewFactory
{	
	/**
	 * @see AbstractViewFactory.create.
	 */
	@Override
	public Widget create(Activity activity, int handle)
	{
		Button b = new Button( activity );
		b.setOnClickListener( new MoSyncSendOnClick( handle ) );
		
		return new ButtonWidget(handle, b);
	}
	
	/**
	 * A class responsible for sending mosync widget events
	 * when a button is clicked.
	 * 
	 * @author fmattias
	 */
	class MoSyncSendOnClick implements OnClickListener
	{
		private int m_handle = -1;
		
		/**
		 * Constructor.
		 * 
		 * @param handle The handle of the widget bound to this 
		 *               onclick handler.
		 */
		public MoSyncSendOnClick(int handle)
		{
			m_handle = handle;
		}

		/**
		 * @see OnClickListener.onClick.
		 */
		@Override
		public void onClick(View v)
		{
			EventQueue eventQueue = EventQueue.getDefault( );
			eventQueue.postWidgetEvent( WIDGET_EVENT_CLICKED, m_handle );
		}
	}
}
