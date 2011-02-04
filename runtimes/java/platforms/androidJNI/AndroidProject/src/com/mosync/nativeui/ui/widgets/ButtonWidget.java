package com.mosync.nativeui.ui.widgets;

import android.widget.Button;

/**
 * Wraps the behavior of a Button view. Some properties and event handling
 * are extensions of Widget.
 * 
 * @author fmattias
 */
public class ButtonWidget extends LabelWidget
{
	/**
	 * Constructor.
	 * 
	 * @param handle Integer handle corresponding to this instance.
	 * @param button A button wrapped by this widget.
	 */
	public ButtonWidget(int handle, Button button)
	{
		super( handle, button );
	}
}
