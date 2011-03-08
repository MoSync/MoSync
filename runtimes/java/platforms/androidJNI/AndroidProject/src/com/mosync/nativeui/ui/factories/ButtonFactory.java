package com.mosync.nativeui.ui.factories;

import android.app.Activity;
import android.widget.Button;

import com.mosync.nativeui.core.Types;
import com.mosync.nativeui.ui.widgets.ButtonWidget;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.util.MoSyncSendOnClick;
import com.mosync.nativeui.util.properties.PropertyConversionException;

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
		
		ButtonWidget button = new ButtonWidget( handle, b );
		
		try
		{
			button.setProperty( Types.WIDGET_PROPERTY_TEXT_HORIZONTAL_ALIGNMENT, Types.WIDGET_HORIZONTAL_CENTER );
			button.setProperty( Types.WIDGET_PROPERTY_TEXT_VERTICAL_ALIGNMENT, Types.WIDGET_VERTICAL_CENTER );
		}
		catch(PropertyConversionException pe)
		{
			// If this happens, there is a bug in the implementation, just return null.
			return null;
		}
		
		return button;
	}
}
