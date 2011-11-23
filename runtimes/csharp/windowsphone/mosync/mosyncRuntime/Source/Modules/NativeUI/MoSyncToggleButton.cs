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
 * @file MoSyncButton.cs
 * @author Ciprian Filipas
 *
 * @brief This represents the Button Widget implementation for the NativeUI
 *        component on Windows Phone 7, language c#
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
        // The button class
        public class ToggleButton : WidgetBaseWindowsPhone
        {
            protected System.Windows.Controls.Primitives.ToggleButton mToggleButton;

            public ToggleButton()
            {
                mToggleButton = new System.Windows.Controls.Primitives.ToggleButton();
                mView = mToggleButton;

                //mToggleButton.Click += new RoutedEventHandler(
                //   delegate(Object from, RoutedEventArgs evt)
                //   {
                //       //click event needs a memory chunk of 8 bytes
                //       Memory eventData = new Memory(8);

                //       //starting with the 0 Byte we write the eventType
                //       const int MAWidgetEventData_eventType = 0;
                //       //starting with the 4th Byte we write the widgetHandle
                //       const int MAWidgetEventData_widgetHandle = 4;

                //       eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_CLICKED);
                //       eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                //       //posting a CustomEvent
                //       mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                //   });
                
            }

            //checked
            [MoSyncWidgetProperty(MoSync.Constants.MAW_TOGGLE_BUTTON_CHECKED)]
            public String Checked
            {
                get
                {
                    return mToggleButton.IsChecked.ToString();
                }
                set
                {
                    bool checkedVal;
                    bool.TryParse(value, out checkedVal);
                    mToggleButton.IsChecked = checkedVal;
                }
            }


        }
    }
}

