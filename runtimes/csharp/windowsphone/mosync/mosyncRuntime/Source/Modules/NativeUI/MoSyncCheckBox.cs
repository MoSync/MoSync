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
/**
 * @file MoSyncCheckBox.cs
 * @author Rata Gabriela
 *
 * @brief This represents the CheckBox Widget implementation for the NativeUI
 *        component on Windows Phone 7, language C#
 *
 * @platform WP 7.1
 **/

using Microsoft.Phone.Controls;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Navigation;
using System;
using System.Text.RegularExpressions;
using System.Reflection;

namespace MoSync
{
	namespace NativeUI
	{
        /**
        * The CheckBox class defines the attributes and behavior of a check box widget
        */
		public class CheckBox : WidgetBaseWindowsPhone
		{
            /**
             * Native CheckBox object
             */
			protected System.Windows.Controls.CheckBox mCheckBox;

            /**
             * Constructor
             */
			public CheckBox()
			{
				mCheckBox = new System.Windows.Controls.CheckBox();
				mView = mCheckBox;

				mCheckBox.Click += new RoutedEventHandler(
				delegate(Object from, RoutedEventArgs evt)
				{
						Memory eventData = new Memory(8);
						const int MAWidgetEventData_eventType = 0;
						const int MAWidgetEventData_widgetHandle = 4;

						eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_CLICKED);
						eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
						mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
				});
			}

            /**
             * Set the state of the check box.
             * @param state True if the check box should be checked, false otherwise.
             */
			public virtual void setState(bool state)
			{
				mCheckBox.IsChecked = state;
			}

            /**
             * Property for setting and getting the state of the check box.
             * set: a String containing the values "true" or "false" must be provided.
             * get: returns the state of the checkbox as a String containg "true" or "false"
             */
			[MoSyncWidgetProperty(MoSync.Constants.MAW_CHECK_BOX_CHECKED)]
			public String Checked
			{
				get
				{
					return mCheckBox.IsChecked.ToString();
				}

				set
				{
					bool val; 
					if(bool.TryParse(value, out val))
					{
						mCheckBox.IsChecked = val;
					}
				}
			}
		}
	}
}
