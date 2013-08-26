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
