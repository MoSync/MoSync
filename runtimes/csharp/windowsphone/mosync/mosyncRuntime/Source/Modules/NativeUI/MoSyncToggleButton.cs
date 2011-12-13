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
 * @author Gabriela Rata
 *
 * @brief This represents the ToggleButton Widget implementation for the NativeUI
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
        /**
         * The ToggleButton class defining a ToggleButton button. 
         */
        public class ToggleButton : WidgetBaseWindowsPhone
        {
            /**
             * The native ToggleButton widget. 
             */
            protected Microsoft.Phone.Controls.ToggleSwitch mToggleButton;

            /**
             * Constructor 
             */
            public ToggleButton()
            {
                mToggleButton = new Microsoft.Phone.Controls.ToggleSwitch();
                mView = mToggleButton;

                /**
                 * implementation of the Click event
                 */
                mToggleButton.Click += new EventHandler<RoutedEventArgs>(
                   delegate(Object from, RoutedEventArgs evt)
                   {
                       //click event needs a memory chunk of 8 bytes
                       Memory eventData = new Memory(12);

                       //starting with the 0 Byte we write the eventType
                       const int MAWidgetEventData_eventType = 0;
                       //starting with the 4th Byte we write the widgetHandle
                       const int MAWidgetEventData_widgetHandle = 4;
                       //starting with the 8th Byte write the selectedIndex
                       const int MAWidgetEventData_checked = 8;
                       int state = mToggleButton.IsChecked.Value ? 1 : 0;

                       eventData.WriteInt32(MAWidgetEventData_eventType, MoSync.Constants.MAW_EVENT_CLICKED);
                       eventData.WriteInt32(MAWidgetEventData_widgetHandle, mHandle);
                       eventData.WriteInt32(MAWidgetEventData_checked, state);

                       //posting a CustomEvent
                       mRuntime.PostCustomEvent(MoSync.Constants.EVENT_TYPE_WIDGET, eventData);
                   });
                
            }

            /**
             * Property for setting and getting the state of the check box.
             * set: a String containing the values "true" or "false" must be provided.
             * get: returns the state of the checkbox as a String containg "true" or "false"
             */
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
                    
                    if (bool.TryParse(value, out checkedVal))
                    {
                        mToggleButton.IsChecked = checkedVal;
                    }
                }
            }


        }
    }
}

