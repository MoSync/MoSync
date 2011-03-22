package com.mosync.nativeui.ui.widgets;

import com.mosync.internal.android.MoSyncView;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

/**
 * This screen widget represents the default MoSync canvas
 * screen. It has no properties and does not act as a layout.
 * It only exists so that it can be a part of the widget table.
 * 
 * @author fmattias
 */
public class MoSyncScreenWidget extends Widget
{
	/**
	 * Constructor
	 * 
	 * @param handle handle Integer handle corresponding to this instance.
	 * @param view The MoSyncView wrapped by this widget.
	 */
	public MoSyncScreenWidget(int handle, MoSyncView mosyncScreen)
	{
		super( handle, mosyncScreen );
	}

	@Override
	public boolean setProperty(String property, String value)
			throws PropertyConversionException, InvalidPropertyValueException
	{
		return false;
	}

	@Override
	public String getProperty(String property)
	{
		return "";
	}

	@Override
	public boolean isLayout()
	{
		return false;
	}
}
