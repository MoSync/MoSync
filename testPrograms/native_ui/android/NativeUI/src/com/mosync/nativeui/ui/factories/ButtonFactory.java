package com.mosync.nativeui.ui.factories;

import com.mosync.nativeui.ui.widgets.ButtonWidget;
import com.mosync.nativeui.ui.widgets.Widget;

import android.content.Context;
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
	 * Listener for the buttons.
	 */
	private MoSyncSendOnClick m_onClickListener = new MoSyncSendOnClick( );
	
	/**
	 * @see AbstractViewFactory.create.
	 */
	@Override
	public Widget create(Context context, int handle)
	{
		Button b = new Button( context );
		b.setOnClickListener( m_onClickListener );
		
		return new ButtonWidget(handle, b);
	}
	
	/**
	 * A class that sends an onclick event when the
	 * button is clicked.
	 * 
	 * @author fmattias
	 */
	class MoSyncSendOnClick implements OnClickListener
	{

		@Override
		public void onClick(View v)
		{
			// TODO Send onclikc event to mosync.
		}
	}
}
