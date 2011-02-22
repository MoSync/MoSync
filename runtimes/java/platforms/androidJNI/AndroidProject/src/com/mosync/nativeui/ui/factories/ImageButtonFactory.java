package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.widget.ImageButton;

import com.mosync.nativeui.ui.widgets.ImageButtonWidget;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.util.MoSyncSendOnClick;

/**
 * A factory that creates button that is represented by an image, and sets up
 * the default behavior for sending events when clicked.
 * 
 * @author fmattias
 */
public class ImageButtonFactory implements AbstractViewFactory
{
	/**
	 * @see AbstractViewFactory.create.
	 */
	@Override
	public Widget create(Activity activity, int handle)
	{
		ImageButton b = new ImageButton( activity );
		b.setOnClickListener( new MoSyncSendOnClick( handle ) );

		return new ImageButtonWidget( handle, b );
	}
}
