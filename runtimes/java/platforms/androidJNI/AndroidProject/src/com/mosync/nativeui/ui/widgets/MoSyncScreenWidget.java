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
		return INVALID_PROPERTY_NAME;
	}

	@Override
	public boolean isLayout()
	{
		return false;
	}
}
